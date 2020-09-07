/* (c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/ */

#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "common.h"

#include "sampling/keep_it_sorted.h"
#include "sampling/any_opty.h"
#ifdef DBJ_HAS_CXX20
// #include "sampling/cpp20.h"
// #include "utf8/utf8_decoder_sampler.h"
#endif
#include "utf8/utf8_dbj_nano_services.h"
#include "utf8/utf8_kilim.h"
#include "lambdatix/cpp_lambda_mx_makers.h"
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
	const	char* argv[],
	const	char* envp[]
)
{
	DBJ_PRINT("dbj++nanolib version: %s", dbj::nanolib::VERSION);
	DBJ_PRINT(DBJ_FG_CYAN  "\ndbj++nanolib playground version:[" __TIMESTAMP__ "]\n");
	// call the test units registered, in random order
	// in this scenario easiest is to place the break point 
	// in the test unit of interest
	// if argumet is true, only tu's listing will be shown
	dbj::tu::testing_system::execute(
		/*true*/
	);
#ifndef NDEBUG
	::system("@pause");
#endif

}

#pragma warning( pop ) // 4100

/// ----------------------------------------------------------------------
#ifdef DBJ_USING_SIMPLE_LOG
struct dbj_simplelog_finalizer final {
	~dbj_simplelog_finalizer() {
		dbj_log_finalize();
	}
};
static dbj_simplelog_finalizer dsf_;
#endif // DBJ_USING_SIMPLE_LOG

/// ----------------------------------------------------------------------
// static void ad_hoc_and_temporary(int argc, const char* argv[], const char* envp[]);
/// ----------------------------------------------------------------------
int main(int argc, const char* argv[], const char* envp[])
{
#ifdef DBJ_USING_SIMPLE_LOG
	dbj_simple_log_startup(argv[0]);
#endif

	// ad_hoc_and_temporary(argc, argv, envp);

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

#ifndef _KERNEL_MODE
		try {
#endif
			dbj_program_start(argc, argv, envp);
#ifndef _KERNEL_MODE
		}
		catch (...) {
			DBJ_PRINT(DBJ_FG_RED_BOLD "\n\n" __FILE__ "\n\nUnknown exception!\n\n" DBJ_RESET);
		}
#endif

	exit(EXIT_SUCCESS);
}

/// ----------------------------------------------------------------------
// add remove here ad-hoc testing sampling

extern "C" {
	void test_clang_vs_setup();
	void dbj_mx_sampling(unsigned, unsigned);
	void dbj_mx_2_sampling(unsigned, unsigned);

	int dbj_matrix_struct_test();

	int dbj_string_storage_test(const int argc, const char** argv);

	int compare_all_allocation_methods(int argc, const char** argv);

} // "C

void aligned_allocation_test();
int recursive_lambada();

// #include "./to_be_decided/win32_swprintf_conversion_attempt.h"

static void ad_hoc_and_temporary(int argc, const char* argv[], const char* envp[])
{
	compare_all_allocation_methods(argc, argv);

	dbj_string_storage_test( argc, argv);

	recursive_lambada();

	aligned_allocation_test();

	dbj_matrix_struct_test();

	test_clang_vs_setup();
#ifdef TEST_WCONVERTER_WSTRING_INC
	test_wconverter_wstring();
#endif // TEST_WCONVERTER_WSTRING_INC
	dbj_mx_2_sampling(2, 2);
	dbj_mx_sampling(2, 2);
}



