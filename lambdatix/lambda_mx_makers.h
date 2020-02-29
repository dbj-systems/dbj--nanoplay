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

	*/

	template< typename T, size_t len_ >
	inline constexpr auto arr_stack() { return std::array<T, len_>{ {}}; }

	// can not be constexpr
	// because of unique_ptr
	template< typename T, size_t len_ >
	inline std::unique_ptr<T[]> arr_heap() { return std::make_unique<T[]>(len_); }

	template< typename T, size_t rows, size_t cols, typename F >
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
			using dbj::nanolib::logging::log;
			using namespace dbj::mtx;

			constexpr size_t width_ = 3; // 0,1,2
			constexpr size_t height_ = 3; // 0,1,2

			constexpr size_t last_col_ = width_ - 1; // 0,1,2
			constexpr size_t last_row_ = height_ - 1; // 0,1,2

			constexpr auto matrix_on_stack = dbj_mx_make_stack(int, width_, height_);
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
