
// #include "./dbj--nanolib/win32/win32_error_msg_box_exit.h"
#include "D:\DEVL\GitHub\dbj++nanoplay\dbj--nanolib\win32\win32_error_msg_box_exit.h"

#include "../common.h"

#if DBJ_HAS_CXX20

// using CL and having extern "C" is largely (or completely) undocumented
// as an example error C7555 (see bellow) does not happen in *.c files
// when using CL
extern "C"
{
	static void sample_dbj_nano_mbox() {
		// Generate an error
		if (!GetProcessId(NULL)) {

			// error C7555: use of designated initializers requires at least '/std:c++latest'
			// this requires C++20 on the calling side 
			win32_error_msg rezult =
				win32_error_msg_box_exit(
					(win32_error_msg_box_arg) {
				.display_error_message = false,
					.process_exit = false,
					.last_error = 0,
					.error_source = "test_dbj_win32_error"
			}
			);

			DBJ_PRINT("%s", rezult.data);
		}

	}
}

TU_REGISTER([]
	{
		DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE DBJ_RESET);
		sample_dbj_nano_mbox();
	});

#endif // DBJ_HAS_CXX20
