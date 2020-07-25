#ifdef __clang__ // using C99 VLA 

// also inspired by https://gustedt.wordpress.com/2014/09/08/dont-use-fake-matrices/

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
// MSVC (the latest as of 2020 APR) compiles  only if the code is in the C file
//


#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

// https://vorpus.org/blog/why-does-calloc-exist/
#undef DBJ_ALLOC
#define DBJ_ALLOC(S_) calloc(1,S_)

#undef DBJ_FREE
#define DBJ_FREE(P_) do { assert(P_ != NULL ); if(P_ != NULL) free(P_); P_ = NULL; } while(0)

/*
   (c) 2020 by dbj@dbj.org CC BY SA 4.0
 ---------------------------------------------------------------------------------
   ridiculously small and fast matrix

   In the "dbj mx" the data type
   is pointer to a whole of an 2D array

	typedef T (*type_name)[W][H]

*/
#define dbj_mx_declare( T, type_name, width, height ) typedef T(*type_name)[width][height]
#define dbj_mx_type( T, type_name, width, height ) T(*type_name)[width][height]
/*
	  since we keep the matrix in the data type as above
	  this is how we reach a desired slot and the value
*/
#define dbj_mx_slot(name,j,k)  ((assert(name!=NULL), (*name[j][k])))
/*
	 allocation is beautifully simple and elegant, thanks to C99
*/
#define dbj_mx_make( T, VAR_, W_, H_  ) T (*VAR_)[W_][H_] = calloc(1, sizeof(T[W_][H_])) 

#define dbj_mx_free( ptr_ ) DBJ_FREE( ptr_ )

/*
	more or less everything can be done through the for-each concept
	see the callback specimens bellow
	remember! name is pointer to the whole matrix as declared above
*/
#define dbj_mx_foreach(mx_ptr_ ,w,h, cb) \
      do {\
      for ( int j =0 ; j < w; ++j) \
      for ( int k =0 ; k < h; ++k) \
         cb( w,h, mx_ptr_, j,k );  \
    } while(0)

/* SAMPLING starts here *****************************************/

// callback specimen
// prints the matrix of int's
static inline void cback_print(const int W, const int H, int(*mx)[W][H], int R, int C)
{
	static int row_ctr = 0;
	if (0 == (row_ctr++ % W)) printf("\n");

	printf(" [%02d %02d]: %04d", R, C, (*mx)[R][C]);
}

// arbitrary fill callback
static inline void cback_zero
( const int W, const int H, int (*mx)[W][H], int R, int C )
{
	assert(R < W);
	assert(C < H);
	(*mx)[R][C] = 0 ;
}
/******************************************************************/
// ad hoc testing
void dbj_mx_sampling(const int W , const int H )
{
	// declares mx type
	// typedef  int(*mx2d_t)[W][H];
	// dbj_mx_declare(int, mx2d_t, W, H);

	// declares and defines the mx variable
	dbj_mx_make(int, mx2d, W,H);

	assert(mx2d);

	// zero the matrix
	dbj_mx_foreach(mx2d, W, H, cback_zero);

	// change the int value in mx2d[1][1]
	// dbj_mx_slot(mx2d, 1, 1) = 42;

	printf("\n\n");
	// print the matrix
	dbj_mx_foreach(mx2d, W, H, cback_print);

	dbj_mx_free(mx2d);
 }
/******************************************************************/
/* EOF */
#else
void dbj_mx_sampling(const int W, const int H) {
#pragma message(__FILE__ "\ndbj_mx_sampling() requires clang compiler")
}
#endif // __clang__ // using C99 VLA 
