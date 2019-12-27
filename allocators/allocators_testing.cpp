#include "stack_allocator.h"
#include "aligned_allocator.h"

//std::size_t memory = 0;
//std::size_t alloc = 0;
//
//void* operator new(std::size_t s) // throw(std::bad_alloc)
//{
//    memory += s;
//    ++alloc;
//    return malloc(s);
//}
//
//void  operator delete(void* p) throw()
//{
//    --alloc;
//    free(p);
//}

// very fast and very dangerous
template< template< class T_, class A_> typename container_type, typename value_type, size_t stack_alloc_size = 0xFFF, 
    typename stack_alloc_type  =  stackalloc_space::stack_allocator<value_type, stack_alloc_size> >
using small_container = container_type< value_type , stack_alloc_type > ;

static auto dumsy = []
    {
        using small_vector = small_container< std::vector, char > ;
        //using small_vector = std::vector<char> ;

        small_vector v(4, '*');
        // v.resize(4);

        v[0] = ('A');
        v[1] = ('B');
        v[2] = ('C');
        v[3] = ('D');

        auto data_ = v.data();
        auto size_ = v.size();

        return true;
}();

#include <intrin.h>

template <typename T, std::size_t Alignment>
using aaloc = allignedalloc_space::aligned_allocator<T, Alignment>;

static auto test_aligned_allocator = []()
{
    typedef std::vector<__m128, aaloc<__m128, sizeof(__m128)> > aligned_vector;

    aligned_vector lhs{ 1000 };
    aligned_vector rhs{ 1000 };

    float a = 1.0f;
    float b = 2.0f;
    float c = 3.0f;
    float d = 4.0f;

    float e = 5.0f;
    float f = 6.0f;
    float g = 7.0f;
    float h = 8.0f;

    for (std::size_t i = 0; i < 1000; ++i)
    {
        lhs.push_back(_mm_set_ps(a, b, c, d));
        rhs.push_back(_mm_set_ps(e, f, g, h));

        a += 1.0f; b += 1.0f; c += 1.0f; d += 1.0f;
        e += 1.0f; f += 1.0f; g += 1.0f; h += 1.0f;
    }

    __m128 mul = _mm_mul_ps(lhs[10], rhs[10]);

    return mul;
}();
