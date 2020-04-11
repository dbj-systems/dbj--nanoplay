#include "../common.h"
#include "../dbj--nanolib/nonstd/nano_printf.h"

#include <stdio.h>

inline void dbj_putc(void* p, char c) noexcept
{
	putc(c, stdout);
}

TU_REGISTER(
	[] {
		nano_init_printf(NULL, dbj_putc);

		// The formats chars supported are: 'd' 'u' 'c' 's' 'x' 'X' and 'p'

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
	}
);