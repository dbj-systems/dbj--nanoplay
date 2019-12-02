#include "dbjclib/tests/tests.h"
#include "dbjclib/dbjclib.h"
#include "common.h"

TU_REGISTER(
	[] {

		DBJ_PRINT( DBJ_FG_GREEN "\n\nTesting DBJ CLIB %s" DBJ_RESET, dbj_clib_VERSION);

		dbj_string_list_test(stdout);
		dbj_front_back_string_test(stdout);
		dbj_string_trim_test(stdout);
	}
);