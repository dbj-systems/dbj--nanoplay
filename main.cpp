/* (c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/ */

#define DBJ_USING_ADHOC_AND_TEMPORARY

#include "sampling/keep_it_sorted.h"
#include "sampling/any_opty.h"
#ifdef DBJ_HAS_CXX20
// #include "sampling/cpp20.h"
// #include "utf8/utf8_decoder_sampler.h"
#endif
#include "utf8/utf8_dbj_nano_services.h"
#include "utf8/utf8_kilim.h"

#define DBJ_META_CONVERTER_CANONICAL_TESTS 
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
#endif

#pragma warning( push )
#pragma warning( disable: 4100 )
// https://msdn.microsoft.com/en-us/library/26kb9fy0.aspx 

// just execute all the registered tests
// in no particular order
static void dbj_program_start(
	int	   argc,
	char* argv[],
	char* envp[]
)
{
	DBJ_PRINT("dbj++nanolib version: %s", dbj::nanolib::VERSION);
	DBJ_PRINT(DBJ_FG_CYAN  "\ndbj++nanolib playground version:[" __TIMESTAMP__ "]\n");
	// call the test units registered, in random order
	// in this scenario easiest is to place the break point 
	// in the test unit of interest
	// if argumet is true, only tu's listing will be shown
	dbj::tu::tu_catalog().execute(
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

#ifdef _KERNEL_MODE
#define KERNEL_TRY 
#define KERNEL_CATCH
#else   // ! _KERNEL_MODE
#define KERNEL_TRY try {
#define KERNEL_CATCH } catch (...) { DBJ_PRINT(DBJ_FG_RED_BOLD "\n\n" __FILE__ "\n\nUnknown exception!\n\n" DBJ_RESET);}
#endif  // ! _KERNEL_MODE

extern "C" int test_optparse(int argc, char** argv);
/// ----------------------------------------------------------------------
int main(int argc, char* argv[],  char* envp[])
{
	return test_optparse( argc, argv);
#ifdef DBJ_USING_SIMPLE_LOG
	dbj_simple_log_startup(argv[0]);
#endif

	KERNEL_TRY
	dbj_program_start(argc, argv, envp);
	KERNEL_CATCH

	fprintf(stderr, "\n");
	fprintf(stdout, "\n");
	return EXIT_SUCCESS ;
}

#ifdef DBJ_USING_ADHOC_AND_TEMPORARY

/// ----------------------------------------------------------------------
// add remove here ad-hoc testing sampling

extern "C" {
	int test_optparse(int argc, char** argv);
	void arr_struct_simple_sample(void);
	void test_various_matmuls(void);
	//void test_clang_vs_setup();
	//void dbj_mx_sampling(unsigned, unsigned);
	//void dbj_mx_2_sampling(unsigned, unsigned);
	//int dbj_matrix_struct_test();
	//int dbj_string_storage_test(const int argc, const char** argv);
	//int compare_all_allocation_methods(int argc, const char** argv);

	// int recursive_lambada();

} // "C


// #include "./to_be_decided/win32_swprintf_conversion_attempt.h"

TU_REGISTER([]
	{
		DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE DBJ_RESET);
		DBJ_PRINT(DBJ_FG_CYAN_BOLD "\nAd-Hoc Sampling is grouped here. " DBJ_RESET);

		KERNEL_TRY

			// std::this_thread::sleep_for(0s);

			auto argc_ = __argc;
			auto argv_ = __argv;

			test_optparse(argc_, argv_);
			arr_struct_simple_sample();
			// matmuls();
			//	compare_all_allocation_methods(argc, argv);
			//	dbj_string_storage_test(argc, argv);
			//	recursive_lambada();
			//	aligned_allocation_test();
			//	dbj_matrix_struct_test();
			//	test_clang_vs_setup();
			//#ifdef TEST_WCONVERTER_WSTRING_INC
			//	test_wconverter_wstring();
			//#endif // TEST_WCONVERTER_WSTRING_INC
			//	dbj_mx_2_sampling(2, 2);
			//	dbj_mx_sampling(2, 2);
			KERNEL_CATCH
		});

#endif // DBJ_USING_ADHOC_AND_TEMPORARY


