#ifndef _DBJ_VALSTAT_INC_
#define _DBJ_VALSTAT_INC_
/**
 Proposal for c++ standard function returns
 This is concept, behavioral pattern and kind-of-a standard C++ interface
 
 (c) by  Dusn B. Jovanovic, https://dusanjovanovic.org

 Licence: CC BY SA 4.0
*/

#include <optional>

namespace dbj_std
{
	using ::std::optional;

	template< typename T, typename S>
	struct [[nodiscard]] valstat
	{
		using type			= valstat;
		using value_type	= T;
		using status_type	= S;
		optional<T>			value;
		optional<S>			status;
	};
};

#endif // !_DBJ_VALSTAT_INC_
