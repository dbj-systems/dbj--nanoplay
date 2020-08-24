#pragma once

#include "../common.h"

// #include "dbj--nanolib/dbj++debug.h"
// #include "dbj--nanolib/utf/dbj_utf_cpp.h"

// WIN c++ char32_t string literal
#define HIRAGANA_SL U"平仮名"
//
// To see the above
// WIN10 console setup required
// codepage: 65001
// font: NSimSun
// 

DBJ_PURE_FUNCTION inline void test_dbj_debug() noexcept
{
	dbj::utf::utf16_string utf16_(
		dbj::utf::utf32_string(HIRAGANA_SL)
	);
	wprintf(L"\n%s\n", utf16_.get());

	dbj::utf::utf8_string utf8_(
		dbj::utf::utf32_string(HIRAGANA_SL)
	);
	wprintf(L"\n%S\n", utf8_.get());

	constexpr auto arb = dbj::nanolib::make_arr_buffer(U"平仮名");

	(void)arb;

	DBJ_SXT(dbj::nanolib::make_arr_buffer(U"平仮名"));
	DBJ_SX(
		dbj::utf::utf8_string(
			dbj::utf::utf32_string(
				dbj::nanolib::make_arr_buffer(U"平仮名").data()
			)
		).get()
	);

	using bufy = typename dbj::nanolib::v_buffer::type;

	DBJ_SX("make char32_t string literal into std array buffer, then into dbj buffer then print it");

	DBJ_SX(bufy::make(dbj::nanolib::make_arr_buffer(U"平仮名").data()).data());

}

TU_REGISTER([]
	{
		DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE DBJ_RESET);
		test_dbj_debug();
	});
