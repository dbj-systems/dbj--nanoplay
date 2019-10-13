#pragma once

#include "dbj--nanolib/dbj++valstat.h"
#include "dbj--nanolib/dbj++tu.h"
#include <charconv>

// unavoidabler 
#define FILI __FILE__ , __LINE__

namespace try_no_marcos {

	using namespace std;
	using namespace dbj::nanolib;

	using p_vt = posix_valstat_trait<string>;

	[[nodiscard]] p_vt::return_type the_great_divider_in_the_sky
	( unsigned long dividend, unsigned long divisor  )
	{
		if (0 == divisor)
			return p_vt::make_error(
				p_vt::make_status(
					p_vt::code_type( errc::invalid_argument ), FILI
				)
			);

		if ( int(dividend) > INT_MAX )
			return p_vt::make_error(
				p_vt::make_status(
					__FUNCSIG__ " dividend too large" , FILI
				)
			);

		if ( int(divisor) > INT_MAX )
			return p_vt::make_error(
				p_vt::make_status(
					__FUNCSIG__ " divisor too large", FILI
				)
			);

		std::array<char, 64> str{ { 0 } };

		auto[p, ec] = to_chars(str.data(), str.data() + str.size(), dividend / divisor );

		if ( ec != std::errc() )
			return p_vt::make_error(
				p_vt::make_status(
					__FUNCSIG__ " std::to_chars() has failed", FILI
				)
			);

		*p = '\0';

		return p_vt::make_ok( str.data()  );

	}

	TU_REGISTER(
		[] {

			auto div = the_great_divider_in_the_sky;

			auto [val, stat] = div( 10, 0 );

			if (!val)
				DBJ_PRINT( DBJ_FG_RED_BOLD "\nERROR" DBJ_RESET );
			else
				DBJ_PRINT("\n%s", val->c_str());

			if ( stat )
				DBJ_PRINT("\n\nstatus: %s\n\n", stat->data() );

		}
	);
} // try_no_marcos

#undef FL
