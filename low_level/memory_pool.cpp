#pragma once
/*
Fast Efficient Fixed-Size Memory Pool
No Loops and No Overhead
Ben Kenwright
School of Computer Science
Newcastle University
Newcastle, United Kingdom,
b.kenwright@ncl.ac.uk
*/
class memory_pool
{ 
    // Basic type define
    typedef unsigned int uint;
    typedef unsigned char uchar;

    uint numOfBlocks{}; // Num of blocks
    uint sizeOfEachBlock{}; // Size of each block
    uint numFreeBlocks{}; // Num of remaining blocks
    uint numInitialized{}; // Num of initialized blocks
    uchar* memStart{}; // Beginning of memory pool
    uchar* next{}; // Num of next free block
public:
    memory_pool() = delete ;

    ~memory_pool() {
        if (memStart) {
            delete[] memStart;
            memStart = nullptr;
        }
    }

     memory_pool
     (size_t sizeOfEachBlock, uint numOfBlocks)
    {
        numOfBlocks = numOfBlocks;
        sizeOfEachBlock = sizeOfEachBlock;
        memStart = new uchar[sizeOfEachBlock *
            numOfBlocks];
        numFreeBlocks = numOfBlocks;
        next = memStart;
    }

    uchar* addr_from_index(uint i) const
    {
        return memStart + (i * sizeOfEachBlock);
    }

    uint index_from_addr(const uchar* p) const
    {
        return (((uint)(p - memStart)) / sizeOfEachBlock);
    }

    void* allocate()
    {
        if (numInitialized < numOfBlocks)
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
    void deallocate(void* p)
    {
        if (next != nullptr)
        {
            (*(uint*)p) = index_from_addr(next);
            next = (uchar*)p;
        }
        else
        {
            *((uint*)p) = numOfBlocks;
            next = (uchar*)p;
        }
        ++numFreeBlocks;
    }
}; // memory_pool


static void test_memory_pool() 
{
    constexpr auto sizeOfEachBlock = 128;

    memory_pool mem_pool_( 128,  /*numOfBlocks*/ 64);

    char * cp_ = (char*)mem_pool_.allocate();
}
