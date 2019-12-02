#ifndef _DBJ_INC_COMMON_
#define _DBJ_INC_COMMON_

#if _MSVC_LANG
/*
include windows only from one place
and do it according to ancient windows lore
*/
#define WIN32_LEAN_AND_MEAN
#define STRICT
#define NOSERVICE
#define NOMINMAX
#include <windows.h>

/*
Why was this used?
#if (WINVER < NTDDI_WIN10_RS3)
#else
#error dbj++ requires Windows builds above REDSTONE 3 or above
#endif
*/

/*
rudimentary runtime version checks
https://docs.microsoft.com/en-us/windows/desktop/sysinfo/getting-the-system-version
*/
#include <VersionHelpers.h>

/*
GDI+  if required that is
*/
#ifdef DBJ_GDI_USER
#ifndef _GDIPLUS_H
// because GDI+ 
// can not cope with
// NOMINMAX
#include <algorithm>
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")
#endif // _GDIPLUS_H
#endif

#endif // _MSVC_LANG

#include <future>

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

#include "dbj--pprintpp/pprintpp.hpp"

namespace testing_space {

#undef driver 

	template< typename FP>
	inline void driver(
		FP function_,
		char const* prompt_ = nullptr,
		bool testing_c_interop = false)
	{
		using namespace std;

		if (prompt_) cout << endl << endl << prompt_ << endl;

		cout << boolalpha << endl << DBJ_FG_CYAN << "valstat:" << DBJ_RESET;

		// structured binding of a result from a C function
		auto [value, status] = function_();

		cout << "\nvalue: ";
		if (value)
			cout << DBJ_FG_CYAN_BOLD << *value << DBJ_RESET;
		else
			cout << DBJ_FG_CYAN_BOLD << "{ empty }" << DBJ_RESET;

		cout << " / status:";
		if (status)
			if (false == testing_c_interop)
				cout << DBJ_FG_RED_BOLD << *status << DBJ_RESET;
			else
				cout << DBJ_FG_RED_BOLD << status << DBJ_RESET;
		else
			cout << DBJ_FG_CYAN_BOLD << "{ empty }" << DBJ_RESET;

		cout << endl;
	}
} // interop_testing_space

#endif // _DBJ_INC_COMMON_
