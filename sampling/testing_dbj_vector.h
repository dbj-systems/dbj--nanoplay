#pragma once

#include "common.h"
#include <vector>

namespace dbj::nanoplay::testing {

#if DBJ_TESTING_VECTOR_PERFORMANCE
	constexpr size_t number_of_insertions 
		= size_t(0xFFFFFF /*MAXINT*/ );

	template< typename V>
	inline void test_push( V vec_ ) 
	{
		DBJ_PRINT( DBJ_FG_GREEN "\n\nTesting push_back of %zu elements, into\n\n %s\n\n" DBJ_RESET , number_of_insertions, 
			typeid(V).name() );
		time_t st = clock();

		for (size_t i = 0; i < number_of_insertions; ++i) vec_.push_back(i);
		DBJ_PRINT("\n\nDone in: %.3f seconds\n", double((clock() - st)) / CLOCKS_PER_SEC);
	}

	TU_REGISTER([] {
#if DBJ_TERMINATE_ON_BAD_ALLOC
	DBJ_PRINT(DBJ_FG_RED_BOLD "\n\nProgram is configured to terminate on heap memory exhaustion\n\n" DBJ_RESET );
#endif
		test_push( std::vector<size_t>{} );
		test_push(dbj::nanolib::vector<size_t>{});

		::system("@pause");
		});
#endif // DBJ_TESTING_VECTOR_PERFORMANCE

	/*
	--------------------------------------------------------------------------
	*/
#if DBJ_TESTING_VECTOR_CONFORMANCE
	using dbj::nanolib::v_buffer;

	inline v_buffer::buffer_type reduce_duplicate_chars_to_one 
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
		DBJ_PRINT(DBJ_FG_GREEN "\n\nTesting dbj::nanolib::vector usage with standard algorithms\n\n" DBJ_RESET);

		DBJ_TX(
			reduce_duplicate_chars_to_one(
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

