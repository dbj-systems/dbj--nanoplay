#ifdef __clang__ // using C99 VLA 

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#define SX(F_,V_) printf("\n%-24s:"F_ , #V_, (V_) )

// without that assert this SWAP would be completely generic
#define SWAP(a,b) do {                          \
assert(a.size == b.size );                      \
 __typeof__(a) temp; temp = a; a = b; b = temp; \
} while(0)

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#define SHOW(...)  fprintf( stderr, "%s", #__VA_ARGS__ )
#define PRINT(...) fprintf( stderr, __VA_ARGS__ )
/// ------------------------------------------------------------------------

#define ARR_ELEM_TYPE double
#define ARR_ELEM_TYPE_FMT_CHAR "%3.3f"

#define NAME(T_,S_) _CRT_CONCATENATE( T_, S_ )

#define ARRTYPE( T_, S_ ) \
typedef struct _CRT_CONCATENATE( T_, S_ ) { \
    size_t size; \
    T_ data[S_]; \
}  _CRT_CONCATENATE(T_, S_); \
\
static inline _CRT_CONCATENATE( T_, S_ ) \
_CRT_CONCATENATE(construct_, _CRT_CONCATENATE( T_, S_ ))   ( void ) { \
    return (_CRT_CONCATENATE( T_, S_ )) { .size = S_ }; \
}

ARRTYPE( float, 255 );

typedef struct ARR {
    size_t size;
    ARR_ELEM_TYPE(*data)[];
} ARR;

ARR construct_arr(size_t sze_) {
    return (ARR) { .size = sze_, .data = malloc( sizeof(ARR_ELEM_TYPE[sze_])) };
    // return (ARR) { .size = sze_, .data =  calloc(sze_, sizeof(ARR_ELEM_TYPE)) };
}

static void free_arr(ARR arr_) { free(arr_.data);  arr_.data = 0; }

static int print_arr(ARR arr_) {

    PRINT("{");
    for (unsigned k = 0; k < arr_.size; ++k) {
        PRINT(ARR_ELEM_TYPE_FMT_CHAR, (*arr_.data)[k] );
    }
    PRINT(" }");
    return arr_.size;
}

static ARR  make_arr(unsigned count, ...)
{
    ARR arr_ = construct_arr( count );
    va_list args;
    va_start(args, count);
    for (unsigned k = 0; k < count; ++k)
    {
        if ( k >= arr_.size ) break;
        (*arr_.data)[k] = (ARR_ELEM_TYPE)va_arg(args, ARR_ELEM_TYPE );
    }
    va_end(args);
    return arr_;
}

/// ---------------------------------------------------------------
void  arr_struct_simple_sample() {

    // create populate
    ARR A1 = make_arr( 6, 1.0, 2.0, 3.0,  4.0,  5.0,  6.0);
    ARR A2 = make_arr( 6, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0);

    PRINT("\nA1: "); print_arr(A1);
    PRINT("\nA2: "); print_arr(A2);
    // swap
    SHOW( SWAP(A1, A2) ) ;
    SWAP(A1, A2);

    PRINT("\nA1: "); print_arr(A1);
    PRINT("\nA2: "); print_arr(A2);

    // free
    free_arr(A1);
    free_arr(A2);

}

#else // ! __clang__

void  arr_struct_simple_sample() {
#pragma message(__FILE__ "\n\n requires clang compiler")
}

#endif // ! __clang__ 
