#include "dbjclib/tests/tests.h"
#include "common.h"

TU_REGISTER(
	[] {
		dbj_string_list_test(stdout);
		dbj_front_back_string_test(stdout);
		dbj_string_trim_test(stdout);
	}
);