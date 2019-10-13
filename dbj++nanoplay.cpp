//

#define TESTING_DBJ_RETVALS

#include "dbj--nanolib/dbj++valstat.h"

int main()
{
	DBJ_PRINT( DBJ_FG_CYAN  "\n\ndbj++nanolib playground\n\n" DBJ_RESET );

	// call the test units registered
	// in random order
	// easiest is to place the break point in the test unit of interest
	dbj::tu::catalog.execute();
}

