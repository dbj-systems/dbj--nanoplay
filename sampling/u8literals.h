#pragma once

#include "../common.h"

namespace dbj::u8literals {

	using namespace std::literals;
	
//#include <cstdio>
//#include <cstdlib>
//#include <string>
//#include <string_view>

	constexpr auto  singles  = u8"│┤┐└┴┬├─┼┘┌"sv;
	constexpr auto 	doubles  = u8"╣║╗╝╚╔╩╦╠═╬"sv;
	constexpr auto 	doublesv = u8"╟╨╥╙╓╫╢╖╜"sv;
	constexpr auto 	singlesv = u8"╕╒╪╞╧╡╘╤╛"sv;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wmissing-braces"
#pragma clang diagnostic ignored "-Wformat"
#endif

#define scatter( SQNC ) \
std::printf("\nInput: %s, one by one --> ", SQNC ); \
for ( auto & u8chr : SQNC ) \
{ \
   std::printf("%c", u8chr) ; \
}

	// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r2.html
	TU_REGISTER([]
		{
			using namespace std;
			fmt::print("\n\n__cplusplus: {}, __cpp_lib_char8_t: {}\n\n", __cplusplus, __cpp_lib_char8_t);
			scatter(u8"РАЧУНАРИ");
			scatter(singles);
			scatter(doubles);
			scatter(doublesv);
			scatter(singlesv);
		});

#undef scatter

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}

