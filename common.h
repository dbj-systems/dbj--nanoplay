#ifndef _DBJ_INC_COMMON_
#define _DBJ_INC_COMMON_

/* (c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/ */

#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

// #pragma clang diagnostic push

#ifdef __clang__
#pragma clang system_header
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wpragma-once-outside-header"
#endif // __clang__


// MSVC STD LIB aka STL
// _ITERATOR_DEBUG_LEVEL 2 is the DEBUG build default
// and that will produce segv with dbj stack alocator 
// that was not tested with stack allocator I suppose?
// https://docs.microsoft.com/en-us/cpp/standard-library/iterator-debug-level?view=vs-2019

#if (_ITERATOR_DEBUG_LEVEL != 0)
#undef _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL  0
#endif

// #define TESTING_DBJ_RETVALS
#include <crtdbg.h>
#include <cassert>
#include <future>
#include <iomanip>
#include <random>
#include <thread>


/// NOTE: among other things this ensures synchronized output
///       unless one uses naked <stdio.h> or god forbid <iostream>
#define DBJ_NANO_LIB_MT
#define NANO_PRINTF_IN_USE

#if 0
// iuse it as local, for when developing
#include "../dbj--nanolib/dbj++tu.h"
#else
// use it as a submodule
#include "dbj-nanotest/dbj++tu.h"
#endif

#include "dbj--simplelog/dbj_simple_log.h"
// inline int dbj_simple_log_startup(const char* app_full_path)


namespace testing_space {

/// #undef driver 

	template< typename FP>
	inline void driver(
		FP function_,
		char const* prompt_ = nullptr,
		bool testing_c_interop = false)
	{
		using namespace std;

		if (prompt_) DBJ_PRINT(prompt_);

		DBJ_PRINT("%s%s%s", DBJ_FG_CYAN, "valstat:", DBJ_RESET);

		// structured binding of a result from a C function
		auto [value, status] = function_();

		DBJ_PRINT("value: ");
		if (value)
			DBJ_PRINT("%s%s%s", DBJ_FG_CYAN_BOLD, *value, DBJ_RESET);
		else
			DBJ_PRINT("%s%s%s", DBJ_FG_CYAN_BOLD, "{ empty }", DBJ_RESET);

		DBJ_PRINT(" / status:") ;
		if (status)
			if (false == testing_c_interop)
				DBJ_PRINT("%s%s%s", DBJ_FG_RED_BOLD, *status, DBJ_RESET);
			else
				DBJ_PRINT("%s%s%s", DBJ_FG_RED_BOLD, status, DBJ_RESET);
		else
			DBJ_PRINT("%s%s%s", DBJ_FG_CYAN_BOLD, "{ empty }", DBJ_RESET);

	}
} // interop_testing_space

#endif // _DBJ_INC_COMMON_
