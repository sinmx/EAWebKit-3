/*
 * Copyright (C) 2007, 2008, 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Justin Haygood (jhaygood@reaktix.com)
 * Copyright (C) 2011 Electronic Arts, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef ThreadingPrimitives_h
#define ThreadingPrimitives_h

#include "Platform.h"

#include <wtf/Assertions.h>
#include <wtf/FastAllocBase.h>
#include <wtf/Locker.h>
#include <wtf/Noncopyable.h>

#if OS(WINDOWS)
#include <windows.h>
#endif

#if USE(PTHREADS)
#include <pthread.h>
#elif PLATFORM(GTK)
#include "GOwnPtr.h"
#endif

#if PLATFORM(QT)
#include <qglobal.h>
QT_BEGIN_NAMESPACE
class QMutex;
class QWaitCondition;
QT_END_NAMESPACE
#endif

//+EAWebKitChange
//12/7/2011
#if PLATFORM(EA)
namespace EA
{
namespace WebKit
{
class IMutex;
class IThreadCondition;
}
}
#endif
//-EAWebKitChange
namespace WTF {


#if USE(PTHREADS)
typedef pthread_mutex_t PlatformMutex;
#if HAVE(PTHREAD_RWLOCK)
typedef pthread_rwlock_t PlatformReadWriteLock;
#else
typedef void* PlatformReadWriteLock;
#endif
typedef pthread_cond_t PlatformCondition;
#elif PLATFORM(GTK)
typedef GOwnPtr<GMutex> PlatformMutex;
typedef void* PlatformReadWriteLock; // FIXME: Implement.
typedef GOwnPtr<GCond> PlatformCondition;
#elif PLATFORM(QT)
typedef QT_PREPEND_NAMESPACE(QMutex)* PlatformMutex;
typedef void* PlatformReadWriteLock; // FIXME: Implement.
typedef QT_PREPEND_NAMESPACE(QWaitCondition)* PlatformCondition;
//+EAWebKitChange
//12/7/2011
#elif PLATFORM(EA)
typedef EA::WebKit::IMutex* PlatformMutex;
typedef void* PlatformReadWriteLock;  
typedef EA::WebKit::IThreadCondition* PlatformCondition;
//-EAWebKitChange
#elif OS(WINDOWS)
struct PlatformMutex {
	CRITICAL_SECTION m_internalMutex;
	size_t m_recursionCount;
};
//-EAWebKitChange
typedef void* PlatformReadWriteLock; // FIXME: Implement.
struct PlatformCondition {
    size_t m_waitersGone;
    size_t m_waitersBlocked;
    size_t m_waitersToUnblock; 
    HANDLE m_blockLock;
    HANDLE m_blockQueue;
    HANDLE m_unblockLock;

    bool timedWait(PlatformMutex&, DWORD durationMilliseconds);
    void signal(bool unblockAll);
};
#else
typedef void* PlatformMutex;
typedef void* PlatformReadWriteLock;
typedef void* PlatformCondition;
#endif
    
class Mutex {
    WTF_MAKE_NONCOPYABLE(Mutex); WTF_MAKE_FAST_ALLOCATED;
public:
    Mutex();
    ~Mutex();

    void lock();
    bool tryLock();
    void unlock();

public:
    PlatformMutex& impl() { return m_mutex; }
private:
    PlatformMutex m_mutex;
};

typedef Locker<Mutex> MutexLocker;

class ReadWriteLock {
    WTF_MAKE_NONCOPYABLE(ReadWriteLock);
public:
    ReadWriteLock();
    ~ReadWriteLock();

    void readLock();
    bool tryReadLock();

    void writeLock();
    bool tryWriteLock();
    
    void unlock();

private:
    PlatformReadWriteLock m_readWriteLock;
};

class ThreadCondition {
    WTF_MAKE_NONCOPYABLE(ThreadCondition);
public:
    ThreadCondition();
    ~ThreadCondition();
    
    void wait(Mutex& mutex);
    // Returns true if the condition was signaled before absoluteTime, false if the absoluteTime was reached or is in the past.
    // The absoluteTime is in seconds, starting on January 1, 1970. The time is assumed to use the same time zone as WTF::currentTime().
    bool timedWait(Mutex&, double absoluteTime);
    void signal();
    void broadcast();
    
private:
    PlatformCondition m_condition;
};

#if OS(WINDOWS) || defined(CS_UNDEFINED_STRING)
// The absoluteTime is in seconds, starting on January 1, 1970. The time is assumed to use the same time zone as WTF::currentTime().
// Returns an interval in milliseconds suitable for passing to one of the Win32 wait functions (e.g., ::WaitForSingleObject).
DWORD absoluteTimeToWaitTimeoutInterval(double absoluteTime);
#endif

} // namespace WTF

using WTF::Mutex;
using WTF::MutexLocker;
using WTF::ThreadCondition;

#if OS(WINDOWS)
using WTF::absoluteTimeToWaitTimeoutInterval;
#endif

#endif // ThreadingPrimitives_h
