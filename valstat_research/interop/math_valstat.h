#pragma once

#include "c_interop.h"


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
  struct interop_valstat_int { 
	int * value; 
	const char * status; 
  }
*/
	valstat_interop(double);

	/* type returned is: interop_valstat_double */
	valstat_interop_decl(double) log_valstat(  double _X);
	valstat_interop_decl(double) log10_valstat( double _X);

	/*
	in C there is no overloading allowed, thus we can not use the 
	log and log10 as function names
	*/

#ifdef __cplusplus
} // extern 
#endif // __cplusplus

