#ifdef CXX20_CUCHAR_READY
//  g++ prog.cc -Wall -Wextra -std=c++2a
//  clang++ prog.cc -Wall -Wextra -std=c++2a "-Wno-unused-variable"

#if __has_include(<cuchar>)
#  include <cuchar>
#  define have_cuchar 1
#else
#include <uchar.h>
#  define have_cuchar 0
#endif

#include <stdio.h>
#include <clocale>
#include <climits>

template<size_t N>
void  u32sample(const char32_t(&str32)[N])
{
#ifndef __clang__
    std::mbstate_t state{};
#else
    mbstate_t state{};
#endif

    char out[MB_LEN_MAX]{};
    for (char32_t const& c : str32)
    {
#ifndef __clang__
        /*std::size_t rc =*/ std::c32rtomb(out, c, &state);
#else
        /* std::size_t rc =*/ ::c32rtomb(out, c, &state);
#endif
        printf("%s", out);
    }
}

template<size_t N>
void  u8sample(const char8_t(&str8)[N])
{
#ifndef __clang__
    std::mbstate_t state{};
#else
    mbstate_t state{};
#endif

    char out[MB_LEN_MAX]{};
    for (char8_t const& c : str8)
    {
#ifndef __clang__
        /*std::size_t rc =*/ std::c8rtomb(out, c, &state);
#else
        /* std::size_t rc =*/ ::c8rtomb(out, c, &state);
#endif
        printf("%s", out);
    }
}

extern "C" int cuchar_sampler(int, char ** ) 
{
    std::setlocale(LC_ALL, "en_US.utf8");

#ifdef __linux__
    printf("\nLinux like OS ");
#endif

    printf("\n __cplusplus:\t\t %ld ", __cplusplus);
    printf("\n __cpp_char8_t:\t\t %ld ", __cpp_char8_t);
#ifdef __VERSION__
    printf("\n compiler __VERSION__:\t %s, ", __VERSION__);
#endif // __VERSION__

    printf("\n\nhave_cuchar:\t\t %d\n", have_cuchar);

    printf("\nchar32_t *, Converting to 'char *', and then printing: ");
    u32sample(U"ひらがな");
    printf("\nDone ...\n");

    printf("\nchar8_t *, Converting to 'char *', and then printing: ");
    u8sample(u8"ひらがな");
    printf("\nDone ...");

    return 42;
}

#endif // CXX20_CUCHAR_READY