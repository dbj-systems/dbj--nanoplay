#pragma once

#include "../common.h"

namespace dbj_jzon_testing {

	TU_REGISTER([] 
		{
		jzon::document doc;

		TU_CHECK( false == doc.parse(""));
		TU_CHECK(doc.error_code() == jzon::error::expecting_value);

		TU_CHECK(doc.is_number());
		TU_CHECK(doc.to_number() == 1234567890);

		TU_CHECK(doc.error_code() == jzon::error::expecting_string);

#if DBJ_HAS_CXX20 == 0

		TU_CHECK(doc.parse(u8R"json(1234567890)json"));
		TU_CHECK(false == doc.parse(u8R"json({42: "member name must be string")json"));

		TU_CHECK(doc.parse(u8R"json({
    "empty": {},
    "alpha": "abcdefghijklmnopqrstuvwyz",
    "num": 123456789,
    "literals": [false, true, null]
})json"));
#endif // not C++20

		TU_CHECK(doc.is_object());
		TU_CHECK(doc.size() == 4);
		// TU_CHECK(doc["alpha"].to_string(), "abcdefghijklmnopqrstuvwyz");
		TU_CHECK(doc["num"].to_string("haha") == "haha");
		TU_CHECK(doc["num"].to_number() == 123456789);
		TU_CHECK(doc[100][500].to_number(100500) == 100500);
		TU_CHECK(doc["empty"].is_object());
		TU_CHECK(doc["empty"].size() == 0);
		TU_CHECK(doc["empty"]["missing"].is_null());
		TU_CHECK(doc["empty"]["missing"][111]["really"].to_bool(true));
		TU_CHECK(doc["literals"].is_array());
		TU_CHECK(doc["literals"].size() == 3);
		TU_CHECK(doc["literals"][0].is_bool());
		TU_CHECK(doc["literals"][1].to_bool());
		TU_CHECK(doc["literals"][999].is_null());
		TU_CHECK(doc["literals"]["missing"].is_null());
		});

} // namespace dbj_jzon_testing
