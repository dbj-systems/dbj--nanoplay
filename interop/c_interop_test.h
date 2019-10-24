#ifndef _INC_C_INTEROP_TESTING_
#define _INC_C_INTEROP_TESTING_

#include "c_interop.h"
#include "../common.h"
#include <iostream>

namespace interop_testing_space {
	using namespace std;

	inline void consuming_interop_valstat( ) {

		auto driver = [](valstat_kind kind_) {
			cout << boolalpha << endl << DBJ_FG_CYAN << "Interop valstat returned:" << DBJ_RESET ;

			// structured binding of a result from a C function
			auto [value, status] = interop_valstat_testing(kind_);

			cout << "\nvalue: ";
			if (value)
				cout << *value;
			else
				cout << DBJ_FG_CYAN_BOLD << "{ empty }" << DBJ_RESET ;

			cout << "\nstatus: ";
			if (status)
				cout << status;
			else
				cout << DBJ_FG_CYAN_BOLD << "{ empty }" << DBJ_RESET ;

			cout << endl;
		};

		cout << endl << "Testing EMPTY valstat interop";
		driver( valstat_kind::EMPTY );
		cout << endl << "Testing ERR valstat interop";
		driver( valstat_kind::ERR );
		cout << endl << "Testing INFO valstat interop";
		driver( valstat_kind::INFO );
		cout << endl << "Testing OK valstat interop";
		driver( valstat_kind::OK );
	}

	// TESTING
	TU_REGISTER([] {
		consuming_interop_valstat();
	});
}

#endif // _INC_C_INTEROP_TESTING_

