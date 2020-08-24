#pragma once

#include "../common.h"
#include "utf8_common.h"

namespace dbj_kilim {

	// using namespace std::string_view_literals;
	using std::array;

	inline std::random_device random_device_;
	inline std::mt19937 the_generator_(random_device_());

	constexpr array		singles{ u8"┤", u8"┐", u8"└", u8"┴", u8"┬", u8"├", u8"─", u8"┼", u8"┘", u8"┌", u8"│" };
	constexpr array 	doubles{ u8"╣", u8"║", u8"╗", u8"╝", u8"╚", u8"╔", u8"╩", u8"╦", u8"╠", u8"═", u8"╬" };
	constexpr array 	doublesv{ u8"╟", u8"╨", u8"╥", u8"╙", u8"╓", u8"╫", u8"╢", u8"╖", u8"╜" };
	constexpr array 	singlesv{ u8"╕", u8"╒", u8"╪", u8"╞", u8"╧", u8"╡", u8"╘", u8"╤", u8"╛" };
	// "アルファベット" arufabetto/ an alphabet
	// requires proper font, e.g. "MS Gothic"
	constexpr array 	arufabetto{ u8"ア", u8"ル", u8"フ", u8"ァ", u8"ベ", u8"ッ", u8"ト" };


	TU_REGISTER([]
		{
			DBJ_PRINT( DBJ_FG_CYAN_BOLD DBJ_FILE_LINE DBJ_RESET );

			{
				::system("chcp 65001");
				// LINUX
				auto smiley = u8"😀";
				DBJ_PRINT(
					"The %s 'glyph': %s", typeid(smiley).name(), (const char*)smiley
				);
			}

			auto driver = [&](auto char_set, size_t counter = size_t(0xF)) {

				using namespace std::chrono_literals;
				std::uniform_int_distribution<> random_idx(0, int(char_set.size()) - 1);

				while (counter--)
				{
					// just print & pray
					// this cast makes it silent
					DBJ_PRINT("%s", (const char *)char_set[random_idx(the_generator_)]);
					std::this_thread::sleep_for(0s);
				}
				DBJ_PRINT(" ");
			};

			DBJ_PRINT(" (c) 2020 dusanjovanovic.org ");
			//driver(singles,		0xFFFF);
			//driver(doubles,		0xFFFF);
			//driver(doublesv,	0xFF);
			//driver(singlesv,	0xFF);
			driver(arufabetto);
		});
}