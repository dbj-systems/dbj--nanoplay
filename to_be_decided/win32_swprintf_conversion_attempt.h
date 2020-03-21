#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define STRICT
#include <windows.h>
/// --------------------------------------------------------------------
#include <assert.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
/// --------------------------------------------------------------------
#ifdef __cpplang
extern "C" {
#endif // __cpplang

typedef struct wstring {
    int size;
    wchar_t* ptr;
} wstring ;

/// no can do with MSVC
#ifdef __clang__
inline wstring wconverter( unsigned int size_, const char narrow_str_[size_]) 
#else
inline wstring wconverter(unsigned int size_, const char * narrow_str_ )
#endif // __clang__
{
    wchar_t* buff = (wchar_t *)calloc(size_ , sizeof(wchar_t));
    unsigned int const buffer_size = size_ ;

    /// int swprintf_rez_ = swprintf_s(buff, buffer_size, L"%hs", narrow_str_);
    /// resolves to this
    /// NULL arg is locale
    int swprintf_rez_ = _swprintf_s_l(buff, size_, L"%S", NULL, narrow_str_);

    if (swprintf_rez_ > 0)
    {
        mbox(L"_swprintf_s_l failed");
    }

    return (wstring) { .size = swprintf_rez_ , .ptr = buff   };
}
/// --------------------------------------------------------------------
int worker(/*_In_ HINSTANCE hInstance*/)
{
    // char narrow_str[] = "z\u00df\u6c34\U0001f34c";
    // or 
    char narrow_str[] = "zß水🍌";
    // or "\x7a\xc3\x9f\xe6\xb0\xb4\xf0\x9f\x8d\x8c";

    wstring wstr = wconverter( _countof(narrow_str),  narrow_str);
    mbox(wstr.ptr);

    free( wstr.ptr );

    return EXIT_SUCCESS;
}

/// --------------------------------------------------------------------
#ifdef __cpplang
} // extern "C" {
#endif // __cpplang
