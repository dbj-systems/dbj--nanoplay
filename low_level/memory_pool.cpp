#pragma once
/*
Started from:
-------------
Fast Efficient Fixed-Size Memory Pool
No Loops and No Overhead
Ben Kenwright
School of Computer Science
Newcastle University
Newcastle, United Kingdom,
b.kenwright@ncl.ac.uk
*/

#include <crtdbg.h>
#include <stdint.h>
#include <malloc.h>

#include "../common.h"

// https://vorpus.org/blog/why-does-calloc-exist/
#define DBJ_ALLOC(T_,N_,S_) (T_*)calloc(N_,S_)
#define DBJ_FREE(P_) do { if(P_ != nullptr) free(P_); P_ = nullptr; } while(0)

///
/// obviously this deals with memory blocks only
/// aka void *
///
struct  memory_pool final
{ 
    // Basic type defines ... but why?
    // typedef unsigned int uint;
    // typedef unsigned char uchar;

    using uint  = uint16_t;
    using uchar = uint8_t ;
    using address_type = uchar;
    using index_type = uint;

    // nested strong types
    struct block_size  final { size_t val{}; };
    struct block_count final { uint val{}; };

    const block_count numOfBlocks{}; // Num of blocks
    const block_size  sizeOfEachBlock{}; // Size of each block
    uint numFreeBlocks{}; // Num of remaining blocks
    uint numInitialized{}; // Num of initialized blocks
    uchar* memStart{}; // Beginning of memory pool
    uchar* next{}; // Num of next free block

    /// no default ctor
    memory_pool() = delete ;

    ~memory_pool() {
            DBJ_FREE( memStart );
    }

     memory_pool
     (block_size const & sizeOfEachBlock_, block_count const & numOfBlocks_)
         noexcept
         : 
         numOfBlocks(numOfBlocks_) ,
         sizeOfEachBlock(sizeOfEachBlock_),
         numFreeBlocks(numOfBlocks_.val)
    {
        memStart = DBJ_ALLOC( uchar, numOfBlocks_.val, sizeOfEachBlock_.val);
        _ASSERTE(memStart);
        next = memStart;
    }

     // no copy definitely
     memory_pool(memory_pool const&) = delete;
     memory_pool & operator = (memory_pool const&) = delete;
     // no move maybe
     memory_pool(memory_pool &&) = delete;
     memory_pool& operator = (memory_pool &&) = delete;

     /// --------------------------------------------------------------------
     address_type * addr_from_index(index_type i) const
    {
        return memStart + (i * sizeOfEachBlock.val);
    }

    index_type index_from_addr(const address_type* p) const
    {
        return (((uint)(p - memStart)) / sizeOfEachBlock.val);
    }

    /// return void * .. this is typeless
    void* allocate()
    {
        if (numInitialized < numOfBlocks.val)
        {
            uint* p = (uint*)addr_from_index(numInitialized);
            *p = numInitialized + 1;
            numInitialized++;
        }
        void* ret = nullptr;
        if (numFreeBlocks > 0)
        {
            ret = (void*)next;
            --numFreeBlocks;
            if (numFreeBlocks != 0)
            {
                next = addr_from_index(*((uint*)next));
            }
            else
            {
                next = nullptr;
            }
        }
        return ret;
    }
    
    /// in DEBUG mode we assert on trying to free the nullptr
    /// in RELEASE mode we just return

    void deallocate(void* p)
    {
#ifdef _DEBUG
        _ASSERTE(p);
#else  // release
        if (!p) return;
#endif

        if (next != nullptr)
        {
            (*(uint*)p) = index_from_addr(next);
            next = (uchar*)p;
        }
        else
        {
            *((uint*)p) = numOfBlocks.val;
            next = (uchar*)p;
        }

        p = nullptr;

        ++numFreeBlocks;
    }
}; // memory_pool


TU_REGISTER ([]
 {
            memory_pool mem_pool_(
                memory_pool::block_size{ 128 },
                memory_pool::block_count{ 64 }
            );

            /// reminder: the mem pool deals with void * only
            memory_pool::address_type* cp_ =
                (memory_pool::address_type*)mem_pool_.allocate();

            memory_pool::index_type idx_ =
                mem_pool_.index_from_addr(cp_);

            memory_pool::address_type* cp_2_ =
                mem_pool_.addr_from_index(idx_);

            mem_pool_.deallocate(cp_2_);

});


