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
    address_type * memStart{}; // Beginning of memory pool
    address_type * next{}; // Num of next free block

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
        memStart = DBJ_ALLOC(address_type, numOfBlocks_.val, sizeOfEachBlock_.val);
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
        _ASSERTE(p);
        return index_type(((uint)(p - memStart)) / sizeOfEachBlock.val);
    }

    /// return index
    index_type allocate()
    {
        if (numInitialized < numOfBlocks.val)
        {
            address_type* p = addr_from_index(numInitialized);
            *p = numInitialized + 1;
            numInitialized++;
        }
        address_type* ret = nullptr;
        if (numFreeBlocks > 0)
        {
            ret = next;
            --numFreeBlocks;
            if (numFreeBlocks != 0)
            {
                next = addr_from_index(*((address_type*)next));
            }
            else
            {
                next = nullptr;
            }
        }
        return index_from_addr( ret );
    }
    
    /// in DEBUG mode we assert on trying to free the nullptr
    /// in RELEASE mode we just return
    /// we receive double pointer so that we can unlink the freed pointer
    void deallocate(index_type idx_ )
    {
        address_type* p = addr_from_index( idx_ );
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

        ++numFreeBlocks;
    }
}; // memory_pool


TU_REGISTER_NOT ([]
 {
            memory_pool mem_pool_(
                memory_pool::block_size{ 128 },
                memory_pool::block_count{ 64 }
            );

            memory_pool::index_type i1_ = mem_pool_.allocate();

            memory_pool::address_type* p1_ =
                mem_pool_.addr_from_index(i1_);

            mem_pool_.deallocate( i1_ );

            // this should be illegal now
            p1_ =  mem_pool_.addr_from_index(i1_);
});


