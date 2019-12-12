#pragma once

#include "../common.h"

//#include <iostream>
//#include <array>
//#include <cassert>
//#include <memory>
//#include <cstdlib>

namespace dbj::mtx {

/*
matrix orienteering  logic

declaration : int M[3][3]
in 2d space :

width : 3
height: 3

0,0   ...  ...
...   ...  ...
...   ...  3,3

*/

	template< typename T, size_t len_ >
	inline constexpr auto arr_stack() { return std::array<T, len_>{ {}}; }

	// can not be constexpr
	// because of unique_ptr
	template< typename T, size_t len_ >
	inline std::unique_ptr<T[]> arr_heap() { return std::make_unique<T[]>(len_); }

	template< typename T, size_t rows, size_t cols,	typename F >
	inline constexpr auto mx(F source_)
	{
		return[arry = source_()]
		(size_t row_, size_t col_) constexpr mutable->T&
		{
			assert(row_ <= rows);
			assert(col_ <= cols);
			return 	arry[row_ * rows + col_];
		};
	} // mx


#define dbj_mx_make(T, R, C, K) dbj::mtx::mx<T, R, C>(dbj::mtx::K<T, R * C>)
#define dbj_mx_make_heap(T,R,C) dbj_mx_make(T, R, C, arr_heap)
#define dbj_mx_make_stack(T,R,C) dbj_mx_make(T, R, C, arr_stack)

	auto changer = [](auto matrix_, size_t row_, size_t col_, auto value_) {
		matrix_(row_, col_) = value_;
		return matrix_;
	};

	auto printer = [](auto matrix_, size_t width_, size_t height_) -> void {
		using namespace std;
		for (size_t row_ = 0; row_ < width_; row_++) {
			cout << endl << "{";
			for (size_t col_ = 0; col_ < height_; col_++) {
				cout << setw(3) <<  matrix_(row_, col_) << " ";
			}
			cout << "}" << endl;
		}
		cout << endl;
	};

	TU_REGISTER([]
		{
			using namespace dbj::mtx;
			using namespace std ;

			constexpr size_t width_ = 3; // 0,1,2
			constexpr size_t height_ = 3; // 0,1,2

			constexpr size_t last_col_ = width_  - 1; // 0,1,2
			constexpr size_t last_row_ = height_ - 1; // 0,1,2

			constexpr auto matrix_on_stack = dbj_mx_make_stack(int, width_, height_);
			 auto matrix_on_heap = dbj_mx_make_heap(int, width_, height_);

			cout << endl << "matrix_on_stack " << endl;
			printer( changer(matrix_on_stack , last_col_, last_row_, 42 ) , width_, height_) ;

			cout << endl << "matrix_on_heap " << endl;
			printer(changer(matrix_on_stack, last_col_, last_row_, 42), width_, height_);
		});

#undef dbj_mx_make 
#undef dbj_mx_make_heap 
#undef dbj_mx_make_stack 

} // dbj::mtx
