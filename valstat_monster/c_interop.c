#include <stdlib.h>
#include "c_interop.h"

#define _DBJ_STRINGIZE(x) #x

#define FLT __FILE__ "(" _DBJ_STRINGIZE(__LINE__) ")" __TIMESTAMP__

/*
this is deliberately verbose
*/
valstat_interop_decl(int) interop_valstat_testing(valstat_kind kind_)
{
	// value is static int
	static int fty2 = 42;
	// status is static string
	const char* msg = "What's the meaning of this?";

	// info
	interop_valstat_int vint_info = { &fty2, msg };
	// error
	interop_valstat_int vint_err = { NULL, msg };
	// ok
	interop_valstat_int vint_ok = { &fty2, NULL };
	// empty
	interop_valstat_int vint_empty = { NULL, NULL };

	switch (kind_) {
	case INFO: return vint_info;
	case OK: return vint_ok;
	case ERR: return vint_err;
	}
	return vint_empty;
}


