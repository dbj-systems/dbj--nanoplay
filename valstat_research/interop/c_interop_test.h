#pragma once
#ifndef _INC_C_INTEROP_TESTING_
#define _INC_C_INTEROP_TESTING_

#include "c_interop.h"
#include "math_valstat.h"
#include "../../common.h"
#include "../valstat_dbj.h"
#include <iostream>

namespace interop_testing_space {
	using namespace std;
	using namespace testing_space;

	inline void consuming_interop_valstat( ) {
		driver([] {return interop_valstat_testing(dbj::valstat_kind::EMPTY); }, "Testing EMPTY valstat interop", true ) ;
		driver([] {return interop_valstat_testing(dbj::valstat_kind::ERR); }, "Testing ERR valstat interop", true);
		driver([] {return interop_valstat_testing(dbj::valstat_kind::INFO); }, "Testing INFO valstat interop", true);
		driver([] {return interop_valstat_testing(dbj::valstat_kind::OK); }, "Testing OK valstat interop", true);
	}

	// TESTING
	TU_REGISTER([] {

		consuming_interop_valstat();

		cout << endl << endl << " valstat_interop_decl(uint64_t) divider(uint64_t dividend, uint64_t divisor) ";
		driver([] { return dbj::divider(1,0); }, "calling divider(1,0);", true);
		driver([] { return dbj::divider(1,1); }, "calling divider(1,1);", true);
		driver([] { return dbj::divider(0,1); }, "calling divider(0,1);", true);

		driver([] { return log_valstat(-2.0);   }, "log_valstat(-2.0);"  , true );
		driver([] { return log10_valstat(-5.0); }, "log10_valstat(-5.0);", true );
		driver([] { return log_valstat(0.0);    }, "log_valstat(0.0);"   , true);
	});
}

#endif // _INC_C_INTEROP_TESTING_

