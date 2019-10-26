#pragma once

#include "../common.h"
#include <vector>

#define DBJ_TESTING_VECTOR_PERFORMANCE 1
#define DBJ_TESTING_VECTOR_CONFORMANCE 1

namespace dbj::nanoplay::testing {

#if DBJ_TESTING_VECTOR_PERFORMANCE
	constexpr size_t number_of_insertions 
		= size_t(0xFFFF);

	template< typename V>
	inline void test_push( V vec_, typename V::value_type specimen_ ) 
	{
		DBJ_PRINT( DBJ_FG_GREEN "\nTesting push_back of %zu elements, into %s" DBJ_RESET , number_of_insertions, 
			typeid(V).name() );
		time_t st = clock();
		
		for (size_t i = 0; i < number_of_insertions; ++i) 
			vec_.push_back( specimen_ );

		DBJ_PRINT("\nInsertion done in: %.3f seconds\n", double((clock() - st)) / CLOCKS_PER_SEC);

		DBJ_PRINT(DBJ_FG_GREEN "\nTesting erase(begin()) on %zu elements.\n" DBJ_RESET, number_of_insertions);
			st = clock();
			while (vec_.size() > 0) {
				vec_.erase(vec_.begin());
				printf("\r%llu", vec_.size());
			}
		DBJ_PRINT("\nErasing done in: %.3f seconds\n", double((clock() - st)) / CLOCKS_PER_SEC);
	}

	TU_REGISTER([] {
		test_push( std::vector<wchar_t>{}, L'@' );
		test_push(dbj::nanolib::vector<wchar_t>{}, L'@');
		});
#endif // DBJ_TESTING_VECTOR_PERFORMANCE

	/*
	--------------------------------------------------------------------------
	*/
#if DBJ_TESTING_VECTOR_CONFORMANCE
	using dbj::nanolib::v_buffer;

	inline v_buffer::buffer_type reduce_chars_to_one 
	(v_buffer::buffer_type buff_)
	{
		buff_.erase(
			std::unique(buff_.begin(), buff_.end()),
			buff_.end());
		return buff_;
	}

	inline bool space_and_space (char lhs, char rhs)
	{
		return (lhs == ' ') && (rhs == ' ');
	}

	inline v_buffer::buffer_type reduce_spaces_to_one
	(v_buffer::buffer_type buff_)
	{
		buff_.erase(
			std::unique(buff_.begin(), buff_.end(), space_and_space), 
			buff_.end());
		return buff_;
	}

	TU_REGISTER([] {
		DBJ_PRINT(DBJ_FG_GREEN "\nTesting %s usage with standard algorithms\n" DBJ_RESET
			, typeid( v_buffer::buffer_type ).name()
		);

		DBJ_TX(
			reduce_chars_to_one(
				v_buffer::make("aaaaabbbbbbbbbccccccccccddddddddddeeeeeeeeefffffffffgggggggghhhhhhhh") 
			)
		);

		DBJ_TX(
			reduce_spaces_to_one(
				v_buffer::make("a         b  bc      c cd  dd   dde  ee   eff  f   fg g  gggh h   hh") 
			)
		);

	});
#endif // DBJ_TESTING_VECTOR_CONFORMANCE

} //  namespace dbj::nanoplay 

