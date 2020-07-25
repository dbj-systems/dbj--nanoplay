#include "../common.h"

/// ----------------------------------------------------------------------------

inline void dbj_putc(void* p, char c) noexcept
{
	putc(c, stdout);
}
/// ----------------------------------------------------------------------------
/// The format chars supported are: 'd' 'u' 'c' 's' 'x' 'X' and 'p'
///
unsigned __stdcall printer(void*);
static unsigned __stdcall printer(void* arg_ )
{
	DBJ_NANO_LIB_SYNC_ENTER;

	const char* name_ = (char*)arg_;
	nano_init_printf(NULL, dbj_putc);

	nano_printf("\nBEGIN of %s", name_);
	
#define SPRINF( F_ , ...) do {\
char buf[BUFSIZ]{0};\
 nano_sprintf(buf, F_, __VA_ARGS__);\
 nano_printf("\n%s", buf );\
} while(0)

	SPRINF("\n%d", 42);
	SPRINF("\n%u", 42U);
	SPRINF("\n%c", 'A');
	SPRINF("\n%s", "Abra Ca Dabra");
	SPRINF("\n%x", 0x00012345);
	SPRINF("\n%p", 0x00012345);

	// what here?
	SPRINF("\n%@", 0x00012345);
	//
	Sleep(0);
	nano_printf(
		"\n-----------------------------------------------------------"
		"\nEND of %s", name_
	);
	DBJ_NANO_LIB_SYNC_LEAVE;
	return 0;
#undef SPRINF
}
/// ----------------------------------------------------------------------------
static void  test_nano_printf ()
{
	DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE); DBJ_PRINT(" " DBJ_RESET);

	HANDLE consumer_thread_1 = (HANDLE)_beginthreadex(
		NULL, 0, printer,
		/* thread data is the name we generate for it */
		(void *)"PRINTER_A",
		/*CREATE_SUSPENDED*/ 0, 
		NULL);

	HANDLE consumer_thread_2 = (HANDLE)_beginthreadex(
		NULL, 0, printer,
		/* thread data is the name we generate for it */
		(void *)"PRINTER_B",
		/*CREATE_SUSPENDED*/ 0, 
		NULL);

	WaitForSingleObject(consumer_thread_1, 1 );
	CloseHandle(consumer_thread_1);

	WaitForSingleObject(consumer_thread_2, 1 );
	CloseHandle(consumer_thread_2);
}

TU_REGISTER( test_nano_printf ); 
