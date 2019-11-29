#include <stdlib.h>
#include "c_interop.h"



/*
this is deliberately verbose
*/
valstat_interop_decl(int) interop_valstat_testing(valstat_kind kind_)
{
	// value is static int
	static int fty2 = 42;

	// info
	interop_int vint_info = { &fty2, INTEROP_JSON(" Just an info") };
	// error
	interop_int vint_err = { NULL, INTEROP_JSON("error has occured") };
	// ok
	interop_int vint_ok = { &fty2, NULL };
	// empty
	interop_int vint_empty = { NULL, NULL };

	switch (kind_) {
	case INFO: return vint_info;
	case OK: return vint_ok;
	case ERR: return vint_err;
	case EMPTY: return vint_empty;
	}
	return vint_empty;
}

/*
 constolled division of only positive integer types
*/
valstat_interop_decl(uint64_t)
  divider (uint64_t	dividend, uint64_t	divisor)
{
	if (0 == divisor)
		return	(interop_uint64_t){	NULL, INTEROP_JSON("invalid_argument: zero divisor") };

	if ((dividend) >= UINT16_MAX)
		return (interop_uint64_t) { NULL, INTEROP_JSON("dividend too large") };

	if ((divisor) >= UINT16_MAX)
		return (interop_uint64_t) { NULL, INTEROP_JSON("divisor too large") };

	// quick and dirty cludge
	static uint64_t rezult_anchor = 0;
		rezult_anchor = (dividend / divisor);
	// valstat OK
	return (interop_uint64_t) { & rezult_anchor , NULL };
}


  