#pragma once
#include "../common.h"

namespace dbj::mtx {

	/*
	(c) 2019/2020 by dbj@dbj.org

	C++ modern lambda powering a modern matrix

	matrix orienteering  logic used in here

	declaration : int M[3][3] means in 2d space :

	width : 3
	height: 3

	0,0   ...  ...
	...   ...  ...
	...   ...  2,2

	it is important to use the words "width" and "height". Think of matrix as an rectangle in 2D space. 
	Where the origin (0,0) is the upper left corner

	The key idea bellow is to allocate 1D array and treat it as an 2D array aka matrix

   // create on stack
    T arry_[ width * height ] ;

	// to fetch the cell X,Y
	return 	arry[row_ * rows + col_];

    // example 
	// to T matrix[2,2] ; 

            width = height = 2
	   row := { 0 .. 1 }
	    |
		V	  0   1      <-- col := { 0 .. 1 }
			+---+---+
		0	|   |   |    
			+---+---+
		1	|   |   |    
			+---+---+

	// we create 
	T arry[2*2]{} ;

      0   1   2   3      <-- index := { 0 .. 3 }
	+---+---+---+---+
    |   |   |   |   |    
	+---+---+---+---+

    // index calculation
	matrix[1,2] --> (x = 1, y = 1, width = 2) 
	    --> arry[y * height + x] --> arry[2 + 1] --> arry[3]
    
	*/

	// notice the array size as an compile time template argument
	// C++ can not declare run time native arrays
	template< typename T, size_t len_ >
	inline constexpr auto arr_stack() { return std::array<T, len_>{ {}}; }

	// notice the array size as an compile template argument
	// C++ can not declare run time native arrays
	// can not be constexpr
	// because of unique_ptr
	template< typename T, size_t len_ >
	inline std::unique_ptr<T[]> arr_heap() { return std::make_unique<T[]>(len_); }

    // mx is a method that returns lambda object
	// in this case declared as an 'inner class'
	template< typename T, size_t rows, size_t cols, 
	// F is a templated function
	typename F >
	inline constexpr auto mx(
		F source_
	)
	{
		// we are in here only on the first call
		// we return inner lambda to be executed by users
		// later on
		return[
			// here we create the 1D array and store it in the 
			// lambda object member called arry
			// source_ has to be a function that returns an instance
			// that has indexing operator aka "[]"
			// examples are return types of 'arr_stack' and 'arr_heap' above
			// notice they are passed array size as an compile template argument
			// otherwise C++ can not declare run time native arrays
			arry = source_()
			]
		(size_t row_, size_t col_) 
		    // compile time possibility
		    constexpr 
		    // we declare this lambda mutable so it is not
			// an 'operator () const' on the lambda object
			mutable
			// thus we can return the reference 
			// to the value stored in above defined 'arry'
			->T&
		{
			assert(row_ < /*=*/ rows);
			assert(col_ < /*=*/ cols);
			// important: 'rows' and 'height' mean the same
			// 'row' and 'Y' mean the same
			// 'col' and 'X' mean the same
			// matrix index X,Y transform into the 
			// the index for the 'arry' 
			// declared as T arry[ rows * cols ] 
			return 	arry[row_ * rows + col_];
		};
	} // mx

// at this point templated function name 
// is used, this is a macro so this is text substitution 
// for example this is created
// as a result of using  dbj_mx_make_heap macro as bellow
//  dbj::mtx::mx<T,R,C>( dbj::mtx::arr_heap<T, R * C> 
#define dbj_mx_make(T, R, C, K) dbj::mtx::mx<T, R, C>(dbj::mtx::K<T, R * C>)

// here we pass templated function names not function pointers, 
#define dbj_mx_make_heap(T,R,C) dbj_mx_make(T, R, C, arr_heap)
#define dbj_mx_make_stack(T,R,C) dbj_mx_make(T, R, C, arr_stack)

// change the matrix and return the same matrix all by value
// were matrix is the result of calling the mx function above 
// to create the matrix
// thus matrix is the lambda object, supposedly highly optimized
// to be moved
// this is critical as this lambda is probably called on each cell
// of some matrix
inline auto changer = [](auto matrix_, size_t row_, size_t col_, auto value_) {
	matrix_(row_, col_) = value_;
	return matrix_;
};

inline auto printer = [](auto matrix_, size_t width_, size_t height_) -> void {
		using dbj::nanolib::logging::log;

		for (size_t row_ = 0; row_ < width_; row_++) {
			log("\n{");
			for (size_t col_ = 0; col_ < height_; col_++) {
				/// cout << setw(3) <<  matrix_(row_, col_) << " ";
				log(matrix_(row_, col_), " ");
			}
			log("}\n");
		}
		log("\n");
};

	TU_REGISTER([]
		{
			DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE); DBJ_PRINT(" " DBJ_RESET);

			using dbj::nanolib::logging::log;
			using namespace dbj::mtx;

			constexpr size_t width_ = 3; // col index := 0,1,2
			constexpr size_t height_ = 3; // row index := 0,1,2

			constexpr size_t last_col_ = width_ - 1; // 2
			constexpr size_t last_row_ = height_ - 1; // 2

             // notice this is compile time operation
			constexpr auto matrix_on_stack = dbj_mx_make_stack(int, width_, height_);
             // notice this is run time operation
			auto matrix_on_heap = dbj_mx_make_heap(int, width_, height_);

			log("matrix_on_stack\n");
			printer(changer(matrix_on_stack, last_col_, last_row_, 42), width_, height_);

			log("\nmatrix_on_heap\n");
			printer(changer(matrix_on_stack, last_col_, last_row_, 42), width_, height_);
		});

#undef dbj_mx_make 
#undef dbj_mx_make_heap 
#undef dbj_mx_make_stack 

} // dbj::mtx
