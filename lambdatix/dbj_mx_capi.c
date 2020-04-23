// https://godbolt.org/z/ViKjWF
//
// dbj zippy matrix nano API
// (c) 2020 APR by dbj@dbj.org CC BY SA 4.0
//
// minimal gcc required: GCC 5.1 -std=c99
//
// -O3 will work of course but beware: it will unroll 
// all the loops. Executable size will grow!
//
// minimal clang required: clang 4.0.0 -std=c99
//
// -O3 will works with executable smaller than gcc build
// 
// I have not done benchmarking on the two
//
// There is no Error checking whatsoever
//
// MSVC? Well it compiles here but it should not
// without any output whatsoever ...
//


#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

static struct { int major; int minor; int patch; } version = { 0,5,0 };

/*
   In the "dbj mx" the "matrix" data type
   is pointer to a whole of an 2D array

    T (*2d_array_pointer)[W][H]
*/
#define dbj_mx_declare( T, name, width, height ) T(*name)[width][height]
/*
      since we keep the matrix in the data type as above
      this is how we reach a desired slot and the value
*/
#define dbj_mx_slot(name,j,k) (*name)[j][k]
/*
     allocation is beautifuly simple and elegant, thanks to C99

     note: this is fast but matrix slots are not guarenteed to be "zeroed"
*/
// #define dbj_mx_make( T, width, height ) /*T(*)[width][height])*/calloc( width * height,  sizeof(T) )

#define dbj_mx_make( T, width, height ) /*(T(*)[width][height])*/malloc( sizeof(T[width][height]) )

#define dbj_mx_free( ptr_ ) free(ptr_)

/*
    more or less everything can be done through the for-each concept
    see the callback specimens bellow
    remember! name is pointer to the whole matrix as declared above
*/
#define dbj_mx_foreach(name,w,h, cb) \
      do {\
      for ( int j =0 ; j < w; ++j) {\
      for ( int k =0 ; k < h; ++k)\
         if ( false == cb(j,k, &(*name)[j][k]) ) break;\
      }\
    } while(0)

/* This is where dbj mx ends ************************************/

/* SAMPLING starts here *****************************************/

// note: width and height do not have to be compile time constants
enum { W = 0xF, H = 0xF };
// callback specimen
// breaks on false return
// prints the matrix of int's
inline bool cback_print(int j, int k, int* const val)
{
    static int row_ctr = 0;
    if (0 == (row_ctr++ % W)) printf("\n");
    printf(" [%02d][%02d]: %04d", j, k, *val);
    return true;
}

// fill sample callback
inline bool cback_fill(int j, int k, int* const val)
{
    *val = (10 * j) + k;
    return true;
}
// ad hoc testing
void dbj_mx_sampling()
{
    dbj_mx_declare(int, mx2d, W, H);
    mx2d = dbj_mx_make(int, W, H);

    dbj_mx_slot(mx2d, 1, 1) = 42; // just to show 

    printf("\n\n");

    dbj_mx_foreach(mx2d, W, H, cback_fill);
    dbj_mx_foreach(mx2d, W, H, cback_print);
    dbj_mx_free(mx2d);
}
/******************************************************************/
/* EOF */