#ifndef _INC_C_INTEROP_
#define _INC_C_INTEROP_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
The whole of valstat C interop is two macros bellow
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
what follows is valstat interop testing C side
*/

	typedef enum { EMPTY = 0, OK, ERR, INFO } valstat_kind;

	valstat_interop(int);

	valstat_interop_decl(int) interop_valstat_testing( valstat_kind  );

#ifdef __cplusplus
} // "C"
#endif // __cplusplus

#endif // _INC_C_INTEROP_

