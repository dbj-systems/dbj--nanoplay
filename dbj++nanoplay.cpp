// (c) 2019 by dbj.org -- CC BY-SA 4.0 -- https://creativecommons.org/licenses/by-sa/4.0/ 

#define TESTING_DBJ_RETVALS

#include "dbj--nanolib/dbj++valstat.h"
#include "no_macros_valstat.h"
#include "valstat_monster/valstat_optiref.h"

#include "valstat_monster/c_interop.h"

extern"C" {
	valstat_interop_decl(int) interop_valstat_testing( valstat_kind );
}

#include <iostream>

static void consuming_interop_valstat() {

	using namespace std; 
	cout << boolalpha << endl << "Interop valstat returned:"  ;

	auto [value, status ] = interop_valstat_testing( valstat_kind::INFO );

	cout << "\nvalue: ";
		if (value) 
			cout << *value ;
		else
			cout << "{ empty }";

		cout << "\nstatus: ";
		if (status)
			cout << status;
		else
			cout << "{ empty }";

		cout << endl ;
}

int main(const int, const char* [])
{
	DBJ_PRINT(DBJ_FG_CYAN  "\n\ndbj++nanolib playground\n\n" DBJ_RESET);

	consuming_interop_valstat();
	// call the test units registered
	// in random order
	// easiest is to place the break point in the test unit of interest
	// dbj--nanolib is a submodule here with normal header files inside
	// dbj::tu::catalog.execute();

	return 42;
}

