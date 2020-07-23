// (c) 2019 by dbj.org -- CC BY-SA 4.0 -- https://creativecommons.org/licenses/by-sa/4.0/ 

// #define TESTING_DBJ_RETVALS
#include <crtdbg.h>

#include "common.h"

//#include "sampling/keep_it_sorted.h"
//#include "sampling/any_opty.h"
#ifdef DBJ_HAS_CXX20
// #include "sampling/cpp20.h"
// #include "utf8/utf8_decoder_sampler.h"
#endif
//#include "utf8/utf8_kilim.h"
//#include "lambdatix/lambda_mx_makers.h"
//#include "sampling/dbj_meta_converter.h"
//#include "valstat_research/fibo.h"
//#include "valstat_research/polygon.h"
//#include "valstat_research/valstat_dbj_async.h"
//#include "valstat_research/valstat_dbj_own.h"
//#include "sampling/find_wovels.h"

#if 0
#include "valstat_research/valstat_dbj_samples.h"
#include "sampling/testing_dbj_vector.h"
#include "sampling/no_macros_valstat.h"
#include "sampling/fmt.h"
#include "sampling/win/logfile.h"
#include "lambdatix/narf_again.h"
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
	dbj::tu::testing_system::execute( 
	/*true*/ 
	);
	dbj::nanolib::system_call("pause");
}

#pragma warning( pop ) // 4100

/// ----------------------------------------------------------------------
void ad_hoc_and_temporary(int argc, const char* argv[], const char* envp[]);
/// ----------------------------------------------------------------------
int main(int argc, const char* argv[], const char* envp[])
{

	ad_hoc_and_temporary( argc,argv,envp );

#ifdef DBJ_REDIRECT_STD_IN
	if (freopen("input.txt", "r", stdin) == NULL) {
		// Handle error, errno is set to indicate error
	}
#endif

#undef  DBJ_REDIRECT_STD_ERR
#ifdef DBJ_REDIRECT_STD_ERR
	std::string logfile_name(argv[0]);
	logfile_name.append(".log");

#ifndef NDEBUG
	DBJ_PRINT("local log file: %s", logfile_name.c_str());
#endif
	// https://stackoverflow.com/a/46869216/10870835

	bool revert_on_destruct_ = false;
	dbj::redirector redirect_{ revert_on_destruct_, logfile_name.c_str() };

#endif // DBJ_REDIRECT_STD_ERR

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

/// ----------------------------------------------------------------------
// add remove here ad-hoc testing sampling

extern "C" {
#ifdef __clang__ // using C99 VLA 
	void dbj_mx_sampling( unsigned , unsigned);
#endif // __clang__
	void dbj_mx_2_sampling(unsigned , unsigned);
} // "C

void ad_hoc_and_temporary(int argc, const char* argv[], const char* envp[])
{
	dbj_mx_2_sampling(2, 2);
#ifdef __clang__ // using C99 VLA 
	dbj_mx_sampling(2,2);
#endif // __clang__
}



