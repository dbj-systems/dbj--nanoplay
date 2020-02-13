#ifndef _DBJ_INC_COMMON_
#define _DBJ_INC_COMMON_

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
Why was this used?
#if (WINVER < NTDDI_WIN10_RS3)
#else
#error dbj++ requires Windows builds above REDSTONE 3 or above
#endif
*/


#include <future>
#include <iomanip>

#if 0
// iuse it as local, for when developing
#include "../dbj--nanolib/dbj++valstat.h"
#include "../dbj--nanolib/dbj++tu.h"
#else
// use it as a submodule
#include "dbj--nanolib/dbj++valstat.h"
#include "dbj--nanolib/jzon/jzon.h"
#include "dbj--nanolib/jzon/jzon_utils.h"
#include "dbj--nanolib/dbj++tu.h"
#endif

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
		namespace dbjlog = dbj::nanolib::logging ;

		if (prompt_) dbjlog::log(prompt_);

		dbjlog::log(DBJ_FG_CYAN, "valstat:", DBJ_RESET);

		// structured binding of a result from a C function
		auto [value, status] = function_();

		dbjlog::log("value: ");
		if (value)
			dbjlog::log(DBJ_FG_CYAN_BOLD, *value, DBJ_RESET);
		else
			dbjlog::log(DBJ_FG_CYAN_BOLD, "{ empty }", DBJ_RESET);

		dbjlog::log(" / status:") ;
		if (status)
			if (false == testing_c_interop)
				dbjlog::log(DBJ_FG_RED_BOLD, *status, DBJ_RESET);
			else
				dbjlog::log(DBJ_FG_RED_BOLD, status, DBJ_RESET);
		else
			dbjlog::log(DBJ_FG_CYAN_BOLD, "{ empty }", DBJ_RESET);

	}
} // interop_testing_space

#endif // _DBJ_INC_COMMON_
