#ifdef __clang__ // using C99 VLA 

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

static inline void DBJ_FREE( void * P_)
{ assert(P_ != NULL ); if(P_ != NULL) free(P_); P_ = NULL; }

#define DBJ_CONCAT_ (a,b) a##b
#define DBJ_CONCAT (a,b)  DBJ_CONCAT_(a,b)

/* 
---------------------------------------------------------------------------------
creates a type and struct for a typed matrix
*/
#define  dbj_mx_declare(T_) \
typedef struct dbj_mx_##T_ { \
	unsigned cols; \
	unsigned rows; \
	T_ * data; \
}   dbj_mx_##T_

// create a compound literal
// from the type above
#define dbj_mx_2_make(MXT_ , cols_, rows_) (MXT_){ \
  .cols = cols_, .rows = rows_, .data = calloc( cols_ * rows_ , sizeof( MXT_) ) \
}

#define dbj_mx_2_free( MXV_) DBJ_FREE( MXV_.data )

#define dbj_mx_2_get(T_, MX_,row__,col__) (T_*)&(MX_.data[ MX_.rows * row__ + col__  ])

/*
call back footprint

typedef void (*dbj_mx2_callback)( dbj_mx_2 *, unsigned row, unsigned col, T * value )
*/

#define dbj_mx_2_foreach(T_, MXV_, call_back_) \
do {\
for ( int col=0 ; col < MXV_.cols ; ++col) \
for ( int row=0 ; row < MXV_.rows ; ++row)\
{\
call_back_( & MXV_ , row, col, dbj_mx_2_get( T_, MXV_, row, col) );\
} \
} while(0)

/* SAMPLING starts here *****************************************

bellow declares:

typedef struct dbj_mx_int { 
	unsigned cols;
	unsigned rows; 
	int * data; 
}  dbj_mx_int
*/
dbj_mx_declare(int);

// callback specimen
// prints the matrix of int's
static inline void cback_print(
	struct dbj_mx_int * mx_ , int row, int col, const int* val
)
{
	assert(mx_);
	assert(val);

	assert(
		*val == *dbj_mx_2_get( int, (*mx_), row, col)
	);

	static int row_ctr = 0;
	if (0 == (row_ctr++ % (mx_->cols) )) printf("\n");
	printf(" [%02d %02d]: %04d", row, col, *val);
}

static inline void zoro(dbj_mx_int * mx_, int row, int col, int* val)
{
	assert(mx_);
	assert(val);
	*val = 64 ;
}
/******************************************************************/
void dbj_mx_2_sampling(const int cols, const int rows) {

	dbj_mx_int mx_ = dbj_mx_2_make(dbj_mx_int, 2, 2);

	// zero the matrix
	dbj_mx_2_foreach( int, mx_, zoro);

	// change the int value in mx2d[1][1]
	int* val = dbj_mx_2_get( int, mx_, 1, 1 );
	*val = 64 + 42;

	printf("\n\n");
	// print the matrix
	dbj_mx_2_foreach(int, mx_, cback_print);

	dbj_mx_2_free(mx_);
 }
/******************************************************************/
/* EOF */
#else
void dbj_mx_2_sampling(const int cols, const int rows) {
#pragma message(__FILE__ "\ndbj_mx_2_sampling() requires clang compiler")
}
#endif // __clang__ // using C99 VLA 
