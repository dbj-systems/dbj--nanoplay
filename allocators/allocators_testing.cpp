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

constexpr size_t stack_alloc_size = 200;

template<typename T_>
using stack_alloc_type = stackalloc_space::stack_allocator<T_, stack_alloc_size>;

static auto dumsy = []
    {
        // using SmallVector = std::vector<int, stack_allocator<int, 200> > ;
        using SmallVector = std::vector<int, stack_alloc_type<int> > ;

        //stack_arena<N> a;
        SmallVector v{int(0),4};

        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        v.push_back(4);

        return true;
}; //  ();

#include <intrin.h>

static auto test_aligned_allocator = []()
{
    typedef std::vector<__m128, allignedalloc_space::aligned_allocator<__m128, sizeof(__m128)> > aligned_vector;

    aligned_vector lhs{ 1000, __m128{} };
    aligned_vector rhs{ 1000, __m128{} };

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
