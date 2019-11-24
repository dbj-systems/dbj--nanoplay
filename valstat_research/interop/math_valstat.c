#include "math_valstat.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
For current standard math.h usage please see: 
https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/matherr?view=vs-2019

Obviously a lot of issues with that. also <cmath> is nothing but wrapping these 
same issue into the std namespace.

dbj imagine company, Architect/Desginer of API's will usualy invent some mechanisam arround this.
The issue with that is, it is very specific, perhaps opinionated mechansim. Or she/he will simply
mandate special little exceptions hierarchy to be thrown on errors.

All for end users to use and buy in, or disagree and look elsewhere. 

Below VALSTAT C INTEROP is used in tansforming log and log10 CRT functionns from <math.h> into modern API
with no side effects , readily usable in GUI apps, not using stdio, etc ... 

Even if end users, have never heard of VALSTAT, somehow it looks strangely familliar.

NOTE: this implementation is based on dangerous cludges. This is just a proof of concept.
*/

#undef _STRINGIZE_
#define _STRINGIZE_(x) #x
#undef  STRINGIZE
#define STRINGIZE(x) _STRINGIZE_(x)

#undef FLT
#define FLT(M) \
"{ \"file\": \"" __FILE__  "\", " \
 " \"line\": \""  STRINGIZE(__LINE__) "\", "  "\"message\": \"" M "\", \"timestamp\": \"" __TIMESTAMP__  "\" }" 


static const char* valstat_last_error = 0;
valstat_interop_decl(double) log_valstat(arg_) 
{
	static double last_result = 0;
	valstat_last_error = 0;
	last_result = 0;

	last_result = log(arg_);

	if (valstat_last_error)
		return (valstat_interop_decl(double)){ NULL, valstat_last_error };

	return (valstat_interop_decl(double)){ & last_result , NULL	};

}

valstat_interop_decl(double) log10_valstat(double arg_) 
{
	static double last_result = 0;
	valstat_last_error = 0;
	last_result = 0;

	last_result = log10(arg_);

	if (valstat_last_error)
		return (valstat_interop_decl(double)) { NULL, valstat_last_error };

	return (valstat_interop_decl(double)) { &last_result, NULL };
}

int _matherr(struct _exception* except)
{
	/* Handle _DOMAIN errors for log or log10. */
	if (!strcmp(except->name, "log") || !strcmp(except->name, "log10"))
	{
		switch (except->type)
		{
		case _DOMAIN: valstat_last_error = FLT(  "DOMAIN ERROR" ); break;
		case _SING: valstat_last_error = FLT(  "ARGUMENT SINGULARITY" ); break;
		case _OVERFLOW: valstat_last_error = FLT(  "OVERFLOW RANGE ERROR" ); break;
		case _UNDERFLOW: valstat_last_error = FLT(  "UNDERFLOW RANGE ERROR" ); break;
		case _TLOSS: valstat_last_error = FLT(  "TOTAL LOSS OF PRECISION" ); break;
		case _PLOSS: valstat_last_error = FLT(  "PARTIAL LOSS OF PRECISION" ); break;
		default: 
			perror("Unknown math exception type");
			exit(1);
		}
			return 1;
    }
	return 0;    /* Else use the default actions */
}
