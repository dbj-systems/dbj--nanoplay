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
   is pointer to a *whole* of an 2D array

	typedef T (*type_name)[W][H]

*/
#define dbj_mx_declare( T, type_name, width, height ) typedef T(*type_name)[width][height]
#define dbj_mx_type( T, type_name, width, height ) T(*type_name)[width][height]
/*
	  since we keep the matrix in the data type as above
	  this is how we reach a desired slot and the value
*/
#define dbj_mx_slot(MXP_,j,k)  ( (assert(MXP_!=NULL), (*MXP_)[j][k] ) )
/*
	 allocation is beautifully simple and elegant, thanks to C99
*/
#define dbj_mx_make( T, VAR_, W_, H_  ) T (*VAR_)[W_][H_] = calloc(1, sizeof(T[W_][H_])) 

#define dbj_mx_free( ptr_ ) DBJ_FREE( ptr_ )

/*
	more or less everything can be done with the for-each concept
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

	printf(" [%02d %02d]: %04d", R, C, dbj_mx_slot(mx,R,C) );
}

/// ------------------------------------------------------------------------------------
// ad-hoc filler callback
static int int_filler_arg = 0;
static inline void int_filler
(const int W, const int H, int(*mx)[W][H], int R, int C)
{
	assert(R < W);
	assert(C < H);
	(*mx)[R][C] = int_filler_arg;
}
/******************************************************************/
/// naive algorithm
void matmult_int(
	size_t n1, size_t n2, size_t n3,
	int(*A)[n1][n2], int(*B)[n2][n3], int(*C)[n1][n3]
)
{
	for (size_t i1 = 0; i1 < n1; i1++) {
		for (size_t i3 = 0; i3 < n3; i3++) {
			(*C)[i1][i3] = 0.0;
			for (size_t i2 = 0; i2 < n2; i2++)  (*C)[i1][i3] += (*A)[i1][i2] * (*B)[i2][i3];
		}
	}
}
/******************************************************************/
// ad hoc testing
void dbj_mx_sampling(const int W , const int H )
{
	int n1 = 2, n2 = 3, n3 = 4;

	dbj_mx_make(int, A, n1, n2);
	dbj_mx_make(int, B, n2, n3);
	dbj_mx_make(int, C, n1, n3);

	int_filler_arg = 2;
	dbj_mx_foreach(A, n1, n2, int_filler);

	int_filler_arg = 2;
	dbj_mx_foreach(B, n2, n3, int_filler);

	matmult_int(n1, n2, n3, A, B, C);

	dbj_mx_foreach(C, n1, n3, cback_print);

	DBJ_FREE(A); DBJ_FREE(B); DBJ_FREE(C);
 }
/******************************************************************/
/* EOF */
#else
void dbj_mx_sampling(const int W, const int H) {
#pragma message(__FILE__ "\ndbj_mx_sampling() requires clang compiler")
}
#endif // __clang__ // using C99 VLA 
