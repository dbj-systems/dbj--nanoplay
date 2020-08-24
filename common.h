#ifndef _DBJ_INC_COMMON_
#define _DBJ_INC_COMMON_

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

/*
assumption is calloc, malloc, free are the best possible implementations 
for a given platform
*/
// https://vorpus.org/blog/why-does-calloc-exist/
//
//#undef DBJ_ALLOC
//#define DBJ_ALLOC(T_,N_,S_) (T_*)calloc(N_,S_)
//
//#undef DBJ_FREE
//#define DBJ_FREE(P_) do { assert(P_ != nullptr ); if(P_ != nullptr) free(P_); P_ = nullptr; } while(0)

/*
Why was this used?
#if (WINVER < NTDDI_WIN10_RS3)
#else
#error dbj++ requires Windows builds above REDSTONE 3 or above
#endif
*/

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
#include "../dbj--nanolib/dbj++valstat.h"
#include "../dbj--nanolib/dbj++tu.h"
#else
// use it as a submodule
#include "dbj--nanolib/dbj++valstat.h"
#include "dbj-nanotest/dbj++tu.h"
#endif

#include "dbj--simplelog/dbj_simple_log.h"
// inline int dbj_simple_log_startup(const char* app_full_path)

/*
rudimentary runtime version checks
https://docs.microsoft.com/en-us/windows/desktop/sysinfo/getting-the-system-version
*/
#include <VersionHelpers.h>

namespace testing_space {

/// #undef driver 

	template< typename FP>
	inline void driver(
		FP function_,
		char const* prompt_ = nullptr,
		bool testing_c_interop = false)
	{
		using namespace std;

		if (prompt_) log_trace(prompt_);

		log_trace(DBJ_FG_CYAN, "valstat:", DBJ_RESET);

		// structured binding of a result from a C function
		auto [value, status] = function_();

		log_trace("value: ");
		if (value)
			log_trace(DBJ_FG_CYAN_BOLD, *value, DBJ_RESET);
		else
			log_trace(DBJ_FG_CYAN_BOLD, "{ empty }", DBJ_RESET);

		log_trace(" / status:") ;
		if (status)
			if (false == testing_c_interop)
				log_trace(DBJ_FG_RED_BOLD, *status, DBJ_RESET);
			else
				log_trace(DBJ_FG_RED_BOLD, status, DBJ_RESET);
		else
			log_trace(DBJ_FG_CYAN_BOLD, "{ empty }", DBJ_RESET);

	}
} // interop_testing_space

#endif // _DBJ_INC_COMMON_
