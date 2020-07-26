#ifdef __clang__

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define SHOW(V_,FMT_) (void)printf("\n%s --> " FMT_, #V_, (V_) )

#ifndef __STDC_NO_THREADS__
#pragma message "No threads in this run time"
#else
#include <threads.h>
#endif // __STDC_NO_THREADS__
/*
__c11_atomic builtins
Clang provides a set of builtins which are intended to be used to implement C11’s <stdatomic.h> header. These builtins provide the semantics of the _explicit form of the corresponding C11 operation, and are named with a __c11_ prefix. The supported operations, and the differences from the corresponding C11 operations, are:

__c11_atomic_init
__c11_atomic_thread_fence
__c11_atomic_signal_fence
__c11_atomic_is_lock_free (The argument is the size of the _Atomic(...) object, instead of its address)
__c11_atomic_store
__c11_atomic_load
__c11_atomic_exchange
__c11_atomic_compare_exchange_strong
__c11_atomic_compare_exchange_weak
__c11_atomic_fetch_add
__c11_atomic_fetch_sub
__c11_atomic_fetch_and
__c11_atomic_fetch_or
__c11_atomic_fetch_xor
__c11_atomic_fetch_max
__c11_atomic_fetch_min
The macros __ATOMIC_RELAXED, __ATOMIC_CONSUME, __ATOMIC_ACQUIRE, __ATOMIC_RELEASE, __ATOMIC_ACQ_REL, and __ATOMIC_SEQ_CST are provided, with values corresponding to the enumerators of C11’s memory_order enumeration.
/// ------------------------------------------------------------------------------------
*/
#include "stdatomic_nano.h"

/// https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_71/rzarg/typeof_operator.htm
#define SWAP(a,b) do { __typeof__(a) temp; temp = a; a = b; b = temp; } while(0)

/*
testing clang visual studio setup 
*/
void test_clang_vs_setup() {
	{
		int a = 13, b = 42;
		SWAP(a, b);
	}

	atomic_flag flag = ATOMIC_FLAG_INIT;
	atomic_int  current = ATOMIC_VAR_INIT(0);

	atomic_init(&current, 13);
	SHOW(atomic_load(&current), "%d");
	SHOW(atomic_exchange( & current , 42 ), "%d") ;
	SHOW(atomic_load(&current), "%d");
}

#else  // no clang
void test_clang_vs_setup() {
#pragma message __FILE__ " requires clang compiler"
}
#endif // no clang
