/*
Copyright (C) 2007,2009-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
// EAFileNotification.cpp
//
// Created and maintained by Paul Pedriana - 2007
//
// Implements automatic notifications of changes in the file system.
///////////////////////////////////////////////////////////////////////////////


#include <EAIO/internal/Config.h>
#include <EAIO/EAFileNotification.h>
#include <EAIO/FnEncode.h>
#include <EAIO/PathString.h>
#include EA_ASSERT_HEADER



///////////////////////////////////////////////////////////////////////////////
// Windows implementation
//


#include <EAIO/EAFileDirectory.h>
#include <EAIO/EAFileUtil.h>


#ifdef _MSC_VER
    #pragma warning(disable: 4127) // conditional expression is constant
#endif


/// GetCurrentFCNTime
///
/// Returns the current time in milliseconds.
/// It's less important that the absolute time be accurate but rather
/// that relative milliseconds be accurate.
/// 
static int GetCurrentFCNTime()
{
        return 0; // Fill in others here as needed.
}


namespace EA{

namespace IO{



///////////////////////////////////////////////////////////////////////////////
// FSEntryHashCompare::operator()
//
// Note that we don't has the pointer but has the value.
//
size_t FileChangeNotification::FSEntryHashCompare::operator()(const FSEntryARC& fseARC) const
{
    const char16_t* pString = fseARC->msName.c_str();
    size_t c, nValue = 0x811c9dc5; // Use the FNV string hash algorithm.
    while((c = (size_t)((uint16_t)*pString++)) != 0)
        nValue = (size_t)((nValue * 16777619) ^ c);
    return nValue;
}


///////////////////////////////////////////////////////////////////////////////
// FSEntryHashCompare::operator()
//
// Note that we don't compare pointers but instead compare values.
//
bool FileChangeNotification::FSEntryHashCompare::operator()(const FSEntryARC& fseARC1, const FSEntryARC& fseARC2) const 
{
    return (fseARC1->mpParentEntry == fseARC2->mpParentEntry) && (fseARC1->msName == fseARC2->msName);
}                                                                                                 


///////////////////////////////////////////////////////////////////////////////
// FileChangeNotification
//
FileChangeNotification::FileChangeNotification(EA::Allocator::ICoreAllocator* pCoreAllocator)
  : mbInitialized(false),
    mpCoreAllocator(pCoreAllocator ? pCoreAllocator : IO::GetAllocator()),
    //mDirectoryPath(),
    mnChangeTypeFlags(kChangeTypeFlagNone),
    mnOptionFlags(kOptionFlagDefaults),
    mpFileChangeCallback(NULL),
    mpFileChangeCallbackContext(NULL),
  #if EAIO_THREAD_SAFETY_ENABLED
    mnBusy(0),
    mThread(),
  #endif
    mbThreadShouldRun(false),
    mbStarted(false),
    mEntryDirectoryRoot(),
    mpEntryDirectoryCurrent(NULL),
    mCurrentChangeSet(EA::Allocator::EAIOEASTLCoreAllocator(EAIO_ALLOC_PREFIX "EAFileChangeNotification/FSEntrySet", mpCoreAllocator))
{
    mDirectoryPath[0] = 0;
    mEntryDirectoryRoot.mpCoreAllocator = mpCoreAllocator;
}


///////////////////////////////////////////////////////////////////////////////
// ~FileChangeNotification
//
FileChangeNotification::~FileChangeNotification()
{
    Shutdown();
}


///////////////////////////////////////////////////////////////////////////////
// Init
//
bool FileChangeNotification::Init()
{
    if(!mbInitialized)
    {
        // Nothing special required for this platform.
        mbInitialized = true;
    }
    return true;
}


///////////////////////////////////////////////////////////////////////////////
// Shutdown
//
bool FileChangeNotification::Shutdown()
{
    if(mbInitialized)
    {
        mbInitialized = false;
        if(mbStarted)
            Stop();
        return true;
    }
    return false;
}


///////////////////////////////////////////////////////////////////////////////
// Start
//
bool FileChangeNotification::Start()
{
    if(!mbStarted)
    {
        if(!mpCoreAllocator)
            SetAllocator(IO::GetAllocator());

        if(EA::IO::Directory::Exists(mDirectoryPath))
        {
            mbStarted = true;

            mEntryDirectoryRoot.mChildEntrySet.clear(); // This will Release all entries recursively.
            BuildEntry(&mEntryDirectoryRoot, mDirectoryPath, NULL, true);
            mpEntryDirectoryCurrent = NULL;
            // TraceEntryTree(&mEntryDirectoryRoot); // Debugging only.

            #if EAIO_THREAD_SAFETY_ENABLED
                if(mnOptionFlags & kOptionFlagMultithreaded)
                {
                    mbThreadShouldRun = true;
                    Thread::ThreadParameters tp;
                    // In this thread we need more than the default 8K
                    tp.mnStackSize = 32768;
                    mThread.Begin(this, NULL, &tp);
                }
            #endif
        }
    }
    return mbStarted;
}


///////////////////////////////////////////////////////////////////////////////
// GetIsStarted
//
bool FileChangeNotification::GetIsStarted() const
{
    return mbStarted;
}


    ///////////////////////////////////////////////////////////////////////////////
// Stop
//
bool FileChangeNotification::Stop()
{
    if(mbStarted)
    {
        mbStarted = false;

        #if EAIO_THREAD_SAFETY_ENABLED
            if(mThread.GetStatus() == EA::Thread::Thread::kStatusRunning)
            {
                mbThreadShouldRun = false;
                mThread.WaitForEnd();
            }
        #endif
    }
    return true;
}


///////////////////////////////////////////////////////////////////////////////
// Poll
//
// Iterates the file system, looking for changes.
//
void FileChangeNotification::Poll(int timeoutMS)
{
    if(mbStarted)
    {
        #if EAIO_THREAD_SAFETY_ENABLED
        if(mnBusy.SetValueConditional(1, 0)) // If we can grab the mini-mutex...
        #endif
        {
            const int timeoutAbsolute = GetCurrentFCNTime() + timeoutMS;
            if(!mpEntryDirectoryCurrent)
            {
                mpEntryDirectoryCurrent = &mEntryDirectoryRoot;
                mEntryDirectoryRoot.mChildEntrySetIt = mEntryDirectoryRoot.mChildEntrySet.begin();
            }
            mCurrentChangeSet.clear();

            PollInternal(mpEntryDirectoryCurrent, timeoutAbsolute);

            for(FSEntrySet::iterator it(mCurrentChangeSet.begin()); it != mCurrentChangeSet.end(); ++it)
            {
                const FSEntry* const pFSEntry = *it;

                char16_t pDirectoryPath[EA::IO::kMaxPathLength];
                EA_ASSERT(!pFSEntry->mbDirectory && pFSEntry->mpParentEntry);
                GetEntryPath(pFSEntry->mpParentEntry, pDirectoryPath);

                #if defined(EA_TRACE_ENABLED) && EA_TRACE_ENABLED // If EATrace is available and we aren't instead using something else...
                    EA_TRACE_FORMATTED(("FileChangeNotification: File changed: %d \"%ls%ls\"\n", pFSEntry->mnChangeFlags, pDirectoryPath, pFSEntry->msName.c_str()));
                #endif

                SendNotification(pDirectoryPath, pFSEntry->msName.c_str(), pFSEntry->mnChangeFlags);
            }

            #if EAIO_THREAD_SAFETY_ENABLED
                mnBusy = 0;
            #endif
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// PollInternal
//
// What we implement here is directory tree iteration that can be 
// interrupted at any time and which collects detected changes 
// as it goes. Pretty tricky indeed.
//
bool FileChangeNotification::PollInternal(FSEntry* pFSEntryBase, int timeoutAbsolute)
{
    bool bTimeExpired = false;

    EA_ASSERT(pFSEntryBase->mbDirectory);

    // Get a list of the current file entries in this directory.
    // We will compare these to our tree.
    FSEntrySet childEntrySet(EA::Allocator::EAIOEASTLCoreAllocator(EAIO_ALLOC_PREFIX "EAFileChangeNotification/FSEntrySet", mpCoreAllocator));
    GetChildFileEntrySet(pFSEntryBase, childEntrySet);

    // We continue our iteration of this directory where we last left off.
    while((pFSEntryBase->mChildEntrySetIt != pFSEntryBase->mChildEntrySet.end()) && !bTimeExpired)
    {
        FSEntry* const pFSEntryTemp = *pFSEntryBase->mChildEntrySetIt;
        bool bAdvanceToNext = true;     // True if we need to advance to the next dir

        if(pFSEntryTemp->mbDirectory)
        {
            // We ignore the case of this directory having been removed.
            if(EntryExists(pFSEntryTemp))
            {
                #if defined(EA_TRACE_ENABLED) && EA_TRACE_ENABLED && FCN_POLL_TRACE_ENABLED
                    char16_t pPath[EA::IO::kMaxPathLength]; // Will be a directory path ending in directory separator or will be a file path.
                    GetEntryPath(pFSEntryTemp, pPath);
                    EA_TRACE_FORMATTED(("FileChangeNotification: Polling directory: \"%ls\"...\n", pPath));
                #endif

                mpEntryDirectoryCurrent        = pFSEntryTemp;
                pFSEntryTemp->mChildEntrySetIt = pFSEntryTemp->mChildEntrySet.begin();
                bTimeExpired = PollInternal(pFSEntryTemp, timeoutAbsolute);
            }
        }
        else
        {
            #if defined(EA_TRACE_ENABLED) && EA_TRACE_ENABLED && FCN_POLL_TRACE_ENABLED
                char16_t pPath[EA::IO::kMaxPathLength]; // Will be a directory path ending in directory separator or will be a file path.
                GetEntryPath(pFSEntryTemp, pPath);
                EA_TRACE_FORMATTED(("FileChangeNotification: Polling file: \"%ls\"\n", pPath));
            #endif

            FSEntrySet::iterator it = childEntrySet.find(pFSEntryTemp);

            if(it == childEntrySet.end()) // If fsEntryTemp was removed...
            {
                // Add fsEntryTemp to the file change list.
                pFSEntryTemp->mnChangeFlags = kChangeTypeFlagFileName;
                mCurrentChangeSet.insert(pFSEntryTemp);

                // Remove fsEntryTemp from our tree.
                pFSEntryBase->mChildEntrySetIt = pFSEntryBase->mChildEntrySet.erase( pFSEntryBase->mChildEntrySetIt );
                bAdvanceToNext = false; // We erased the entry, so we don't need to advance
            }
            else
            {
                // Get/update the change time of fsEntryTemp.
                time_t nCurrentTime;
                size_type nCurrentSize;
                GetFileTimeAndSize(pFSEntryTemp, &nCurrentTime, &nCurrentSize);

                if(nCurrentTime != pFSEntryTemp->mnTime || nCurrentSize != pFSEntryTemp->mnSize)
                {
                    // Add fsEntryTemp to the file change list.
                    if(nCurrentTime != pFSEntryTemp->mnTime)
                    {
                        pFSEntryTemp->mnChangeFlags |= kChangeTypeFlagModified;
                        pFSEntryTemp->mnTime = nCurrentTime;
                    }
                    if(nCurrentSize != pFSEntryTemp->mnSize)
                    {
                        pFSEntryTemp->mnChangeFlags |= kChangeTypeFlagSize;
                        pFSEntryTemp->mnSize = nCurrentSize;
                    }

                    mCurrentChangeSet.insert(pFSEntryTemp);
                }

                // We won't be needing to refer to this entry any more, 
                // so we can be slightly more efficient by removing it now.
                childEntrySet.erase(it);
            }
        }

        if(!bTimeExpired)
        {
            if (bAdvanceToNext)
                ++pFSEntryBase->mChildEntrySetIt;
            bTimeExpired = (GetCurrentFCNTime() >= timeoutAbsolute);
        }
    }

    // We are done with this directory. Reset our iterator and go back to the parent directory.
    if(pFSEntryBase->mChildEntrySetIt == pFSEntryBase->mChildEntrySet.end())
    {
        // Now check for any entries in childEntrySet that weren't in our tree. 
        // These entries would be new entries to the tree.
        for(FSEntrySet::iterator it = childEntrySet.begin(); it != childEntrySet.end(); ++it)
        {
            FSEntry* const pFSEntryTemp = *it;

            if(pFSEntryBase->mChildEntrySet.find(pFSEntryTemp) == pFSEntryBase->mChildEntrySet.end()) // If we appear to have a new file...
            {
                // Add fsEntryTemp to the file change list.
                pFSEntryTemp->mnChangeFlags = kChangeTypeFlagFileName;
                mCurrentChangeSet.insert(pFSEntryTemp);

                // Add the new fsEntry to our tree. This insert might result in invalidating
                // mChildEntrySetIt, but that's OK because we will be resetting it to 
                // mChildEntrySet.begin() below.
                pFSEntryBase->mChildEntrySet.insert(pFSEntryTemp);
            }
        }

        pFSEntryBase->mChildEntrySetIt = pFSEntryBase->mChildEntrySet.begin();
        mpEntryDirectoryCurrent        = pFSEntryBase->mpParentEntry;

        // If the time has expired, then advance the iterator to skip over the
        // parent directory (since the caller won't do it with the expired flag set.)
        if(bTimeExpired && mpEntryDirectoryCurrent)
            ++mpEntryDirectoryCurrent->mChildEntrySetIt;
        else
            bTimeExpired = true; // We are completely done with the tree; trigger an exit from this function.
    }

    return bTimeExpired;
}


///////////////////////////////////////////////////////////////////////////////
// Run
//
intptr_t FileChangeNotification::Run(void* /*pContext*/)
{
    while(mbThreadShouldRun)
    {
        Poll(100);

        #if EAIO_THREAD_SAFETY_ENABLED
            EA::Thread::ThreadSleep(3000);
        #else
            // To consider: Implement something.
        #endif
    }

    return 0;
}


