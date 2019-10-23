// (c) 2019 by dbj.org -- CC BY-SA 4.0 -- https://creativecommons.org/licenses/by-sa/4.0/ 

#define TESTING_DBJ_RETVALS

#include "dbj--nanolib/dbj++valstat.h"
#include "no_macros_valstat.h"
#include "valstat_optiref/valstat_optiref.h"

#include "interop/c_interop.h"
#include "win/logfile.h"
#include "valstat_2/valstat_2.h"


int main(const int, const char* argv [])
{
	auto [val,stat]  = dbj::win::log_file_path(argv[0]);

	DBJ_PRINT(DBJ_FG_CYAN  "\n\ndbj++nanolib playground\n\n" DBJ_RESET);

	if ( val )
		DBJ_PRINT(DBJ_FG_CYAN  "\n\nlog file path: %s\n\n" DBJ_RESET, val->data() );

	// call the test units registered, in random order
	// in this scenario easiest is to place the break point 
	// in the test unit of interest
	dbj::tu::catalog.execute();

	return 42;
}

