// (c) 2019 by dbj.org -- CC BY-SA 4.0 -- https://creativecommons.org/licenses/by-sa/4.0/ 

#define TESTING_DBJ_RETVALS

#include "dbj--nanolib/dbj++valstat.h"
#include "no_macros_valstat.h"

int main( const int, const char * [] )
{
	DBJ_PRINT( DBJ_FG_CYAN  "\n\ndbj++nanolib playground\n\n" DBJ_RESET );

	// call the test units registered
	// in random order
	// easiest is to place the break point in the test unit of interest
	// dbj--nanolib is a submodule here with normal header files inside
	dbj::tu::catalog.execute();

	return 42;
}

