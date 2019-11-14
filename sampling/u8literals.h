#pragma once

#include "../common.h"

#if __cplusplus // > 201703L
// C++20 code

namespace dbj::u8literals {
	using namespace std::literals;
	// std::u8string_view literals
	constexpr auto  singles = u8"│┤┐└┴┬├─┼┘┌"sv;
	constexpr auto 	doubles = u8"╣║╗╝╚╔╩╦╠═╬"sv;
	constexpr auto 	doublesv = u8"╟╨╥╙╓╫╢╖╜"sv;
	constexpr auto 	singlesv = u8"╕╒╪╞╧╡╘╤╛"sv;

	template<typename T, size_t N>
	void scatter(const T(&sequence)[N])
	{
		std::printf("\n\ntype of a single u8 element: %s\n", typeid(sequence[0]).name());
		for (auto& u8chr : sequence) std::printf("%c", u8chr);
	}

	void scatter(std::u8string_view sequence)
	{
		std::printf("\n\ntype of a single u8 element: %s\n", typeid(sequence[0]).name());
		for (auto& u8chr : sequence) std::printf("%c", u8chr);
	}

	// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r2.html
	TU_REGISTER([]
		{
			using namespace std;

			fmt::print("\n\n__cplusplus: {}, __cpp_lib_char8_t: {}\n\n", __cplusplus, __cpp_lib_char8_t);

			scatter(u8"АБВГДЂЕЖЧЋШ");
			scatter(singles);
			scatter(doubles);
			scatter(doublesv);
			scatter(singlesv);

			// "アルファベット" arufabetto/ an alphabet
			fmt::print("\n{} arufabetto/ an alphabet", "アルファベット");
			// requires proper font, e.g. "MS Gothic"
			scatter(u8"アルファベット");

			bool dumsy = true;
			if (dumsy) return;
		});
}

#endif // std::u8string_view
