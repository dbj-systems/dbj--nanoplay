
// #include "./dbj--nanolib/win32/win32_error_msg_box_exit.h"
#include "D:\DEVL\GitHub\dbj++nanoplay\dbj--nanolib\win32\win32_error_msg_box_exit.h"

#include "../common.h"

TU_REGISTER([]
	{
		DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE DBJ_RESET);

		// Generate an error
		if (!GetProcessId(NULL)) {

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
	});