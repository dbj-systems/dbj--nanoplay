#ifndef _INC_C_INTEROP_
#define _INC_C_INTEROP_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/*
	The whole of valstat C interop is in macros bellow
	*/
#define valstat_interop_( T ) typedef struct interop_valstat_##T \
{ \
T * value ; \
const char * status  ; \
} interop_valstat_##T

#define valstat_interop( T ) valstat_interop_( T )

#define valstat_interop_decl_( T ) interop_valstat_##T 
#define valstat_interop_decl( T ) valstat_interop_decl_(T) 

	/*
	here we can define all the valstat for intrinsic types
	we might need
	*/
	/* this has created 
	  struct interop_valstat_int { int * value; const char * status; } 
	 */
	valstat_interop(int);
	/* this has created 
	  struct interop_valstat_uint64_t  { uint64_t * value; const char * status; } 
	  */
	valstat_interop(uint64_t );

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
#endif // __cplusplus

#endif // _INC_C_INTEROP_

