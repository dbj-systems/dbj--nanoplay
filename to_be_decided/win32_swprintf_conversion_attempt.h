#pragma once
#ifndef TEST_WCONVERTER_WSTRING_INC
#define TEST_WCONVERTER_WSTRING_INC

/// --------------------------------------------------------------------
#ifdef __clang__
/// --------------------------------------------------------------------
#include <assert.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
/*
#define __STDC_WANT_LIB_EXT1__1
#include <stdio.h>
int sprintf_s(char * restrict s, rsize_t n,
const char * restrict format, ...);
*/
#define __STDC_WANT_LIB_EXT1__1
#include <stdio.h>
/// --------------------------------------------------------------------
#undef countof
#define countof(a)  (sizeof(a) / sizeof(*(a)))
/// --------------------------------------------------------------------
#ifdef __cpplang
extern "C" {
#endif // __cpplang

    typedef struct wstring {
        int size;
        wchar_t* ptr;
    } wstring;

#ifndef MessageBox
    int __stdcall
        MessageBoxW(
            void* hWnd,
            const wchar_t* lpText,
            const wchar_t* lpCaption,
            unsigned int uType);
#endif

    static inline void mbox(wchar_t* msg_)
    {
#define MB_ICONEXCLAMATION          0x00000030L
        static const wchar_t* app_full_path = (__wargv != 0 ? __wargv[0] : L"null " );

        (void)MessageBoxW(NULL, msg_, app_full_path, MB_ICONEXCLAMATION);

#undef MB_ICONEXCLAMATION
    }

    static inline wstring wconverter(unsigned int size_, const char narrow_str_[size_])
    {
        wchar_t* buff = (wchar_t*)calloc(size_, sizeof(wchar_t));
        unsigned int const buffer_size = size_;

        // int swprintf_rez_ = swprintf(buff, size_, L"%S", narrow_str_);

        int  swprintf_rez_ = _swprintf_s_l(
            buff,
            buffer_size,
            L"%S",
            /*locale_t locale*/ NULL,
            narrow_str_
        );

        if (swprintf_rez_ < 0)
        {
            perror("sprintf_s failed");
            exit(1);
        }

        return (wstring) { .size = swprintf_rez_, .ptr = buff };
    }
    /// --------------------------------------------------------------------

    static inline int test_wconverter_wstring()
    {
        // char narrow_str[] = "z\u00df\u6c34\U0001f34c";
        // or 
        char narrow_str[] = "zß水🍌";
        // or "\x7a\xc3\x9f\xe6\xb0\xb4\xf0\x9f\x8d\x8c";
        setlocale(LC_ALL, "en_US.UTF-8");
        wstring wstr = wconverter(countof(narrow_str), narrow_str);
        mbox(wstr.ptr);
        setlocale(LC_ALL, "C"); // back to default
        free(wstr.ptr);

        return EXIT_SUCCESS;
    }

/// --------------------------------------------------------------------
#ifdef __cpplang
} // extern "C" {
#endif // __cpplang

/// --------------------------------------------------------------------
#else
static inline int test_wconverter_wstring() {
#pragma message( __FILE__ "\ntest_wconverter_wstring() requires clang compiler")
    return EXIT_FAILURE;
}
#endif // __clang__
/// --------------------------------------------------------------------

#endif // !TEST_WCONVERTER_WSTRING_INC