int FileChangeNotification::FSEntry::AddRef()
{
    return ++mnRefCount;
}


int FileChangeNotification::FSEntry::Release()
{
    if(mnRefCount > 1)
        return --mnRefCount;

    EA_ASSERT(mpCoreAllocator != NULL);
    EA_CA_DELETE(this, mpCoreAllocator);
    return 0;
}


void FileChangeNotification::FSEntry::SetAllocator(EA::Allocator::ICoreAllocator* pCoreAllocator)
{
    mpCoreAllocator = pCoreAllocator;
    mChildEntrySet.get_allocator().set_allocator(pCoreAllocator);

    #if defined(EASTL_VERSION_N) && (EASTL_VERSION_N >= 11000)
        msName.get_overflow_allocator().set_allocator(pCoreAllocator);
    #endif
}


///////////////////////////////////////////////////////////////////////////////
// GetChildFileEntrySet
//
void FileChangeNotification::GetChildFileEntrySet(FSEntry* pFSEntryBase, FSEntrySet& childEntrySet)
{
    DirectoryIterator            directoryIterator;
    DirectoryIterator::EntryList entryList(DirectoryIterator::EntryList::allocator_type(EASTL_NAME_VAL(EAIO_ALLOC_PREFIX "EAFileNotification/FSEntry")));
    char16_t                     pPath[EA::IO::kMaxPathLength]; // Will be a directory path ending in directory separator or will be a file path.

    GetEntryPath(pFSEntryBase, pPath);

    if(directoryIterator.Read(pPath, entryList, NULL, kDirectoryEntryFile))
    {
        for(DirectoryIterator::EntryList::iterator it = entryList.begin(); it != entryList.end(); ++it)
        {
            const DirectoryIterator::Entry& entry = *it;

            EA_ASSERT(pFSEntryBase->mpCoreAllocator != NULL);
            FSEntryARC pFSEntry = EA_CA_NEW(FSEntry, pFSEntryBase->mpCoreAllocator, EAIO_ALLOC_PREFIX "EAFileChangeNotification/FSEntry");

            pFSEntry->msName.assign(entry.msName.data(), entry.msName.length());
            pFSEntry->mbDirectory = false;
            pFSEntry->mpParentEntry = pFSEntryBase;
            pFSEntry->mnChangeFlags = 0;
            pFSEntry->mpCoreAllocator = pFSEntryBase->mpCoreAllocator;
            GetFileTimeAndSize(pFSEntry, &pFSEntry->mnTime, &pFSEntry->mnSize);

            childEntrySet.insert(pFSEntry); // AutoRefCount operation.
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// GetEntryPath
//
// Gets a full path from a single entry.
//
void FileChangeNotification::GetEntryPath(const FSEntry* pFSEntry, char16_t* pPath, 
                                          uint32_t nMaxPermittedLength)
{
    const FSEntry*  entryArray[32];
    int             entryArrayCount = 0;
    char16_t*       pPathCurrent = pPath;

    while(pFSEntry)
    {
        entryArray[entryArrayCount++] = pFSEntry;
        pFSEntry = pFSEntry->mpParentEntry;
    }

    while(--entryArrayCount >= 0)
    {
        const char16_t* const pCurrent = entryArray[entryArrayCount]->msName.c_str();
        const bool      bDirectory     = entryArray[entryArrayCount]->mbDirectory;
        const uint32_t  nCurrentLength = (uint32_t)EAIOStrlen16(pCurrent);

        EAIOStrlcpy16(pPathCurrent, pCurrent, nMaxPermittedLength);

        pPathCurrent        += nCurrentLength;
        nMaxPermittedLength -= nCurrentLength;

        if((entryArrayCount > 0) || bDirectory)
        {
            if(EA::IO::Path::EnsureTrailingSeparator(pPath, nMaxPermittedLength))  // If one was added...
                ++pPathCurrent;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// EntryExists
//
bool FileChangeNotification::EntryExists(const FSEntry* pFSEntry)
{
    char16_t pPath[EA::IO::kMaxPathLength]; // Will be a directory path ending in directory separator or will be a file path.
    GetEntryPath(pFSEntry, pPath);
    if(pFSEntry->mbDirectory)
        return EA::IO::Directory::Exists(pPath);
    return EA::IO::File::Exists(pPath);
}


///////////////////////////////////////////////////////////////////////////////
// GetFileTimeAndSize
//
void FileChangeNotification::GetFileTimeAndSize(const FSEntry* pFSEntry, time_t* pTime, size_type* pSize)
{
    char16_t pPath[EA::IO::kMaxPathLength]; // Will be a directory path ending in directory separator or will be a file path.
    GetEntryPath(pFSEntry, pPath);
    (*pTime) = EA::IO::File::GetTime(pPath, EA::IO::kFileTimeTypeLastModification);
    (*pSize) = EA::IO::File::GetSize(pPath);
}


///////////////////////////////////////////////////////////////////////////////
// BuildEntry
//
// Sets up the given entry and recursively sets up child entries.
//
void FileChangeNotification::BuildEntry(FSEntry* pFSEntryBase, const char16_t* pName, FSEntry* pEntryParent, bool bDirectory)
{
    pFSEntryBase->msName        = pName;
    pFSEntryBase->mbDirectory   = bDirectory;
    pFSEntryBase->mpParentEntry = pEntryParent;
    pFSEntryBase->mnChangeFlags = 0;

    // We remove a trailing file separator from the name if it is present.
    if(bDirectory && !pFSEntryBase->msName.empty() && EA::IO::IsFilePathSeparator(pFSEntryBase->msName.back()))
        pFSEntryBase->msName.pop_back();

    char16_t pPath[EA::IO::kMaxPathLength]; // Will be a directory path ending in directory separator or will be a file path.
    GetEntryPath(pFSEntryBase, pPath);

    if(bDirectory)
    {
        pFSEntryBase->mnTime = 0;
        pFSEntryBase->mnSize = 0;
    }
    else
    {
        pFSEntryBase->mnTime = EA::IO::File::GetTime(pPath, EA::IO::kFileTimeTypeLastModification);
        pFSEntryBase->mnSize = EA::IO::File::GetSize(pPath);
    }

    if(bDirectory)
    {
        DirectoryIterator            directoryIterator;
        DirectoryIterator::EntryList entryList(DirectoryIterator::EntryList::allocator_type(EASTL_NAME_VAL(EAIO_ALLOC_PREFIX "EAFileNotification/FSEntry"), mpCoreAllocator));
        const int                    nEntryFlags = (mnOptionFlags & kOptionFlagWatchSubdirectories) ? kDirectoryEntryDirectory | kDirectoryEntryFile : kDirectoryEntryFile;

        if(directoryIterator.Read(pPath, entryList, NULL, nEntryFlags))
        {
            for(DirectoryIterator::EntryList::iterator it = entryList.begin(); it != entryList.end(); ++it)
            {
                const DirectoryIterator::Entry& entry = *it;

                FSEntryARC pFSEntry = EA_CA_NEW(FSEntry, pFSEntryBase->mpCoreAllocator, EAIO_ALLOC_PREFIX "EAFileChangeNotification/FSEntry");
                pFSEntry->mpCoreAllocator = pFSEntryBase->mpCoreAllocator;

                BuildEntry(pFSEntry, entry.msName.c_str(), pFSEntryBase, entry.mType == kDirectoryEntryDirectory);
                pFSEntryBase->mChildEntrySet.insert(pFSEntry); // AutoRefCount operation.
            }
        }
    }    
}


///////////////////////////////////////////////////////////////////////////////
// TraceEntryTree
//
// Prints out a user-readable text tree of the file system as we see it.
//
void FileChangeNotification::TraceEntryTree(const FSEntry* pFSEntry, int nCurrentDepth)
{
    #if defined(EA_TRACE_ENABLED) && EA_TRACE_ENABLED
        Path::PathString8 path8;
        const int  kSpaceSize = 3;
        const int  kMaxDepth  = 32;
        char       leadingSpace[(kMaxDepth + 1) * kSpaceSize];
        int        i;

        // If this is the first time through, print the tree base.
        if(nCurrentDepth == 0)
        {
            ConvertPathUTF16ToUTF8(path8, pFSEntry->msName.c_str());

            #if defined(EA_TRACE_ENABLED) && EA_TRACE_ENABLED // If EATrace is available and we aren't instead using something else...
                if(pFSEntry->mbDirectory)
                    EA_TRACE_FORMATTED(("%s%c", path8.c_str(), EA::IO::kFilePathSeparatorString8));
                else
                    EA_TRACE_MESSAGE(path8.c_str());
            #endif

            ++nCurrentDepth;
        }

        // Setup the indenting space.
        for(i = 0; (i < nCurrentDepth) && (i < kMaxDepth); i++){
            for(int j = 0; j < kSpaceSize; j++)
                leadingSpace[(i * kSpaceSize) + j] = ' ';
        }
        leadingSpace[i * kSpaceSize] = 0;

        // Trace the elements.
        for(FSEntrySet::const_iterator it = pFSEntry->mChildEntrySet.begin(); it != pFSEntry->mChildEntrySet.end(); ++it)
        {
            const FSEntry* const pFSEntry = *it;
            ConvertPathUTF16ToUTF8(path8, pFSEntry->msName.c_str());

            #if defined(EA_TRACE_ENABLED) && EA_TRACE_ENABLED // If EATrace is available and we aren't instead using something else...
                if(pFSEntry->mbDirectory)
                    EA_TRACE_FORMATTED(("%s%s%c", leadingSpace, path8.c_str(), EA::IO::kFilePathSeparatorString8));
                else
                    EA_TRACE_FORMATTED(("%s%s", leadingSpace, path8.c_str()));
            #endif

            if(pFSEntry->mbDirectory)
                TraceEntryTree(pFSEntry, nCurrentDepth + 1);
        }
    #else
        (void)pFSEntry;
        (void)nCurrentDepth;
    #endif
}


///////////////////////////////////////////////////////////////////////////////
// GetWatchedDirectory
//
const char16_t* FileChangeNotification::GetWatchedDirectory() const
{
    return mDirectoryPath;
}


///////////////////////////////////////////////////////////////////////////////
// SetWatchedDirectory
//
bool FileChangeNotification::SetWatchedDirectory(const char16_t* pDirectory)
{
    if(!mbStarted)
    {
        EAIOStrlcpy16(mDirectoryPath, pDirectory, kMaxDirectoryLength);
        return true;
    }
    return false;
}

bool FileChangeNotification::SetWatchedDirectory(const char8_t* pDirectory)
{
    Path::PathString16 directory16;
    ConvertPathUTF8ToUTF16(directory16, pDirectory);

    return SetWatchedDirectory(directory16.c_str());
}



///////////////////////////////////////////////////////////////////////////////
// SetNotificationCallback
//
void FileChangeNotification::SetNotificationCallback(FileChangeCallback pFileChangeCallback, void* pContext)
{
    mpFileChangeCallback        = pFileChangeCallback;
    mpFileChangeCallbackContext = pContext;
}


///////////////////////////////////////////////////////////////////////////////
// GetChangeTypeFlags
//
int FileChangeNotification::GetChangeTypeFlags() const
{
    return mnChangeTypeFlags;
}


///////////////////////////////////////////////////////////////////////////////
// SetChangeTypeFlags
//
bool FileChangeNotification::SetChangeTypeFlags(int nChangeTypeFlags)
{
    if(!mbStarted)
    {
        mnChangeTypeFlags = nChangeTypeFlags;
        return true;
    }
    return false;
}


///////////////////////////////////////////////////////////////////////////////
// GetOptionFlags
//
int FileChangeNotification::GetOptionFlags() const
{
    return mnOptionFlags;
}


///////////////////////////////////////////////////////////////////////////////
// SetOptionFlags
//
bool FileChangeNotification::SetOptionFlags(int nOptionFlags)
{
    if(!mbStarted)
    {
        mnOptionFlags = nOptionFlags;
        return true;
    }
    return false;
}


///////////////////////////////////////////////////////////////////////////////
// SetAllocator
//
void FileChangeNotification::SetAllocator(EA::Allocator::ICoreAllocator* pCoreAllocator)
{
    mpCoreAllocator = pCoreAllocator;
    mCurrentChangeSet.get_allocator().set_allocator(pCoreAllocator);
    mEntryDirectoryRoot.SetAllocator(pCoreAllocator);
}


///////////////////////////////////////////////////////////////////////////////
// SendNotification
//
void FileChangeNotification::SendNotification(const char16_t* pDirectory, const char16_t* pFile, int nChangeTypeFlags)
{
    if(mpFileChangeCallback)
    {
        // In this implementation we don't really know which change type 
        // occurred, so we just pass back mnChangeTypeFlags.
        mpFileChangeCallback(this, pDirectory, pFile, nChangeTypeFlags, mpFileChangeCallbackContext);
    }
}


} // namespace EA

} // namespace IO


