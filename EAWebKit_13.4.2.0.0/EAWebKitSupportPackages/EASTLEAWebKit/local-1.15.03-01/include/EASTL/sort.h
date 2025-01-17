/*
Copyright (C) 2005,2009,2010,2012 Electronic Arts, Inc.  All rights reserved.

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
// EASTL/sort.h
// Written by Paul Pedriana - 2005.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// This file implements sorting algorithms. Some of these are equivalent to 
// std C++ sorting algorithms, while others don't have equivalents in the 
// C++ standard. We implement the following sorting algorithms:
//    is_sorted             -- 
//    sort                  -- Unstable. The implementation of this is simply mapped to quick_sort.
//    quick_sort            -- Unstable.
//    tim_sort              -- Stable.
//    tim_sort_buffer       -- Stable.
//    partial_sort          -- Unstable.
//    insertion_sort        -- Stable. 
//    shell_sort            -- Unstable.
//    heap_sort             -- Stable. 
//    stable_sort           -- Stable. The implementation of this is simply mapped to merge_sort.
//    merge                 -- 
//    merge_sort            -- Stable. 
//    merge_sort_buffer     -- Stable. 
//    nth_element           -- Unstable.
//    radix_sort*           -- Stable. Important and useful sort for integral data.
//    comb_sort*            -- Unstable.
//    bubble_sort*          -- Stable. Useful in practice for sorting tiny sets of data (<= 10 elements).
//    selection_sort*       -- Unstable.
//    shaker_sort*          -- Stable.
//    bucket_sort*          -- Stable. 
//
// * Found in sort_extra.h.
//
// Additional sorting and related algorithms we may want to implement:
//    partial_sort_copy     This would be like the std STL version.
//    paritition            This would be like the std STL version. This is not categorized as a sort routine by the language standard.
//    stable_partition      This would be like the std STL version.
//    counting_sort         Maybe we don't want to implement this.
//
//////////////////////////////////////////////////////////////////////////////


#ifndef EASTL_SORT_H
#define EASTL_SORT_H


#include <EASTL/internal/config.h>
#include <EASTL/iterator.h>
#include <EASTL/memory.h>
#include <EASTL/algorithm.h>
#include <EASTL/functional.h>
#include <EASTL/heap.h>
#include <EASTL/allocator.h>
#include <EASTL/memory.h>


#if defined(EA_PRAGMA_ONCE_SUPPORTED)
    #pragma once // Some compilers (e.g. VC++) benefit significantly from using this. We've measured 3-4% build speed improvements in apps as a result.
#endif



namespace eastl
{

    /// is_sorted
    ///
    /// Returns true if the range [first, last) is sorted.
    /// An empty range is considered to be sorted.
    /// To test if a range is reverse-sorted, use 'greater' as the comparison 
    /// instead of 'less'.
    ///
    /// Example usage:
    ///    vector<int> intArray;
    ///    bool bIsSorted        = is_sorted(intArray.begin(), intArray.end());
    ///    bool bIsReverseSorted = is_sorted(intArray.begin(), intArray.end(), greater<int>());
    ///
    template <typename ForwardIterator, typename StrictWeakOrdering>
    bool is_sorted(ForwardIterator first, ForwardIterator last, StrictWeakOrdering compare)
    {
        if(first != last)
        {
            ForwardIterator current = first;

            for(++current; current != last; first = current, ++current)
            {
                if(compare(*current, *first))
                {
                    EASTL_VALIDATE_COMPARE(!compare(*first, *current)); // Validate that the compare function is sane.
                    return false;
                }
            }
        }
        return true;
    }

    template <typename ForwardIterator>
    inline bool is_sorted(ForwardIterator first, ForwardIterator last)
    {
        typedef eastl::less<typename eastl::iterator_traits<ForwardIterator>::value_type> Less;

        return eastl::is_sorted<ForwardIterator, Less>(first, last, Less());
    }



    /// merge
    ///
    /// This function merges two sorted input sorted ranges into a result sorted range.
    /// This merge is stable in that no element from the first range will be changed
    /// in order relative to other elements from the first range.
    ///
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
    OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare compare)
    {
        while((first1 != last1) && (first2 != last2))
        {
            if(compare(*first2, *first1))
            {
                EASTL_VALIDATE_COMPARE(!compare(*first1, *first2)); // Validate that the compare function is sane.
                *result = *first2;
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
            }
            ++result;
        }

        return eastl::copy(first2, last2, eastl::copy(first1, last1, result));
    }

    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    inline OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
    {
        typedef eastl::less<typename eastl::iterator_traits<InputIterator1>::value_type> Less;

        return eastl::merge<InputIterator1, InputIterator2, OutputIterator, Less>
                           (first1, last1, first2, last2, result, Less());
    }



    /// insertion_sort
    ///
    /// Since insertion_sort requires that the data be addressed with a BidirectionalIterator and 
    /// not the more flexible RandomAccessIterator, we implement the sort by doing a for loop within
    /// a for loop. If we were to specialize this for a RandomAccessIterator, we could replace the
    /// inner for loop with a call to upper_bound, which would be faster.
    ///
    template <typename BidirectionalIterator, typename StrictWeakOrdering>
    void insertion_sort(BidirectionalIterator first, BidirectionalIterator last, StrictWeakOrdering compare)
    {
        typedef typename eastl::iterator_traits<BidirectionalIterator>::value_type value_type;

        if(first != last) // if the range is non-empty...
        {
            BidirectionalIterator iCurrent, iNext, iSorted = first;

            for(++iSorted; iSorted != last; ++iSorted)
            {
                const value_type temp(*iSorted);

                iNext = iCurrent = iSorted;

                // Note: The following loop has a problem: it can decrement iCurrent to before 'first'.
                // It doesn't dereference the iterator, but std STL disallows that operation. This isn't 
                // a problem for EASTL containers and ranges, as they support a single decrement of first,
                // but std STL iterators may have a problem with it. Dinkumware STL, for example, will assert.
                // To do: Fix this loop to not decrement like so.
                for(--iCurrent; (iNext != first) && compare(temp, *iCurrent); --iNext, --iCurrent)
                {
                    EASTL_VALIDATE_COMPARE(!compare(*iCurrent, temp)); // Validate that the compare function is sane.
                    *iNext = *iCurrent;
                }

                *iNext = temp;
            }
        }
    } // insertion_sort



    template <typename BidirectionalIterator>
    void insertion_sort(BidirectionalIterator first, BidirectionalIterator last)
    {
        typedef typename eastl::iterator_traits<BidirectionalIterator>::value_type value_type;

        if(first != last)
        {
            BidirectionalIterator iCurrent, iNext, iSorted = first;

            for(++iSorted; iSorted != last; ++iSorted)
            {
                const value_type temp(*iSorted);

                iNext = iCurrent = iSorted;

                // Note: The following loop has a problem: it can decrement iCurrent to before 'first'.
                // It doesn't dereference the iterator, but std STL disallows that operation. This isn't 
                // a problem for EASTL containers and ranges, as they support a single decrement of first,
                // but std STL iterators may have a problem with it. Dinkumware STL, for example, will assert.
                // To do: Fix this loop to not decrement like so.
                for(--iCurrent; (iNext != first) && (temp < *iCurrent); --iNext, --iCurrent)
                {
                    EASTL_VALIDATE_COMPARE(!(*iCurrent < temp)); // Validate that the compare function is sane.
                    *iNext = *iCurrent;
                }

                *iNext = temp;
            }
        }
    } // insertion_sort


    #if 0 /*
    // STLPort-like variation of insertion_sort. Doesn't seem to run quite as fast for small runs.
    //
    template <typename RandomAccessIterator, typename Compare>
    void insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare compare)
    {
        if(first != last)
        {
            for(RandomAccessIterator i = first + 1; i != last; ++i)
            {
                const typename eastl::iterator_traits<RandomAccessIterator>::value_type value(*i);

                if(compare(value, *first))
                {
                    EASTL_VALIDATE_COMPARE(!compare(*first, value)); // Validate that the compare function is sane.
                    eastl::copy_backward(first, i, i + 1);
                    *first = value;
                }
                else
                {
                    RandomAccessIterator end(i), prev(i);

                    for(--prev; compare(value, *prev); --end, --prev)
                    {
                        EASTL_VALIDATE_COMPARE(!compare(*prev, value)); // Validate that the compare function is sane.
                        *end = *prev;
                    }

                    *end = value;
                }
            }
        }
    }


    // STLPort-like variation of insertion_sort. Doesn't seem to run quite as fast for small runs.
    //
    template <typename RandomAccessIterator>
    void insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
    {
        if(first != last)
        {
            for(RandomAccessIterator i = first + 1; i != last; ++i)
            {
                const typename eastl::iterator_traits<RandomAccessIterator>::value_type value(*i);

                if(value < *first)
                {
                    EASTL_VALIDATE_COMPARE(!(*first < value)); // Validate that the compare function is sane.
                    eastl::copy_backward(first, i, i + 1);
                    *first = value;
                }
                else
                {
                    RandomAccessIterator end(i), prev(i);

                    for(--prev; value < *prev; --end, --prev)
                    {
                        EASTL_VALIDATE_COMPARE(!(*prev < value)); // Validate that the compare function is sane.
                        *end = *prev;
                    }

                    *end = value;
                }
            }
        }
    } */
    #endif


    /// shell_sort
    ///
    /// Implements the ShellSort algorithm. This algorithm is a serious algorithm for larger 
    /// data sets, as reported by Sedgewick in his discussions on QuickSort. Note that shell_sort
    /// requires a random access iterator, which usually means an array (eg. vector, deque).
    /// ShellSort has good performance with presorted sequences.
    /// The term "shell" derives from the name of the inventor, David Shell.
    ///
    /// To consider: Allow the user to specify the "h-sequence" array.
    ///
    template <typename RandomAccessIterator, typename StrictWeakOrdering>
    void shell_sort(RandomAccessIterator first, RandomAccessIterator last, StrictWeakOrdering compare)
    {
        typedef typename eastl::iterator_traits<RandomAccessIterator>::difference_type difference_type;

        // We use the Knuth 'h' sequence below, as it is easy to calculate at runtime. 
        // However, possibly we are better off using a different sequence based on a table.
        // One such sequence which averages slightly better than Knuth is:
        //    1, 5, 19, 41, 109, 209, 505, 929, 2161, 3905, 8929, 16001, 36289, 
        //    64769, 146305, 260609, 587521, 1045505, 2354689, 4188161, 9427969, 16764929

        if(first != last)
        {
            RandomAccessIterator iCurrent, iBack, iSorted, iInsertFirst;
            difference_type      nSize  = last - first;
            difference_type      nSpace = 1; // nSpace is the 'h' value of the ShellSort algorithm.

            while(nSpace < nSize)
                nSpace = (nSpace * 3) + 1; // This is the Knuth 'h' sequence: 1, 4, 13, 40, 121, 364, 1093, 3280, 9841, 29524, 88573, 265720, 797161, 2391484, 7174453, 21523360, 64570081, 193710244, 

            for(nSpace = (nSpace - 1) / 3; nSpace >= 1; nSpace = (nSpace - 1) / 3)  // Integer division is less than ideal.
            {
                for(difference_type i = 0; i < nSpace; i++)
                {
                    iInsertFirst = first + i;

                    for(iSorted = iInsertFirst + nSpace; iSorted < last; iSorted += nSpace)
                    {
                        iBack = iCurrent = iSorted;
                        
                        for(iBack -= nSpace; (iCurrent != iInsertFirst) && compare(*iCurrent, *iBack); iCurrent = iBack, iBack -= nSpace)
                        {
                            EASTL_VALIDATE_COMPARE(!compare(*iBack, *iCurrent)); // Validate that the compare function is sane.
                            eastl::iter_swap(iCurrent, iBack);
                        }
                    }
                }
            }
        }
    } // shell_sort

    template <typename RandomAccessIterator>
    inline void shell_sort(RandomAccessIterator first, RandomAccessIterator last)
    {
        typedef eastl::less<typename eastl::iterator_traits<RandomAccessIterator>::value_type> Less;

        eastl::shell_sort<RandomAccessIterator, Less>(first, last, Less());
    }



    /// heap_sort
    ///
    /// Implements the HeapSort algorithm. 
    /// Note that heap_sort requires a random access iterator, which usually means 
    /// an array (eg. vector, deque).
    ///
    template <typename RandomAccessIterator, typename StrictWeakOrdering>
    void heap_sort(RandomAccessIterator first, RandomAccessIterator last, StrictWeakOrdering compare)
    {
        // We simply call our heap algorithms to do the work for us.
        eastl::make_heap<RandomAccessIterator, StrictWeakOrdering>(first, last, compare);
        eastl::sort_heap<RandomAccessIterator, StrictWeakOrdering>(first, last, compare);
    }

    template <typename RandomAccessIterator>
    inline void heap_sort(RandomAccessIterator first, RandomAccessIterator last)
    {
        typedef eastl::less<typename eastl::iterator_traits<RandomAccessIterator>::value_type> Less;

        eastl::heap_sort<RandomAccessIterator, Less>(first, last, Less());
    }




    /// merge_sort_buffer
    ///
    /// Implements the MergeSort algorithm with a user-supplied buffer.
    /// The input buffer must be able to hold a number of items equal to 'last - first'.
    /// Note that merge_sort_buffer requires a random access iterator, which usually means 
    /// an array (eg. vector, deque).
    ///
    
    // For reference, the following is the simple version, before inlining one level 
    // of recursion and eliminating the copy:
    //
    //template <typename RandomAccessIterator, typename T, typename StrictWeakOrdering>
    //void merge_sort_buffer(RandomAccessIterator first, RandomAccessIterator last, T* pBuffer, StrictWeakOrdering compare)
    //{
    //    typedef typename eastl::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    //
    //    const difference_type nCount = last - first;
    //
    //    if(nCount > 1)
    //    {
    //        const difference_type nMid = nCount / 2;
    //
    //        eastl::merge_sort_buffer<RandomAccessIterator, T, StrictWeakOrdering>
    //                                (first,        first + nMid, pBuffer, compare);
    //        eastl::merge_sort_buffer<RandomAccessIterator, T, StrictWeakOrdering>
    //                                (first + nMid, last        , pBuffer, compare);
    //        eastl::copy(first, last, pBuffer);
    //        eastl::merge<T*, T*, RandomAccessIterator, StrictWeakOrdering>
    //                    (pBuffer, pBuffer + nMid, pBuffer + nMid, pBuffer + nCount, first, compare);
    //    }
    //}
    
    template <typename RandomAccessIterator, typename T, typename StrictWeakOrdering>
    void merge_sort_buffer(RandomAccessIterator first, RandomAccessIterator last, T* pBuffer, StrictWeakOrdering compare)
    {
        typedef typename eastl::iterator_traits<RandomAccessIterator>::difference_type difference_type;
        const difference_type nCount = last - first;

        if(nCount > 1)
        {
            const difference_type nMid = nCount / 2;
            RandomAccessIterator  half = first + nMid;
 
            if(nMid > 1)
            {
                const difference_type nQ1(nMid / 2);
                RandomAccessIterator  part(first + nQ1);

                eastl::merge_sort_buffer<RandomAccessIterator, T, StrictWeakOrdering>(first, part, pBuffer,       compare);
                eastl::merge_sort_buffer<RandomAccessIterator, T, StrictWeakOrdering>(part,  half, pBuffer + nQ1, compare);
                eastl::merge<RandomAccessIterator, RandomAccessIterator, T*, StrictWeakOrdering>
                            (first, part, part, half, pBuffer, compare);
            }
            else
                *pBuffer = *first;
 
            if((nCount - nMid) > 1)
            {
                const difference_type nQ3((difference_type)(((size_t)nMid + (size_t)nCount) >> 1));  // Equivalent to (nQ3 = (nMid + nCount) / 2) but handles the case of integer rollover.
                RandomAccessIterator  part(first + nQ3);

                eastl::merge_sort_buffer<RandomAccessIterator, T, StrictWeakOrdering>(half, part, pBuffer + nMid, compare);
                eastl::merge_sort_buffer<RandomAccessIterator, T, StrictWeakOrdering>(part, last, pBuffer + nQ3,  compare);
                eastl::merge<RandomAccessIterator, RandomAccessIterator, T*, StrictWeakOrdering>
                            (half, part, part, last, pBuffer + nMid, compare);
            }
            else
                *(pBuffer + nMid) = *half;
 
            eastl::merge<T*, T*, RandomAccessIterator, StrictWeakOrdering>
                        (pBuffer, pBuffer + nMid, pBuffer + nMid, pBuffer + nCount, first, compare);
        }
    }

    template <typename RandomAccessIterator, typename T>
    inline void merge_sort_buffer(RandomAccessIterator first, RandomAccessIterator last, T* pBuffer)
    {
        typedef eastl::less<typename eastl::iterator_traits<RandomAccessIterator>::value_type> Less;

        eastl::merge_sort_buffer<RandomAccessIterator, T, Less>(first, last, pBuffer, Less());
    }



    /// merge_sort
    ///
    /// Implements the MergeSort algorithm.
    /// This algorithm allocates memory via the user-supplied allocator. Use merge_sort_buffer
    /// function if you want a version which doesn't allocate memory.
    /// Note that merge_sort requires a random access iterator, which usually means 
    /// an array (eg. vector, deque).
    /// 
    template <typename RandomAccessIterator, typename Allocator, typename StrictWeakOrdering>
    void merge_sort(RandomAccessIterator first, RandomAccessIterator last, Allocator& allocator, StrictWeakOrdering compare)
    {
        typedef typename eastl::iterator_traits<RandomAccessIterator>::difference_type difference_type;
        typedef typename eastl::iterator_traits<RandomAccessIterator>::value_type      value_type;

        const difference_type nCount = last - first;

        if(nCount > 1)
        {
            // We need to allocate an array of nCount value_type objects as a temporary buffer.
            value_type* const pBuffer = (value_type*)allocate_memory(allocator, nCount * sizeof(value_type), EASTL_ALIGN_OF(value_type), 0);
            eastl::uninitialized_fill(pBuffer, pBuffer + nCount, value_type());

            eastl::merge_sort_buffer<RandomAccessIterator, value_type, StrictWeakOrdering>
                                    (first, last, pBuffer, compare);

            eastl::destruct(pBuffer, pBuffer + nCount);
            EASTLFree(allocator, pBuffer, nCount * sizeof(value_type));
        }
    }

    template <typename RandomAccessIterator, typename Allocator>
    inline void merge_sort(RandomAccessIterator first, RandomAccessIterator last, Allocator& allocator)
    {
        typedef eastl::less<typename eastl::iterator_traits<RandomAccessIterator>::value_type> Less;

        eastl::merge_sort<RandomAccessIterator, Allocator, Less>(first, last, allocator, Less());
    }



    /// partition
    ///
    /// Implements the partition algorithm.
    /// Rearranges the elements in the range [first, last), in such a way that all the elements 
    /// for which pred returns true precede all those for which it returns false. The iterator 
    /// returned points to the first element of the second group.
    /// The relative ordering within each group is not necessarily the same as before the call. 
    /// See function stable_partition for a function with a similar behavior and stability in 
    /// the ordering.
    /// 
    /// To do: Implement a version that uses a faster BidirectionalIterator algorithm for the 
    ///        case that the iterator range is a bidirectional iterator instead of just an
    ///        input iterator (one direction).
    ///
    template<typename InputIterator, typename Predicate>
    InputIterator partition(InputIterator begin, InputIterator end, Predicate predicate)
    {
        if(begin != end) 
        {
            while(predicate(*begin))
            {
                if(++begin == end) 
                    return begin;
            }

            InputIterator middle = begin;

            while(++middle != end)
            {
                if(predicate(*middle))
                {
                    eastl::swap(*begin, *middle);
                    ++begin;
                }
            }
        }

        return begin;
    }



    /////////////////////////////////////////////////////////////////////
    // quick_sort
    //
    // We do the "introspection sort" variant of quick sort which is now
    // well-known and understood. You can read about this algorithm in
    // many articles on quick sort, but briefly what it does is a median-
    // of-three quick sort whereby the recursion depth is limited to a
    // some value (after which it gives up on quick sort and switches to
    // a heap sort) and whereby after a certain amount of sorting the 
    // algorithm stops doing quick-sort and finishes the sorting via
    // a simple insertion sort.
    /////////////////////////////////////////////////////////////////////

    #if defined(CS_UNDEFINED_STRING) || defined(EA_PROCESSOR_X86_64)  && defined(_MSC_VER)
        static const int kQuickSortLimit = 28; // For sorts of random arrays over 100 items, 28 - 32 have been found to be good numbers on VC++/Win32.
    #else
        static const int kQuickSortLimit = 16; // It seems that on other processors lower limits are more beneficial, as they result in fewer compares.
    #endif

    namespace Internal
    {
        template <typename Size>
        inline Size Log2(Size n)
        {
            int i;
            for(i = 0; n; ++i)
                n >>= 1;
            return i - 1;
        }

        // To do: Investigate the speed of this bit-trick version of Log2.
        //        It may work better on some platforms but not others.
        //
        // union FloatUnion {
        //     float    f;
        //     uint32_t i;
        // };
        // 
        // inline uint32_t Log2(uint32_t x)
        // {
        //     const FloatInt32Union u = { x };
        //     return (u.i >> 23) - 127;
        // }
    }


    /// get_partition
    ///
    /// This function takes const T& instead of T because T may have special alignment
    /// requirements and some compilers (e.g. VC++) are don't respect alignment requirements
    /// for function arguments.
    ///
    template <typename RandomAccessIterator, typename T>
    inline RandomAccessIterator get_partition(RandomAccessIterator first, RandomAccessIterator last, const T& pivotValue)
    {
        const T pivotCopy(pivotValue); // Need to make a temporary because the sequence below is mutating.

        for(; ; ++first)
        {
            while(*first < pivotCopy)
            {
                EASTL_VALIDATE_COMPARE(!(pivotCopy < *first)); // Validate that the compare function is sane.
                ++first;
            }
            --last;

            while(pivotCopy < *last)
            {
                EASTL_VALIDATE_COMPARE(!(*last < pivotCopy)); // Validate that the compare function is sane.
                --last;
            }

            if(first >= last) // Random access iterators allow operator >=
                return first;

            eastl::iter_swap(first, last);
        }
    }


    template <typename RandomAccessIterator, typename T, typename Compare>
    inline RandomAccessIterator get_partition(RandomAccessIterator first, RandomAccessIterator last, const T& pivotValue, Compare compare)
    {
        const T pivotCopy(pivotValue); // Need to make a temporary because the sequence below is mutating.

        for(; ; ++first)
        {
            while(compare(*first, pivotCopy))
            {
                EASTL_VALIDATE_COMPARE(!compare(pivotCopy, *first)); // Validate that the compare function is sane.
                ++first;
            }
            --last;

            while(compare(pivotCopy, *last))
            {
                EASTL_VALIDATE_COMPARE(!compare(*last, pivotCopy)); // Validate that the compare function is sane.
                --last;
            }

            if(first >= last) // Random access iterators allow operator >=
                return first;

            eastl::iter_swap(first, last);
        }
    }


    namespace Internal
    {
        // This function is used by quick_sort and is not intended to be used by itself. 
        // This is because the implementation below makes an assumption about the input
        // data that quick_sort satisfies but arbitrary data may not.
        // There is a standalone insertion_sort function. 
        template <typename RandomAccessIterator>
        inline void insertion_sort_simple(RandomAccessIterator first, RandomAccessIterator last)
        {
            for(RandomAccessIterator current = first; current != last; ++current)
            {
                typedef typename eastl::iterator_traits<RandomAccessIterator>::value_type value_type;

                RandomAccessIterator end(current), prev(current);
                const value_type     value(*current);

                for(--prev; value < *prev; --end, --prev) // We skip checking for (prev >= first) because quick_sort (our caller) makes this unnecessary.
                {
                    EASTL_VALIDATE_COMPARE(!(*prev < value)); // Validate that the compare function is sane.
                    *end = *prev;
                }

                *end = value;
            }
        }


        // This function is used by quick_sort and is not intended to be used by itself. 
        // This is because the implementation below makes an assumption about the input
        // data that quick_sort satisfies but arbitrary data may not.
        // There is a standalone insertion_sort function. 
        template <typename RandomAccessIterator, typename Compare>
        inline void insertion_sort_simple(RandomAccessIterator first, RandomAccessIterator last, Compare compare)
        {
            for(RandomAccessIterator current = first; current != last; ++current)
            {
                typedef typename eastl::iterator_traits<RandomAccessIterator>::value_type value_type;

                RandomAccessIterator end(current), prev(current);
                const value_type     value(*current);

                for(--prev; compare(value, *prev); --end, --prev) // We skip checking for (prev >= first) because quick_sort (our caller) makes this unnecessary.
                {
                    EASTL_VALIDATE_COMPARE(!compare(*prev, value)); // Validate that the compare function is sane.
                    *end = *prev;
                }

                *end = value;
            }
        }
    } // namespace Internal


    template <typename RandomAccessIterator>
    inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last)
    {
        typedef typename eastl::iterator_traits<RandomAccessIterator>::difference_type difference_type;
        typedef typename eastl::iterator_traits<RandomAccessIterator>::value_type      value_type;

        eastl::make_heap<RandomAccessIterator>(first, middle);

        for(RandomAccessIterator i = middle; i < last; ++i)
        {
            if(*i < *first)
            {
                EASTL_VALIDATE_COMPARE(!(*first < *i)); // Validate that the compare function is sane.
                const value_type temp(*i);
                *i = *first;
                eastl::adjust_heap<RandomAccessIterator, difference_type, value_type>
                                  (first, difference_type(0), difference_type(middle - first), difference_type(0), temp);
            }
        }

        eastl::sort_heap<RandomAccessIterator>(first, middle);
    }


    template <typename RandomAccessIterator, typename Compare>
    inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare compare)
    {
        typedef typename eastl::iterator_traits<RandomAccessIterator>::difference_type difference_type;
        typedef typename eastl::iterator_traits<RandomAccessIterator>::value_type      value_type;

        eastl::make_heap<RandomAccessIterator, Compare>(first, middle, compare);

        for(RandomAccessIterator i = middle; i < last; ++i)
        {
            if(compare(*i, *first))
            {
                EASTL_VALIDATE_COMPARE(!compare(*first, *i)); // Validate that the compare function is sane.
                const value_type temp(*i);
                *i = *first;
                eastl::adjust_heap<RandomAccessIterator, difference_type, value_type, Compare>
                                  (first, difference_type(0), difference_type(middle - first), difference_type(0), temp, compare);
            }
        }

        eastl::sort_heap<RandomAccessIterator, Compare>(first, middle, compare);
    }


    template<typename RandomAccessIterator>
    inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

        while((last - first) > 5)
        {
            const value_type           midValue(eastl::median<value_type>(*first, *(first + (last - first) / 2), *(last - 1)));
            const RandomAccessIterator midPos(eastl::get_partition<RandomAccessIterator, value_type>(first, last, midValue));

            if(midPos <= nth)
                first = midPos;
            else
                last = midPos;
        }

        eastl::insertion_sort<RandomAccessIterator>(first, last);
    }


    template<typename RandomAccessIterator, typename Compare>
    inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, Compare compare)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

        while((last - first) > 5)
        {
            const value_type           midValue(eastl::median<value_type, Compare>(*first, *(first + (last - first) / 2), *(last - 1), compare));
            const RandomAccessIterator midPos(eastl::get_partition<RandomAccessIterator, value_type, Compare>(first, last, midValue, compare));

            if(midPos <= nth)
                first = midPos;
            else
                last = midPos;
        }

        eastl::insertion_sort<RandomAccessIterator, Compare>(first, last, compare);
    }


    namespace Internal
    {
        template <typename RandomAccessIterator, typename Size>
        inline void quick_sort_impl(RandomAccessIterator first, RandomAccessIterator last, Size kRecursionCount)
        {
            typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

            while(((last - first) > kQuickSortLimit) && (kRecursionCount > 0))
            {
                const RandomAccessIterator position(eastl::get_partition<RandomAccessIterator, value_type>(first, last, eastl::median<value_type>(*first, *(first + (last - first) / 2), *(last - 1))));

                eastl::Internal::quick_sort_impl<RandomAccessIterator, Size>(position, last, --kRecursionCount);
                last = position;
            }

            if(kRecursionCount == 0)
                eastl::partial_sort<RandomAccessIterator>(first, last, last);
        }


        template <typename RandomAccessIterator, typename Size, typename Compare>
        inline void quick_sort_impl(RandomAccessIterator first, RandomAccessIterator last, Size kRecursionCount, Compare compare)
        {
            typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

            while(((last - first) > kQuickSortLimit) && (kRecursionCount > 0))
            {
                const RandomAccessIterator position(eastl::get_partition<RandomAccessIterator, value_type, Compare>(first, last, eastl::median<value_type, Compare>(*first, *(first + (last - first) / 2), *(last - 1), compare), compare));

                eastl::Internal::quick_sort_impl<RandomAccessIterator, Size, Compare>(position, last, --kRecursionCount, compare);
                last = position;
            }

            if(kRecursionCount == 0)
                eastl::partial_sort<RandomAccessIterator, Compare>(first, last, last, compare);
        }
    }


    /// quick_sort
    ///
    /// quick_sort sorts the elements in [first, last) into ascending order, 
    /// meaning that if i and j are any two valid iterators in [first, last) 
    /// such that i precedes j, then *j is not less than *i. quick_sort is not 
    /// guaranteed to be stable. That is, suppose that *i and *j are equivalent: 
    /// neither one is less than the other. It is not guaranteed that the 
    /// relative order of these two elements will be preserved by sort.
    ///
    /// We implement the "introspective" variation of quick-sort. This is 
    /// considered to be the best general-purpose variant, as it avoids 
    /// worst-case behaviour and optimizes the final sorting stage by 
    /// switching to an insertion sort.
    ///
    template <typename RandomAccessIterator>
    void quick_sort(RandomAccessIterator first, RandomAccessIterator last)
    {
        typedef typename eastl::iterator_traits<RandomAccessIterator>::difference_type difference_type;

        if(first != last)
        {
            eastl::Internal::quick_sort_impl<RandomAccessIterator, difference_type>(first, last, 2 * Internal::Log2(last - first));

            if((last - first) > (difference_type)kQuickSortLimit)
            {
                eastl::insertion_sort<RandomAccessIterator>(first, first + kQuickSortLimit);
                eastl::Internal::insertion_sort_simple<RandomAccessIterator>(first + kQuickSortLimit, last);
            }
            else
                eastl::insertion_sort<RandomAccessIterator>(first, last);
        }
    }


    template <typename RandomAccessIterator, typename Compare>
    void quick_sort(RandomAccessIterator first, RandomAccessIterator last, Compare compare)
    {
        typedef typename eastl::iterator_traits<RandomAccessIterator>::difference_type difference_type;

        if(first != last)
        {
            eastl::Internal::quick_sort_impl<RandomAccessIterator, difference_type, Compare>(first, last, 2 * Internal::Log2(last - first), compare);

            if((last - first) > (difference_type)kQuickSortLimit)
            {
                eastl::insertion_sort<RandomAccessIterator, Compare>(first, first + kQuickSortLimit, compare);
                eastl::Internal::insertion_sort_simple<RandomAccessIterator, Compare>(first + kQuickSortLimit, last, compare);
            }
            else
                eastl::insertion_sort<RandomAccessIterator, Compare>(first, last, compare);
        }
    }



    /// sort
    /// 
    /// We simply use quick_sort. See quick_sort for details.
    ///
    template <typename RandomAccessIterator>
    inline void sort(RandomAccessIterator first, RandomAccessIterator last)
    {
        eastl::quick_sort<RandomAccessIterator>(first, last);
    }

    template <typename RandomAccessIterator, typename Compare>
    inline void sort(RandomAccessIterator first, RandomAccessIterator last, Compare compare)
    {
        eastl::quick_sort<RandomAccessIterator, Compare>(first, last, compare);
    }



    /// stable_sort
    /// 
    /// We simply use merge_sort. See merge_sort for details.
    /// Beware that the used merge_sort -- and thus stable_sort -- allocates 
    /// memory during execution. Try using merge_sort_buffer if you want
    /// to avoid memory allocation.
    /// 
    template <typename RandomAccessIterator, typename StrictWeakOrdering>
    void stable_sort(RandomAccessIterator first, RandomAccessIterator last, StrictWeakOrdering compare)
    {
        eastl::merge_sort<RandomAccessIterator, EASTLAllocatorType, StrictWeakOrdering>
                         (first, last, *get_default_allocator(0), compare);
    }

    template <typename RandomAccessIterator>
    void stable_sort(RandomAccessIterator first, RandomAccessIterator last)
    {
        eastl::merge_sort<RandomAccessIterator, EASTLAllocatorType>
                         (first, last, *get_default_allocator(0));
    }

    template <typename RandomAccessIterator, typename Allocator, typename StrictWeakOrdering>
    void stable_sort(RandomAccessIterator first, RandomAccessIterator last, Allocator& allocator, StrictWeakOrdering compare)
    {
        eastl::merge_sort<RandomAccessIterator, Allocator, StrictWeakOrdering>(first, last, allocator, compare);
    }

    // This is not defined because it would cause compiler errors due to conflicts with a version above. 
    //template <typename RandomAccessIterator, typename Allocator>
    //void stable_sort(RandomAccessIterator first, RandomAccessIterator last, Allocator& allocator)
    //{
    //    eastl::merge_sort<RandomAccessIterator, Allocator>(first, last, allocator);
    //}




    namespace Internal
    {
        // Portions of the tim_sort code were originally written by Christopher Swenson.
        // https://github.com/swenson/sort
        // All code in this repository, unless otherwise specified, is hereby licensed under the 
        // MIT Public License: Copyright (c) 2010 Christopher Swenson

        const intptr_t kTimSortStackSize = 64; // Question: What's the upper-limit size requirement for this?

        struct tim_sort_run
        {
            intptr_t start;
            intptr_t length;
        };


        // EASTL_COUNT_LEADING_ZEROES
        //
        // Count leading zeroes in an integer.
        //
        #ifndef EASTL_COUNT_LEADING_ZEROES
            #if   defined(__GNUC__)
                #if (EA_PLATFORM_PTR_SIZE == 8)
                    #define EASTL_COUNT_LEADING_ZEROES __builtin_clzll
                #else
                    #define EASTL_COUNT_LEADING_ZEROES __builtin_clz
                #endif
            #endif

            #ifndef EASTL_COUNT_LEADING_ZEROES
                static inline int eastl_count_leading_zeroes(uint64_t x)
                {
                    if(x)
                    {
                        int n = 0;
                        if(x & UINT64_C(0xFFFFFFFF00000000)) { n += 32; x >>= 32; }
                        if(x & 0xFFFF0000)                   { n += 16; x >>= 16; }
                        if(x & 0xFFFFFF00)                   { n +=  8; x >>=  8; }
                        if(x & 0xFFFFFFF0)                   { n +=  4; x >>=  4; }
                        if(x & 0xFFFFFFFC)                   { n +=  2; x >>=  2; }
                        if(x & 0xFFFFFFFE)                   { n +=  1;           }
                        return 63 - n;
                    }
                    return 64;
                }

                static inline int eastl_count_leading_zeroes(uint32_t x)
                {
                    if(x)
                    {
                        int n = 0;
                        if(x <= 0x0000FFFF) { n += 16; x <<= 16; }
                        if(x <= 0x00FFFFFF) { n +=  8; x <<=  8; }
                        if(x <= 0x0FFFFFFF) { n +=  4; x <<=  4; }
                        if(x <= 0x3FFFFFFF) { n +=  2; x <<=  2; }
                        if(x <= 0x7FFFFFFF) { n +=  1;           }
                        return n;
                    }
                    return 32;
                }

                #define EASTL_COUNT_LEADING_ZEROES eastl_count_leading_zeroes
            #endif
        #endif


        // Sorts a range whose initial (start - first) entries are already sorted.
        // This function is a useful helper to the tim_sort function.
        // This is the same as insertion_sort except that it has a start parameter which indicates
        // where the start of the unsorted data is.
        template <typename BidirectionalIterator, typename StrictWeakOrdering>
        void insertion_sort_already_started(BidirectionalIterator first, BidirectionalIterator last, BidirectionalIterator start, StrictWeakOrdering compare)
        {
            typedef typename eastl::iterator_traits<BidirectionalIterator>::value_type value_type;

            if(first != last) // if the range is non-empty...
            {
                BidirectionalIterator iCurrent, iNext, iSorted = start - 1;

                for(++iSorted; iSorted != last; ++iSorted)
                {
                    const value_type temp(*iSorted);

                    iNext = iCurrent = iSorted;

                    for(--iCurrent; (iNext != first) && compare(temp, *iCurrent); --iNext, --iCurrent)
                    {
                        EASTL_VALIDATE_COMPARE(!compare(*iCurrent, temp)); // Validate that the compare function is sane.
                        *iNext = *iCurrent;
                    }

                    *iNext = temp;
                }
            }
        }


        // reverse_elements
        //
        // Reverses the range [first + start, first + start + size)
        // To consider: Use void eastl::reverse(BidirectionalIterator first, BidirectionalIterator last);
        //
        template <typename RandomAccessIterator>
        void reverse_elements(RandomAccessIterator first, intptr_t start, intptr_t end)
        {
            while(start < end)
            {
                eastl::swap(*(first + start), *(first + end));
                ++start;
                --end;
            }
        }


        // tim_sort_count_run
        //
        // Finds the length of a run which is already sorted (either up or down).
        // If the run is in reverse order, this function puts it in regular order.
        //
        template <typename RandomAccessIterator, typename StrictWeakOrdering>
        intptr_t tim_sort_count_run(const RandomAccessIterator first, const intptr_t start, const intptr_t size, StrictWeakOrdering compare)
        {
            if((size - start) > 1) // If there is anything in the set...
            {
                intptr_t curr = (start + 2);
                
                if(!compare(*(first + start + 1), *(first + start))) // If (first[start + 1] >= first[start]) (If the run is increasing) ...
                {
                    for(;; ++curr)
                    {
                        if(curr == (size - 1)) // If we are at the end of the data... this run is done.
                            break;

                        if(compare(*(first + curr), *(first + curr - 1))) // If this item is not in order... this run is done.
                            break;
                    }
                }
                else  // Else it is decreasing.
                {
                    for(;; ++curr)
                    {
                        if(curr == (size - 1))  // If we are at the end of the data... this run is done.
                            break;

                        if(!compare(*(first + curr), *(first + curr - 1)))  // If this item is not in order... this run is done.
                            break;                                          // Note that we intentionally compare against <= 0 and not just < 0. This is because 
                    }                                                       // The reverse_elements call below could reverse two equal elements and break our stability requirement.

                    reverse_elements(first, start, curr - 1);
                }

                return (curr - start);
            }

            // Else we have just one item in the set.       
            return 1;
        }


        // Input   Return
        // --------------
        //  64      32
        //  65      33
        //  66      33
        //  67      34
        //  68      34
        // ...
        // 125      63
        // 126      63
        // 127      64
        // 128      32
        // 129      33
        // 130      33
        // 131      33
        // 132      33
        // 133      34
        // 134      34
        // 135      34
        // 136      34
        // 137      35
        // ...
        //
        // This function will return a value that is always in the range of [32, 64].
        //
        static inline intptr_t timsort_compute_minrun(intptr_t size)
        {
            const int32_t  top_bit = (int32_t)((sizeof(intptr_t) * 8) - EASTL_COUNT_LEADING_ZEROES((uintptr_t)size));
            const int32_t  shift   = (top_bit > 6) ? (top_bit - 6) : 0;
            const intptr_t mask    = (intptr_t(1) << shift) - 1;
                  intptr_t minrun  = (intptr_t)(size >> shift);

            if(mask & size)
                ++minrun;

            return minrun;
        }


        template <typename RandomAccessIterator, typename T, typename StrictWeakOrdering>
        void tim_sort_merge(RandomAccessIterator first, const tim_sort_run* run_stack, const intptr_t stack_curr, 
                            T* pBuffer, StrictWeakOrdering compare)
        {
            const intptr_t A    = run_stack[stack_curr - 2].length;
            const intptr_t B    = run_stack[stack_curr - 1].length;
            const intptr_t curr = run_stack[stack_curr - 2].start;

            EASTL_DEV_ASSERT((A < 10000000) && (B < 10000000) && (curr < 10000000)); // Sanity check.

            if(A < B) // If the first run is shorter than the second run... merge left.
            {
                // Copy to another location so we have room in the main array to put the sorted items.
                eastl::copy(first + curr, first + curr + A, pBuffer);

                #if EASTL_DEV_DEBUG
                    typedef typename eastl::iterator_traits<RandomAccessIterator>::value_type value_type;

                    for(intptr_t i = 0; i < A; i++)
                        *(first + curr + i) = value_type();
                #endif

                intptr_t i = 0;
                intptr_t j = curr + A;
                
                for(intptr_t k = curr; k < curr + A + B; k++)
                {
                    if((i < A) && (j < (curr + A + B)))
                    {
                        if(!compare(*(first + j), *(pBuffer + i))) // If (first[j] >= pBuffer[i])...
                            *(first + k) = *(pBuffer + i++);
                        else
                            *(first + k) = *(first + j++);
                    }
                    else if(i < A)
                        *(first + k) = *(pBuffer + i++);
                    else
                        *(first + k) = *(first + j++);
                }
            }
            else // Else the second run is equal or shorter... merge right.
            {
                eastl::copy(first + curr + A, first + curr + A + B, pBuffer);

                intptr_t i = B - 1;
                intptr_t j = curr + A - 1;
                
                for(intptr_t k = curr + A + B - 1; k >= curr; k--)
                {
                    if((i >= 0) && (j >= curr))
                    {
                        if(compare(*(pBuffer + i), *(first + j))) // If (pBuffer[i] < first[j]) ...
                            *(first + k) = *(first + j--);
                        else
                            *(first + k) = *(pBuffer + i--);
                    }
                    else if(i >= 0)
                        *(first + k) = *(pBuffer + i--);
                    else
                        *(first + k) = *(first + j--);
                }
            }
        }


        // See the timsort.txt file for an explanation of this function.
        //
        // ------------------------------------------------------------------------
        // What turned out to be a good compromise maintains two invariants on the
        // stack entries, where A, B and C are the lengths of the three righmost 
        // not-yet merged slices:
        //    1.  A > B+C
        //    2.  B > C
        // ------------------------------------------------------------------------
        //
        static inline bool timsort_check_invariant(tim_sort_run* run_stack, const intptr_t stack_curr)
        {
            // To do: Optimize this for the most common type of values.
            if(stack_curr > 2)
            {
                const intptr_t A = run_stack[stack_curr - 3].length;
                const intptr_t B = run_stack[stack_curr - 2].length;
                const intptr_t C = run_stack[stack_curr - 1].length;

                EASTL_DEV_ASSERT((A < 10000000) && (B < 10000000) && (C < 10000000)); // Sanity check.

                if((A <= (B + C)) || (B <= C))
                    return true; // Merge the right-most runs.
            }
            else if(stack_curr == 2)
            {
                const intptr_t A = run_stack[stack_curr - 2].length;
                const intptr_t B = run_stack[stack_curr - 1].length;

                EASTL_DEV_ASSERT((A < 10000000) && (B < 10000000)); // Sanity check.

                if(A <= B)
                    return true; // Merge the right-most runs.
            }

            return false; // Don't merge the right-most runs.
        }


        template <typename RandomAccessIterator, typename T, typename StrictWeakOrdering>
        intptr_t tim_sort_collapse(RandomAccessIterator first, tim_sort_run* run_stack, intptr_t stack_curr, 
                                   T* pBuffer, const intptr_t size, StrictWeakOrdering compare)
        {
            // If the run_stack only has one thing on it, we are done with the collapse.
            while(stack_curr > 1)
            {
                // If this is the last merge, just do it.
                if((stack_curr == 2) && ((run_stack[0].length + run_stack[1].length) == size))
                {
                    tim_sort_merge(first, run_stack, stack_curr, pBuffer, compare);
                    run_stack[0].length += run_stack[1].length;
                    stack_curr--;

                    #if EASTL_DEV_DEBUG
                        memset(&run_stack[stack_curr], 0, sizeof(run_stack[stack_curr]));
                    #endif

                    break;
                }
                // Check if the invariant is off for a run_stack of 2 elements.
                else if((stack_curr == 2) && (run_stack[0].length <= run_stack[1].length))
                {
                    tim_sort_merge(first, run_stack, stack_curr, pBuffer, compare);
                    run_stack[0].length += run_stack[1].length;
                    stack_curr--;

                    #if EASTL_DEV_DEBUG
                        memset(&run_stack[stack_curr], 0, sizeof(run_stack[stack_curr]));
                    #endif

                    break;
                }
                else if (stack_curr == 2)
                    break;

                const intptr_t A = run_stack[stack_curr - 3].length;
                const intptr_t B = run_stack[stack_curr - 2].length;
                const intptr_t C = run_stack[stack_curr - 1].length;
                
                if(A <= (B + C)) // Check first invariant.
                {
                    if(A < C)
                    {
                        tim_sort_merge(first, run_stack, stack_curr - 1, pBuffer, compare);

                        stack_curr--;
                        run_stack[stack_curr - 2].length += run_stack[stack_curr - 1].length;   // Merge A and B.
                        run_stack[stack_curr - 1] = run_stack[stack_curr];

                        #if EASTL_DEV_DEBUG
                            EASTL_DEV_ASSERT((run_stack[stack_curr - 2].start + run_stack[stack_curr - 2].length) <= size);
                            EASTL_DEV_ASSERT((run_stack[stack_curr - 1].start + run_stack[stack_curr - 1].length) <= size);
                            memset(&run_stack[stack_curr], 0, sizeof(run_stack[stack_curr]));
                        #endif
                    }
                    else
                    {
                        tim_sort_merge(first, run_stack, stack_curr, pBuffer, compare);                  // Merge B and C.

                        stack_curr--;
                        run_stack[stack_curr - 1].length += run_stack[stack_curr].length;

                        #if EASTL_DEV_DEBUG
                            EASTL_DEV_ASSERT((run_stack[stack_curr - 1].start + run_stack[stack_curr - 1].length) <= size);
                            memset(&run_stack[stack_curr], 0, sizeof(run_stack[stack_curr]));
                        #endif
                    }
                }
                else if(B <= C) // Check second invariant
                {
                    tim_sort_merge(first, run_stack, stack_curr, pBuffer, compare);

                    stack_curr--;
                    run_stack[stack_curr - 1].length += run_stack[stack_curr].length;       // Merge B and C.

                    #if EASTL_DEV_DEBUG
                        EASTL_DEV_ASSERT((run_stack[stack_curr - 1].start + run_stack[stack_curr - 1].length) <= size);
                        memset(&run_stack[stack_curr], 0, sizeof(run_stack[stack_curr]));
                    #endif
                }
                else
                    break;
            }

            return stack_curr;
        }


        // tim_sort_add_run
        //
        // Return true if the sort is done.
        //
        template <typename RandomAccessIterator, typename T, typename StrictWeakOrdering>
        bool tim_sort_add_run(tim_sort_run* run_stack, RandomAccessIterator first, T* pBuffer, const intptr_t size, const intptr_t minrun, 
                              intptr_t& len, intptr_t& run, intptr_t& curr, intptr_t& stack_curr, StrictWeakOrdering compare)
        {
            len = tim_sort_count_run(first, curr, size, compare); // This will count the length of the run and reverse the run if it is backwards.
            run = minrun;

            if(run < minrun)            // Always make runs be of minrun length (we'll sort the additional data as needed below)
               run = minrun;

            if(run > (size - curr))     // But if there isn't minrun data remaining, just sort what's remaining.
               run = (size - curr);

            if(run > len)               // If there is any additional data we want to sort to bring up the run length to minrun.
            {
                insertion_sort_already_started(first + curr, first + curr + run, first + curr + len, compare);
                len = run;
            }

            // At this point, run will be equal to minrun or will go to the end of our data.
            // Add this run to our stack of runs.
            EASTL_DEV_ASSERT(stack_curr < kTimSortStackSize);
            EASTL_DEV_ASSERT((curr >= 0) && (curr < size) && ((curr + len) <= size));

            run_stack[stack_curr].start  = curr;
            run_stack[stack_curr].length = len;
            stack_curr++;

            // Move to the beginning of the next run in the data.
            curr += len;

            if(curr == size)    // If we have hit the end of the data...
            {
                while(stack_curr > 1) // If there is any more than one run... (else all the data is sorted)
                {
                    tim_sort_merge(first, run_stack, stack_curr, pBuffer, compare);

                    run_stack[stack_curr - 2].length += run_stack[stack_curr - 1].length;
                    stack_curr--;

                    #if EASTL_DEV_DEBUG
                        EASTL_DEV_ASSERT((run_stack[stack_curr - 1].start + run_stack[stack_curr - 1].length) <= size);
                        memset(&run_stack[stack_curr], 0, sizeof(run_stack[stack_curr]));
                    #endif
                }

                return true; // We are done with sorting.
            }

            return false;
        }

    } // namespace Internal


    // tim_sort_buffer
    //
    // Implements the tim-sort sorting algorithm with a user-provided scratch buffer.
    // http://en.wikipedia.org/wiki/Timsort
    // This sort is the fastest sort when sort stability (maintaining order of equal values) is required and
    // data sets are non-trivial (size >= 15). It's also the fastest sort (e.g. faster than quick_sort) for 
    // the case that at at least half your data is already sorted. Otherwise, eastl::quick_sort is about 10% 
    // faster than tim_sort_buffer but is not a stable sort. There are some reports that tim_sort outperforms
    // quick_sort but most of these aren't taking into account that optimal quick_sort implementations use
    // a hybrid approach called "introsort" (http://en.wikipedia.org/wiki/Introsort) which improves quick_sort
    // considerably in practice.
    //
    // Strengths:
    //     - Fastest stable sort for most sizes of data.
    //     - Fastest sort for containers of data already mostly sorted.
    //     - Simpler to understand than quick_sort.
    //
    // Weaknesses:
    //     - User must provide a scratch buffer, otherwise the buffer is dynamically allocated during runtime.
    //     - Not as fast as quick_sort for the general case of randomized data.
    //     - Requires a RandomAccessIterator; thus must be on an array container type and not a list container type.
    //     - Uses a lot of code to implement; thus it's not great when there is little room for more code.
    //
    // The pBuffer parameter must hold at least ((last-first)/2) elements (i.e. half the elements of the container).
    // This minimum size is a worst-case size requirement, but handles all possible cases. pBuffer is just a scratch
    // buffer and is not needed after the return of this function, and doesn't need to be seeded with any particular
    // values upon entering this function.
    //
    // Example usage:
    //     int intArray[64];
    //     int buffer[32];
    //     ...
    //     tim_sort_buffer(intArray, intArray + 64, buffer);
    //
    template <typename RandomAccessIterator, typename T, typename StrictWeakOrdering>
    void tim_sort_buffer(RandomAccessIterator first, RandomAccessIterator last, T* pBuffer, StrictWeakOrdering compare)
    {
        using namespace Internal;

        typedef typename eastl::iterator_traits<RandomAccessIterator>::value_type value_type;

        // To consider: Convert the implementation to use first/last instead of first/size.
        const intptr_t size = (intptr_t)(last - first);

        if(size < 64)
            insertion_sort_already_started(first, first + size, first + 1, compare);
        else
        {
            tim_sort_run   run_stack[kTimSortStackSize];
            intptr_t       stack_curr = 0;
            intptr_t       len, run;
            intptr_t       curr = 0;
            const intptr_t minrun = timsort_compute_minrun(size);

            #if EASTL_DEV_DEBUG
                memset(run_stack, 0, sizeof(run_stack));
            #endif

            if(tim_sort_add_run(run_stack, first, pBuffer, size, minrun, len, run, curr, stack_curr, compare))
                return;
            if(tim_sort_add_run(run_stack, first, pBuffer, size, minrun, len, run, curr, stack_curr, compare))
                return;
            if(tim_sort_add_run(run_stack, first, pBuffer, size, minrun, len, run, curr, stack_curr, compare))
                return;

            for(;;)
            {
                if(timsort_check_invariant(run_stack, stack_curr))
                    stack_curr = tim_sort_collapse(first, run_stack, stack_curr, pBuffer, size, compare);
                else
                {
                    if(tim_sort_add_run(run_stack, first, pBuffer, size, minrun, len, run, curr, stack_curr, compare))
                        break;
                }
            }
        }
    }


    template <typename RandomAccessIterator, typename T>
    inline void tim_sort_buffer(RandomAccessIterator first, RandomAccessIterator last, T* pBuffer)
    {
        typedef eastl::less<T> Less;

        eastl::tim_sort_buffer<RandomAccessIterator, T, Less>(first, last, pBuffer, Less());
    }




    /* 
    // Something to consider adding: An eastl sort which uses qsort underneath. 
    // The primary purpose of this is to have an eastl interface for sorting which
    // results in very little code generation, since all instances map to the 
    // C qsort function.

    template <typename T>
    int small_footprint_sort_func(const void* a, const void* b)
    {
        if(*(const T*)a < *(const T*)b)
            return -1;
        if(*(const T*)a > *(const T*)b)
            return +1;
        return 0;
    }

    template <typename ContiguousIterator>
    void small_footprint_sort(ContiguousIterator first, ContiguousIterator last)
    {
        typedef typename eastl::iterator_traits<ContiguousIterator>::value_type value_type;

        qsort(first, (size_t)eastl::distance(first, last), sizeof(value_type), small_footprint_sort_func<value_type>);
    }
    */

} // namespace eastl


#endif // Header include guard



