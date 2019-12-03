#pragma once

#define DBJ_STD_VALSTAT

#ifdef DBJ_STD_VALSTAT
#include <optional>
#endif

namespace dbj_std
{
    inline namespace valstat_std {
	using ::std::optional;

	template< typename T, typename S>
	struct [[nodiscard]] valstat
	{
		using value_type = T;
		using status_type = S;
		optional<T>			value;
		optional<S>			status;
	};
	}

#ifdef DBJ_STD_VALSTAT
	using namespace valstat_std;
#endif
};