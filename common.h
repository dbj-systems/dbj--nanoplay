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

// submodules
#include "dbj--nanolib/dbj++nanolib.h"
#include "dbj--nanolib/dbj_typename.h" // DBJ_SXT , DBJ_SX , DBJ_NANO_FREE
#include "dbj--nanolib/dbj_heap_alloc.h"
#include "dbj-nanotest/dbj++tu.h"
#ifdef DBJ_USING_SIMPLE_LOG
#include "dbj--simplelog/dbj_simple_log.h"
#endif
// inline int dbj_simple_log_startup(const char* app_full_path)


namespace eastl {
	extern "C" {
		/////////////////////////////////////////////////////////////////////////////
		// EASTL expects us to define these, see allocator.h 
		inline void* user_defined_alloc(
			size_t size_,
			const char* pName,
			int flags,
			unsigned debugFlags,
			const char* file,
			int line
		) noexcept
		{
			// note: no checks whatsoever ...
			return DBJ_NANO_MALLOC(void, size_);
		}

		// alligned allocation
		inline void* user_defined_alloc_aligned(
			size_t size_,
			size_t alignment_,
			size_t alignmentOffset,
			const char* pName,
			int flags,
			unsigned debugFlags,
			const char* file,
			int line
		) noexcept
		{
			// this allocator doesn't support alignment
			EASTL_ASSERT(alignment_ <= 8);
			// note: no checks whatsoever ...
			return DBJ_NANO_MALLOC(void, size_);
		}

		// dbj eastl2010 change to allow for default allocator
		// to use matching de allocation / allocation
		// just like in this case
		inline void user_defined_deallocate(void* ptr_) noexcept
		{
			DBJ_NANO_FREE(ptr_);
		}
	} // "C"
} // eastl ns


// EASTL also wants us to define this (see string.h line 197)
extern "C" inline int Vsnprintf8(char* pDestination, size_t count_,
	const /*char8_t*/ char * pFormat, va_list arguments) {
#ifdef _MSC_VER
	// return _vsnprintf(pDestination, count_ , pFormat, arguments);
	return _vsnprintf_s(pDestination, strlen(pDestination), count_, pFormat, arguments);
#else
	return vsnprintf(pDestination, count_, pFormat, arguments);
#endif
}

namespace dbj::nanolib::ostrmng
{
	inline std::ostream& operator<<(std::ostream& os, const eastl::string & str_)
	{
		return os << str_.c_str();
	}
}

//
// back to dbj
//
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

		DBJ_PRINT(" / status:");
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
