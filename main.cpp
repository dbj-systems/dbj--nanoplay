// (c) 2019 by dbj.org -- CC BY-SA 4.0 -- https://creativecommons.org/licenses/by-sa/4.0/ 

#include "common.h"

#include "sampling/keep_it_sorted.h"
#include "sampling/any_opty.h"
#ifdef DBJ_HAS_CXX20
// #include "sampling/cpp20.h"
// #include "utf8/utf8_decoder_sampler.h"
#endif
#include "utf8/utf8_kilim.h"
#include "lambdatix/lambda_mx_makers.h"
#include "sampling/dbj_meta_converter.h"
#include "valstat_research/fibo.h"
#include "valstat_research/polygon.h"
#include "valstat_research/valstat_dbj_async.h"
#include "valstat_research/valstat_dbj_own.h"
#include "sampling/find_wovels.h"

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
static void ad_hoc_and_temporary(int argc, const char* argv[], const char* envp[]);
/// ----------------------------------------------------------------------
int main(int argc, const char* argv[], const char* envp[])
{
	ad_hoc_and_temporary( argc,argv,envp );

#ifdef DBJ_REDIRECT_STD_IN
	if (freopen("input.txt", "r", stdin) == NULL) {
		perror("freopen(\"input.txt\", \"r\", stdin) failed...");
		exit(errno);
	}
#endif

#ifdef NDEBUG
#undef  DBJ_REDIRECT_STD_ERR
#endif

#ifdef DBJ_REDIRECT_STD_ERR
	using dbj::nanolib::v_buffer;
	using buff_t = v_buffer::buffer_type;
	buff_t logfile_name = v_buffer::format("%s.log", argv[0]);
#ifndef NDEBUG
	DBJ_PRINT("local log file: %s", logfile_name.data());
#endif
	// https://stackoverflow.com/a/46869216/10870835
	dbj::redirector redirect_{ /*revert_on_destruct_*/ false , logfile_name.data() };
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
	void test_clang_vs_setup();
	void dbj_mx_sampling(unsigned, unsigned);
  void dbj_mx_2_sampling(unsigned , unsigned);
} // "C

// #include "./to_be_decided/win32_swprintf_conversion_attempt.h"

static void ad_hoc_and_temporary(int argc, const char* argv[], const char* envp[])
{
	test_clang_vs_setup();
#ifdef TEST_WCONVERTER_WSTRING_INC
	test_wconverter_wstring();
#endif // TEST_WCONVERTER_WSTRING_INC
	dbj_mx_2_sampling(2, 2);
	dbj_mx_sampling(2,2);
}



