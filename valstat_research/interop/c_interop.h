#ifndef _INC_C_INTEROP_
#define _INC_C_INTEROP_

#include "../valstat_dbj_c_interop.h"

#ifdef __cplusplus
namespace dbj {
	extern "C" {
#endif // __cplusplus

		/*
		what follows is valstat interop testing C side
		here we re-use predefined valstat's
		*/

		typedef enum { EMPTY = 0, OK, ERR, INFO } valstat_kind;
		valstat_interop_decl(int) interop_valstat_testing(valstat_kind);

		valstat_interop_decl(uint64_t)
			divider(uint64_t	dividend, uint64_t	divisor);

#ifdef __cplusplus
	} // "C"
} // namespace dbj 
#endif // __cplusplus

#endif // _INC_C_INTEROP_

