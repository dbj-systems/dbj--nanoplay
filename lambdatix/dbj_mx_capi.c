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

#ifdef _WIN32
/// -------------------------------------------------------------------------------
/// now here is the secret sauce key ingredient
/// on windows machine these are 
/// the fastest, proven and measured

#undef  DBJ_NANO_CALLOC
#define DBJ_NANO_CALLOC(T_,S_) (T_*)HeapAlloc(GetProcessHeap(), 0, S_ * sizeof(T_))

#undef DBJ_NANO_MALLOC_2
#define DBJ_NANO_MALLOC_2(T_,S_)(T_*)HeapAlloc(GetProcessHeap(), 0, S_)

#undef DBJ_NANO_FREE
#define DBJ_NANO_FREE(P_) HeapFree(GetProcessHeap(), 0, (void*)P_)

// avoid including windows.h
__declspec(dllimport) void* __stdcall  GetProcessHeap(void);
__declspec(allocator) void* __stdcall HeapAlloc(void* /*hHeap*/, int /*flags*/,size_t /*dwBytes*/);
int __stdcall HeapFree(void* /*hHeap*/, int  /*dwFlags*/, void* /*lpMem*/);

#else
#error It seems WIN32 is required
#endif // _WIN32

/// ---------------------------------------------------------------------------------
/// ---------------------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

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

#ifndef _WIN32
#define dbj_mx_make( T, width, height ) \
/*(T(*)[width][height])*/malloc( sizeof(T[width][height]) )
#else
#define dbj_mx_make( T, width, height ) \
(T*)HeapAlloc(GetProcessHeap(), 0, sizeof(T[width][height]))
#endif // _WIN32

#ifndef _WIN32
#define dbj_mx_free( ptr_ ) free(ptr_)
#else
#define dbj_mx_free( ptr_ )HeapFree(GetProcessHeap(), 0, (void*)ptr_)
#endif // _WIN32


/*
	more or less everything can be done through the for-each concept
	see the callback specimens bellow
	remember! name is pointer to the whole matrix as declared above
*/
#define dbj_mx_foreach(name,w,h, cb) \
      do {\
      for ( int j =0 ; j < w; ++j) \
      for ( int k =0 ; k < h; ++k)\
         cb(j,k, & dbj_mx_slot(name,j,k));\
    } while(0)

/* This is where dbj mx ends ************************************/

/* SAMPLING starts here *****************************************/

// note: width and height do not have to be compile time constants
enum { W = 4, H = 4 };
// callback specimen
// breaks on false return
// prints the matrix of int's
inline void cback_print(int j, int k, int* const val)
{
	static int row_ctr = 0;
	if (0 == (row_ctr++ % W)) printf("\n");
	printf(" [%02d][%02d]: %04d", j, k, *val);
}

// fill sample callback
inline void cback_fill(int j, int k, int* const val)
{
	*val = (10 * j) + k;
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