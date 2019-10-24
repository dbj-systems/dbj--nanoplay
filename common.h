#ifndef _DBJ_INC_COMMON_
#define _DBJ_INC_COMMON_


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

#include "../dbj--nanolib/dbj++valstat.h"
#include "../dbj--nanolib/dbj++tu.h"

#endif // _DBJ_INC_COMMON_
