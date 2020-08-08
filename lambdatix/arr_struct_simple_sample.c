#ifdef __clang__ // using C99 VLA 

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#define SX(F_,V_) printf("\n %s :--> "F_ , #V_, V_ )

#define SWAP(a,b) do { __typeof__(a) temp; temp = a; a = b; b = temp; } while(0)

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

enum { ROWS = 3, COLS = 4 };

/// ------------------------------------------------------------------------
typedef struct ARR {
    size_t size;
    int(*data)[];
} ARR;

ARR make_arr(size_t sze_) {
    return (ARR) { .size = sze_, calloc(1, sizeof(int[sze_])) };
}

static void free_arr(ARR arr_) {
    free(arr_.data);
    arr_.data = 0;
}

static void  fill_arr(ARR arr_, int count, ...)
{
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; ++i)
    {
        if (!(i < arr_.size)) break;
        (*arr_.data)[i] = va_arg(args, int);
    }
    va_end(args);
}

/// ---------------------------------------------------------------
void  arr_struct_simple_sample() {

    // create
    ARR iarr1 = make_arr(3);
    ARR iarr2 = make_arr(3);

    // populate
    fill_arr(iarr1, 3, 1, 2, 3);
    fill_arr(iarr2, 3, 3, 2, 1);

    // swap
    SWAP(iarr1, iarr2);

    // check
    SX("%6s", 3 == (*iarr1.data)[0] ? "true" : "false");
    SX("%6s", 1 == (*iarr2.data)[0] ? "true" : "false");

    // free
    free_arr(iarr1);
    free_arr(iarr2);

}

#else // ! __clang__

void  arr_struct_simple_sample() {
#pragma message(__FILE__ "\arr_struct_simple_sample() requires clang compiler")
}

#endif // ! __clang__ 
