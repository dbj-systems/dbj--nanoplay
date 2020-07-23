// https://godbolt.org/z/ViKjcolsF
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
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <assert.h>

// https://vorpus.org/blog/why-does-calloc-exist/
#undef DBJ_ALLOC
#define DBJ_ALLOC(N_,S_) calloc(N_,S_)

#undef DBJ_FREE
#define DBJ_FREE(P_) do { assert(P_ != NULL ); if(P_ != NULL) free(P_); P_ = NULL; } while(0)

#define DBJ_CONCAT 

/// ---------------------------------------------------------------------------------
#define  dbj_mx_2_declare(T_)
typedef struct dbj_mx_2 {
	unsigned cols;
	unsigned rows;
	unsigned char * data;
} dbj_mx_2 ;

#define dbj_mx_2_make(cols_, rows_) (dbj_mx_2){ \
  .cols = cols_, .rows = rows_, .data = calloc( cols_ * rows_ , sizeof(dbj_mx_2)) \
}

void dbj_mx_2_free(dbj_mx_2 mx ) {	DBJ_FREE( mx.data ); }

#define dbj_mx_2_get(mx_var,T,row__,col__) (T*)&(mx_var.data[ mx_var.rows * row__ + col__  ])

/*
call back footprint

typedef void (*dbj_mx2_callback)( dbj_mx_2 *, unsigned row, unsigned col, T * value )
*/

#define dbj_mx_2_foreach(mx_var, T, call_back_) \
do {\
for ( int col=0 ; col < mx_var.cols ; ++col) \
for ( int row=0 ; row < mx_var.rows ; ++row)\
{\
call_back_( & mx_var , row, col, dbj_mx_2_get(mx_var, T, row, col) );\
} \
} while(0)

/* SAMPLING starts here *****************************************/

// callback specimen
// prints the matrix of int's
static inline void cback_print(dbj_mx_2 * mx_ , int row, int col, const int* val)
{
	assert(mx_);
	assert(val);

	assert(
		*val == *dbj_mx_2_get( (*mx_), int, row, col)
	);

	static int row_ctr = 0;
	if (0 == (row_ctr++ % (mx_->cols) )) printf("\n");
	printf(" [%02d %02d]: %04d", row, col, *val);
}

static inline void zoro(dbj_mx_2* mx_, int row, int col, int* val)
{
	assert(mx_);
	assert(val);
	*val = 64 ;
}
/******************************************************************/
void dbj_mx_2_sampling(const int cols, const int rows) {

	dbj_mx_2 mx_ = dbj_mx_2_make(2, 2);

	// zero the matrix
	dbj_mx_2_foreach(mx_, int, zoro);

	// change the int value in mx2d[1][1]
	int* val = dbj_mx_2_get(mx_, int, 1, 1);
	*val = 64 + 42;

	printf("\n\n");
	// print the matrix
	dbj_mx_2_foreach(mx_, int, cback_print);

	dbj_mx_2_free(mx_);
 }
/******************************************************************/
/* EOF */