
#define LOGTARGET stderr
/* nano debugging */
#define DBJ_SX(fmt_ , x_) fprintf( LOGTARGET, "\n%s --> " fmt_ , #x_, x_ )

#include "dbj_string_storage.h"
#include <stdio.h>

int dbj_string_storage_test( const int argc, const char ** argv ) 
{
	const sl_storage_struct * ss_ = dbj_sl_storage();

	size_t ss_index = sl_storage_store(ss_, __FILE__);

	DBJ_SX("%s" , sl_storage_get(ss_, ss_index));

	DBJ_SX("%llu", sl_storage_left(ss_));
}