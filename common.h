#ifndef _DBJ_INC_COMMON_
#define _DBJ_INC_COMMON_

/* (c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/ */

/// NOTE: among other things this ensures synchronized output
///       unless one uses naked <stdio.h> or god forbid <iostream>
#define DBJ_NANO_LIB_MT

// submodules
#include "dbj--nanolib/dbj++nanolib.h"
#include "dbj--nanolib/dbj_typename.h" // DBJ_SXT , DBJ_SX , DBJ_NANO_FREE
#include "dbj--nanolib/dbj_eastl_inclusor.h"    // includes dbj_heap_alloc.h
#include "dbj-nanotest/dbj++tu.h"
#ifdef DBJ_USING_SIMPLE_LOG
#include "dbj--simplelog/dbj_simple_log.h"
#endif

// TBD
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
