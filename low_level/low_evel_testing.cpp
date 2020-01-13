#include "../common.h"
#include "../dbj--nanolib/nonstd/aligned_allocator.h"
#include "../dbj--nanolib/nonstd/stack_allocator.h"

#include <memory>

// if this compiles with the result of get_allocator()
// on the container using your allocator
// in that case your allocator is fine
// NOTE: can not use reference on the allocator argument
// because get_allocator() returns a copy of allocator
// second reason is, allocator traits requires non const
// reference to the allocator instance used
template< typename Allocator , typename value_t = typename Allocator::value_type >
void traits_sampling( Allocator  allocator_ , value_t def_val_ )
{
    using my_trait = std::allocator_traits < Allocator >;

    DBJ_PRINT(DBJ_FG_GREEN_BOLD "\n\nAllocator type: %s, max size: %zu\n\n" DBJ_RESET,
        typeid(typename my_trait::allocator_type).name(), my_trait::max_size(allocator_));

    auto p = my_trait::allocate(allocator_, size_t(7));
    my_trait::deallocate(allocator_, p, size_t(7));

    value_t val_{};
    my_trait::construct(allocator_, &val_, def_val_);
    my_trait::destroy(allocator_, & val_);

}

#if (_ITERATOR_DEBUG_LEVEL != 0)
#pragma message(  "Warning _ITERATOR_DEBUG_LEVEL is not 0?" )
#endif

TU_REGISTER([]
{
        struct X final {
            char id;
            X() = delete;
        };

        using small_vector = dbj::nanolib::alloc::small_container< std::vector, X >;

        small_vector v(4, { '*' });
        // CAUTION: makes a copy of allocator in use
        traits_sampling(v.get_allocator(), { '*' });

        v[0] = { 'A' };
        v[1] = { 'B' };
        v[2] = { 'C' };
        v[3] = { 'D' };
    auto data_ = v.data();
    data_ = nullptr;
    auto size_ = v.size();
    size_ = 0;
});

#include <intrin.h>

template <typename T, std::size_t Alignment>
using aaloc = dbj::nanolib::alloc::aligned_allocator<T, Alignment>;

TU_REGISTER([]()
{
    typedef std::vector<__m128, aaloc<__m128, sizeof(__m128)> > aligned_vector;

    aligned_vector lhs{ 1000 };
    aligned_vector rhs{ 1000 };

    traits_sampling(lhs.get_allocator(), __m128( _mm_set_ps(1,2,3,4) ) );


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
    mul = __m128{};

});

/*
testing dbj++platform.h
*/
TU_REGISTER(
    [] {
        namespace pm = dbj::nanolib::platform;
        DBJ_PRINT( "\n\n dbj::nanolib::platform::NAME : \"%s\" \n dbj::nanolib::platform::CODE : %3d \n\n", pm::NAME, pm::CODE );

        namespace cr = dbj::nanolib::compiler;
        DBJ_PRINT("\n\n dbj::nanolib::compiler\n NAME : %s\n CODE : \"%3u\" \n MAJOR : %3d \n\n", cr::NAME, cr::CODE, cr::MAJOR);

        namespace le = dbj::nanolib::language;
        DBJ_PRINT("\n\n dbj::nanolib::language\n NAME : %s\n CODE : %3u \n\n", le::NAME,  le::CODE );
    }
);

/*
testing dbj++ct.h
*/

#include "../dbj--nanolib/nonstd/dbj++ct.h"

TU_REGISTER(
    [] {
        namespace ct = dbj::nanolib::ct;

        char target[] = "0123456789";

        assert( 0 == ct::mem_set_s(target, sizeof target, '!', 3 ) );
    }
);

TU_REGISTER(
    [] {
        namespace log = dbj::nanolib::logging;
     
        // log to console
        // default timestamp
        log::log("Output", " with time", "stamp");
        log::logf("|%-12s|--|%12s|--|%-12s|\n","Formated", "output", "with prefix");

        log::config::nanosecond_timestamp();
        log::log("Output", " with nanosecond time", "stamp");
        log::logf("|%-12s|--|%12s|--|%-12s|", "Formated", "output", "with nanosecond time stamp");

        // chnage where is the output sinking to
        log::config::set_sink_function([](std::string_view log_line) {  fprintf(stderr, log_line.data());  });

        // no time stamps, console output
        log::print("\n\n'Normal'", " output", " with no prefix!\n");
        // sinking to the syslog
        // log::config::set_sink_function([](std::string_view log_line) {  local_syslog_client( log_line.data());  });
        // no time stamp, local syslog clinet  output makes the time stamp
        log::prinf("\n|%-12s|--|%12s|--|%-12s|\n", "This", "goes", "to syslog()");

        log::config::restore_default_sink_function();

        log::log("Back to default\n\n");
    }
);