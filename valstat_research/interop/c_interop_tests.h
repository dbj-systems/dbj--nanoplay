#pragma once
#ifndef _INC_C_INTEROP_TESTING_
#define _INC_C_INTEROP_TESTING_

#include "../../common.h"
#include "math_valstat.h"

namespace interop_testing_space {
	using namespace std;
	using namespace testing_space;

	// TESTING
	TU_REGISTER([] {

		driver([] { return dbj::log_valstat(-2.0);   }, "log_valstat(-2.0);"  , true );
		driver([] { return dbj::log10_valstat(-5.0); }, "log10_valstat(-5.0);", true );
		driver([] { return dbj::log_valstat(0.0);    }, "log_valstat(0.0);"   , true);
	});
}

#endif // _INC_C_INTEROP_TESTING_

