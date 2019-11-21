#ifndef _INC_C_INTEROP_TESTING_
#define _INC_C_INTEROP_TESTING_

#include "c_interop.h"
#include "../../common.h"
#include "../valstat_2.h"
#include <iostream>

namespace interop_testing_space {
	using namespace std;
	using namespace  valstat_testing_space;

	inline void consuming_interop_valstat( ) {
		driver([] {return interop_valstat_testing(valstat_kind::EMPTY); }, "Testing EMPTY valstat interop");
		driver([] {return interop_valstat_testing(valstat_kind::ERR); }, "Testing ERR valstat interop");
		driver([] {return interop_valstat_testing(valstat_kind::INFO); }, "Testing INFO valstat interop");
		driver([] {return interop_valstat_testing(valstat_kind::OK); }, "Testing OK valstat interop");
	}

	// TESTING
	TU_REGISTER([] {
		consuming_interop_valstat();

		cout << endl << endl << " valstat_interop_decl(uint64_t) divider(uint64_t dividend, uint64_t divisor) ";
		driver([] { return divider(1,0); }, "calling divider(1,0);");
		driver([] { return divider(1,1); }, "calling divider(1,1);");
		driver([] { return divider(0,1); }, "calling divider(0,1);");
		});
}

#endif // _INC_C_INTEROP_TESTING_

