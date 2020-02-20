// (c) 2019 by dbj.org -- CC BY-SA 4.0 -- https://creativecommons.org/licenses/by-sa/4.0/ 

// #define TESTING_DBJ_RETVALS
#include <crtdbg.h>

#include "common.h"
#include "sampling/keep_it_sorted.h"
/// #include "jzon_sampling/jzon_udl.h"
#include "jzon_sampling/tests.h"
#include "jzon_sampling/test-boxing.h"
#include "jzon_sampling/json-checker-fail.h"
#include "jzon_sampling/conformance-string.h"
#include "jzon_sampling/conformance-double.h"
#if 0
#include "sampling/any_opty.h"
#include "lambdatix/narf_again.h"
#include "utf8/utf8_kilim.h"
#include "sampling/cpp20.h"
#include "lambdatix/lambda_mx_makers.h"
#include "sampling/dbj_meta_converter.h"
#include "valstat_research/fibo.h"
#include "valstat_research/polygon.h"
#include "valstat_research/valstat_dbj_samples.h"
#include "valstat_research/valstat_dbj_async.h"
#include "valstat_research/valstat_dbj_own.h"
#include "utf8_decoder/utf8_decoder_sampler.h"
#include "sampling/testing_dbj_vector.h"
#include "sampling/no_macros_valstat.h"
#include "sampling/fmt.h"
#include "sampling/win/logfile.h"
#include "sampling/find_wovels.h"
#endif

#pragma warning( push )
#pragma warning( disable: 4100 )
// https://msdn.microsoft.com/en-us/library/26kb9fy0.aspx 

// just execute all the registered tests
// in no particular order
static void dbj_program_start(
	const	int	   argc,
	const	char * argv[],
	const	char * envp[]
)
{
	DBJ_PRINT("dbj++nanolib version: %s" , dbj::nanolib::VERSION  );
	DBJ_PRINT(DBJ_FG_CYAN  "dbj++nanolib playground version:[" __TIMESTAMP__ "]");
	// call the test units registered, in random order
	// in this scenario easiest is to place the break point 
	// in the test unit of interest
	// if argumet is true, only tu's listing will be shown
	dbj::tu::catalog.execute( /*true*/ );
	dbj::nanolib::system_call("pause");
}

#pragma warning( pop ) // 4100

int main(int argc, const char* argv[], const char* envp[])
{

#ifdef DBJ_REDIRECT_STD_IN
	if (freopen("input.txt", "r", stdin) == NULL) {
		// Handle error, errno is set to indicate error
	}
#endif

#ifdef DBJ_REDIRECT_STD_OUT
	std::string logfile_name(argv[0]);
	logfile_name.append(".log");

#ifndef NDEBUG
	DBJ_PRINT("local log file: %s", logfile_name.c_str());
#endif
	// https://stackoverflow.com/a/46869216/10870835
	if (freopen(logfile_name.c_str(), "w", stdout) == NULL) {
		// Handle error, errno is set to indicate error
		perror( "\n\n" __FILE__ "\n\nCould not redirect stdout? " );
		exit( EXIT_FAILURE );
	}
#endif // DBJ_REDIRECT_STD_OUT

	auto main_worker = [&]() {
		try {
			dbj_program_start(argc, argv, envp);
		}
		catch (...) {
			DBJ_PRINT(DBJ_FG_RED_BOLD "\n\n" __FILE__ "\n\nUnknown exception!\n\n" DBJ_RESET);
		}
	};

	(void)std::async(std::launch::async, [&] {
		main_worker();
		});

	exit(EXIT_SUCCESS);
}



