
/* (c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/ */

// this will be also used in mini dump creation
#define DBJ_APP_NAME "dbj++nanoplay"
#define DBJ_APP_VERSION_STR "1.2.0"

#pragma region generate mini dump
/**************************************************************************************

Taken from: https://docs.microsoft.com/en-us/windows/win32/dxtecharts/crash-dump-analysis

NOTE: SEH routines are intrinsics to the cl.exe
Usage:

inline void SomeFunction()
{
	__try
	{
		int *pBadPtr = NULL;
		*pBadPtr = 0;
	}
	__except(GenerateDump(GetExceptionInformation()))
	{
	}
}
*/

#include <windows.h>

#include <assert.h>
#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#include <strsafe.h>

//    StringCchPrintfA( szFileName, MAX_PATH, "%s%s", szPath, szAppName );
#undef  win32_sprintf
#define win32_sprintf StringCchPrintfA

// NOTE: this is linkging it also in release builds ...
// since this is a "playground" app this is OK
#pragma comment(lib, "Dbghelp.lib")

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	typedef struct generate_dump_last_run {
		char  dump_folder_name[MAX_PATH];
		char  dump_file_name[MAX_PATH];
		BOOL finished_ok;
	} generate_dump_last_run;

	static generate_dump_last_run dump_last_run;
	static generate_dump_last_run dump_last_run_clean = { {0}, {0}, FALSE };

	/*
	DBJ: warning, currently this is a blocking call to an expensive function
	     which is fine since this gets called from inside SEH handlers only
	*/
	static inline int GenerateDump(EXCEPTION_POINTERS* pExceptionPointers)
	{
		dump_last_run = dump_last_run_clean;

		char  szPath[MAX_PATH] = { 0 };
		char  szFileName[MAX_PATH] = { 0 };
		const char* szAppName = DBJ_APP_NAME;
		const char* szVersion = DBJ_APP_VERSION_STR ;
		DWORD dwBufferSize = MAX_PATH;
		HANDLE hDumpFile = 0;
		SYSTEMTIME stLocalTime;
		MINIDUMP_EXCEPTION_INFORMATION ExpParam;

		GetLocalTime(&stLocalTime);
		GetTempPathA(dwBufferSize, szPath);

		win32_sprintf(szFileName, MAX_PATH, "%s%s", szPath, szAppName);
		CreateDirectoryA(szFileName, NULL);

		win32_sprintf(dump_last_run.dump_folder_name, MAX_PATH, "%s", szFileName);

		win32_sprintf(szFileName, MAX_PATH, "%s%s\\%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp",
			szPath, szAppName, szVersion,
			stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
			stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
			GetCurrentProcessId(), GetCurrentThreadId());

		hDumpFile = CreateFileA(szFileName, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

		win32_sprintf(dump_last_run.dump_file_name, MAX_PATH, "%s", szFileName);

		ExpParam.ThreadId = GetCurrentThreadId();
		ExpParam.ExceptionPointers = pExceptionPointers;
		ExpParam.ClientPointers = TRUE;

		BOOL bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);

		dump_last_run.finished_ok = bMiniDumpSuccessful;
		return EXCEPTION_EXECUTE_HANDLER;
	}

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus

#pragma endregion

#define DBJ_USING_ADHOC_AND_TEMPORARY

#pragma region includes of sampling and test units

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

// eof includes of sampling and test units
#pragma endregion 

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
	// in this scenario for debugging, easiest is to place the break point 
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

/*
this is the dbj standard main

it is very narrow in scope, it covers windows build only
it commpiles in /kernel builds where there is no C++ exceptions
just sa called Structured Exception Handling (SEH)
Microsoft extension to C, also with intrinsics built into cl.exe
And in case of SE being csaught mini dump is created
You open a minidump in Visual Studio and you do the native debugging
from there
*/
int main(int argc, char* argv[],  char* envp[])
{
	__try
	{
#ifdef DBJ_USING_SIMPLE_LOG
	dbj_simple_log_startup(argv[0]);
#endif

	dbj_program_start(argc, argv, envp);
	}
	__except (
		GenerateDump(GetExceptionInformation())
		/* returns 1 aka EXCEPTION_EXECUTE_HANDLER */
		)
	{
		puts( argv[0] );
		perror("SEH Exception caught");

		puts(dump_last_run.finished_ok == TRUE ? "minidump creation succeeded" : "minidump creation failed");
		if (dump_last_run.finished_ok) {
			puts(dump_last_run.dump_folder_name);
			puts(dump_last_run.dump_file_name);
		}
	}

	// flush all the std streams
	fprintf(stderr, "\n");
	fprintf(stdout, "\n");
	return EXIT_SUCCESS ;
}

#ifdef DBJ_USING_ADHOC_AND_TEMPORARY

/// ----------------------------------------------------------------------
// add remove here ad-hoc testing sampling

extern "C" {
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


// this is a special TU where add hoc tests are called from
// DBJ: think of placing a __try / __exception in here
// with minidump creation

TU_REGISTER([]
{
		DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE DBJ_RESET);
		DBJ_PRINT(DBJ_FG_CYAN_BOLD "\nAd-Hoc Sampling is grouped here. " DBJ_RESET);

			// std::this_thread::sleep_for(0s);

			auto argc_ = __argc;
			auto argv_ = __argv;

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
});

#endif // DBJ_USING_ADHOC_AND_TEMPORARY

