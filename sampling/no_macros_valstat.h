#pragma once

#include "common.h"
#include <charconv>
#include <cstdio>
#include <cinttypes>

// unavoidabler 
// this fili is really sily
#define FILI __FILE__ , __LINE__

namespace try_no_marcos {

	using namespace std;
	using namespace dbj::nanolib;

	// string posix valstat trait
	using p_vt = posix_valstat_trait<string>;

	[[nodiscard]] p_vt::return_type the_great_divider_in_the_sky
	( uint64_t	dividend, uint64_t	divisor  )
	{
		if (0 == divisor)
			return p_vt::error(
					errc::invalid_argument , FILI
			);

		if ( int(dividend) >= UINT16_MAX)
			return p_vt::error(
				__FUNCTION__  ": dividend too large" , FILI
			);

		if ( int(divisor) >= UINT16_MAX)
			return p_vt::error(
				__FUNCTION__  ": divisor too large", FILI
			);

			auto buff = v_buffer::format("%.3f", (dividend / divisor));
		return p_vt::ok( buff.data()  );
	}

	TU_REGISTER(
		[] {
			DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE); DBJ_PRINT(" " DBJ_RESET);

			auto driver = [](uint64_t a, uint64_t b) {
				constexpr auto const& div = the_great_divider_in_the_sky;

				DBJ_PRINT( "\n\nTrying to divide %" PRId64 ", with %" PRId64 " \n\n",a,b);

				auto [val, stat] = div(a, b);

				if (!val)
					DBJ_PRINT(DBJ_FG_RED_BOLD "\nERROR");
				else
					DBJ_PRINT("\n%s", val->c_str());

				if (stat)
					DBJ_PRINT( DBJ_RESET  "\n\nstatus: %s\n\n", stat->data());
			};

			driver(UINT16_MAX, 2);
			driver( 5, 7 );
		}
	);
} // try_no_marcos

#undef FILI
