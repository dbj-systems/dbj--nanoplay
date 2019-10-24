// (c) 2019 by dbj.org -- CC BY-SA 4.0 -- https://creativecommons.org/licenses/by-sa/4.0/ 

// #define TESTING_DBJ_RETVALS

#include "common.h"
#if _DBJ_SAMPLING
#include "sampling/testing_dbj_vector.h"
#include "sampling/no_macros_valstat.h"
#include "sampling/win/logfile.h"
#endif
#include "interop/c_interop_test.h"
#include "valstat_2/valstat_2_samples.h"

#include <future>
#pragma warning( push )
#pragma warning( disable: 4100 )
// https://msdn.microsoft.com/en-us/library/26kb9fy0.aspx 

// just execute all the registered tests
// in no particular order
static void dbj_program_start(
	const	int			argc,
	const	wchar_t* argv[],
	const	wchar_t* envp[]
)
{
	DBJ_PRINT(DBJ_FG_CYAN  "\n\ndbj++nanolib playground: %S\n\n" DBJ_RESET, argv[0]);
	// call the test units registered, in random order
	// in this scenario easiest is to place the break point 
	// in the test unit of interest
	dbj::tu::catalog.execute();
}

#pragma warning( pop ) // 4100

#if defined(UNICODE) || defined(_UNICODE)
int wmain(const int argc, const wchar_t* argv[], const wchar_t* envp[])
#else
#error "What could be the [t]reason this is not an UNICODE build?"
int main(int argc, char* argv[], char* envp[])
#endif
{
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



