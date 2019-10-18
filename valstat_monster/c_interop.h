#ifndef _INC_C_INTEROP_
#define _INC_C_INTEROP_

// very tempting no macro idea
//extern "C" {
//	typedef struct interop_valstat 
//	{
//		void* value;
//		void* status;
//	} interop_valstat;
//} // "C"

#define valstat_interop_( T ) typedef struct interop_valstat_##T \
{ \
T * value ; \
const char * status  ; \
} interop_valstat_##T
#define valstat_interop( T ) valstat_interop_( T )

#define valstat_interop_decl_( T ) interop_valstat_##T 
#define valstat_interop_decl( T ) valstat_interop_decl_(T) 

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	typedef enum { EMPTY = 0, OK, ERR, INFO } valstat_kind;

	valstat_interop(int);

	valstat_interop_decl(int) interop_valstat_testing( valstat_kind  );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _INC_C_INTEROP_

