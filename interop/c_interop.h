#ifndef _INC_C_INTEROP_
#define _INC_C_INTEROP_

/*
The whole of valstat C interop is two macros bellow
*/
#define valstat_interop_( T ) typedef struct interop_valstat_##T \
{ \
T * value ; \
const char * status  ; \
} interop_valstat_##T
#define valstat_interop( T ) valstat_interop_( T )

#define valstat_interop_decl_( T ) interop_valstat_##T 
#define valstat_interop_decl( T ) valstat_interop_decl_(T) 

/*
what follows is valstat interop testing C side
*/
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	typedef enum { EMPTY = 0, OK, ERR, INFO } valstat_kind;

	valstat_interop(int);

	valstat_interop_decl(int) interop_valstat_testing( valstat_kind  );

#ifdef __cplusplus
} // "C"
#endif // __cplusplus

#ifdef __cplusplus
/*
--------------------------------------------------------------------------
it is not extremely good, mixing pure C++ in C header. We do it like this
to have all the relevant stuff close and easily observable in this 
one short header
*/
#include "../dbj--nanolib/dbj++tu.h"
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
#endif // __cplusplus

#endif // _INC_C_INTEROP_

