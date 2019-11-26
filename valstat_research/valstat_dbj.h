#pragma once
/*
valstat 2 -- no pairs
*/

#include "valstat"
#include "../common.h"

namespace dbj
{
	using namespace std;

	/*
	dbj -- the imagined company
	this type alias is all they have and need to use std::valstat
	this makes them fully std::valstat compliant
	this also makes other people better understand their API's

	NOTE: std::string is not the most performant json
		  string implementation. dbj, they usually use vector<char> or even
		  unique_ptr<char[]>
		  that issue is largely mitigated by not using strings
		  but handles to the strings registry as the status type
	*/
	template< typename T>
	using valstat = future_std::valstat<T, std::string >;

	// descriptive output
	// the  verbose valstat consuming
	// for testing purposes only
	// dbj usualy do not use <iostream> in a production code
	template<typename T>
	inline std::ostream& operator << (std::ostream& os, const dbj::valstat<T> vt)
	{
		os << "\nvalstat state: ";
		if (!vt.value && vt.status) os << "ERROR";
		if (vt.value && !vt.status) os << "OK";
		if (vt.value && vt.status) os << "INFO";
		if (!vt.value && !vt.status) os << "EMPTY";

		os << " , content: \n{";
		if (vt.value)
			os << "\n { value: " << *vt.value << " }";
		else
			os << "\n { value: empty }";
		os << " ,";
		if (vt.status)
			os << "\n { status: " << *vt.status << " }";
		else
			os << "\n { status: empty }";
		return os << "\n}\n";
	}

} // dbj

