#ifndef _INC_C_INTEROP_TESTING_
#define _INC_C_INTEROP_TESTING_

#include "c_interop.h"
#include "../common.h"
#include <iostream>

namespace interop_testing_space {
	using namespace std;

	auto driver = []( auto function_ , char const* prompt_ = nullptr ) 
	{
		if (prompt_) cout << endl << endl << prompt_ << endl;

		cout << boolalpha << endl << DBJ_FG_CYAN << "valstat:" << DBJ_RESET;

		// structured binding of a result from a C function
		auto [value, status] = function_();

		cout << "\nvalue:\t";
		if (value)
			cout << DBJ_FG_CYAN_BOLD << *value << DBJ_RESET ;
		else
			cout << DBJ_FG_CYAN_BOLD << "{ empty }" << DBJ_RESET;

		cout << " / status:";
		if (status)
			cout << DBJ_FG_RED_BOLD << status << DBJ_RESET ;
		else
			cout << DBJ_FG_CYAN_BOLD << "{ empty }" << DBJ_RESET;

		cout << endl;
	};

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

