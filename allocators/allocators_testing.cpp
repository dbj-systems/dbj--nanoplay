#include "../common.h"
#include "../dbj--nanolib/nonstd/aligned_allocator.h"
#include "../dbj--nanolib/nonstd/stack_allocator.h"

TU_REGISTER([]
{
        using small_vector = dbj::nanolib::alloc::small_container< std::vector, char > ;
        small_vector v(4, '*');
        // v.resize(4);

        v[0] = ('A');
        v[1] = ('B');
        v[2] = ('C');
        v[3] = ('D');

        auto data_ = v.data();
        auto size_ = v.size();

});

#include <intrin.h>

template <typename T, std::size_t Alignment>
using aaloc = dbj::nanolib::alloc::aligned_allocator<T, Alignment>;

TU_REGISTER([]()
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

});
