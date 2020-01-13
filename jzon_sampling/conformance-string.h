#pragma once

#include "../common.h"
#include "../dbj--nanolib/nonstd/dbj++ct.h"

namespace dbj_jzon_testing
{
#define TEST_STRING(json, expect)        \
    do { doc.parse(json);                     \
    val = doc[0];                        \
    n = sizeof(expect) - 1;              \
    DBJ_PRINT(R"(json: "%s", expected parse rezult: "%s")", json, expect); \
	/*TU_CHECK(dbj::nanolib::ct::str_equal(val.to_string() , expect)); */   \
    TU_CHECK(strlen(val.to_string()) == n); \
    TU_CHECK( false == memcmp(val.to_string(), expect, n)) ; } while(0)

	TU_REGISTER([] {
		jzon::document doc;
		jzon::value val;
		size_t n;

		TEST_STRING("[\"\"]", "");
		TEST_STRING("[\"Hello\"]", "Hello");
		TEST_STRING("[\"Hello\\nWorld\"]", "Hello\nWorld");
		TEST_STRING("[\"Hello\\u0000World\"]", R"(Hello\0World)");
		TEST_STRING("[\"\\\"\\\\/\\b\\f\\n\\r\\t\"]", "\"\\/\b\f\n\r\t");
		TEST_STRING("[\"\\u0024\"]", "\x24");                    // Dollar sign U+0024
		TEST_STRING("[\"\\u00A2\"]", "\xC2\xA2");                // Cents sign U+00A2
		TEST_STRING("[\"\\u20AC\"]", "\xE2\x82\xAC");            // Euro sign U+20AC
		TEST_STRING("[\"\\uD834\\uDD1E\"]", "\xF0\x9D\x84\x9E"); // G clef sign U+1D11E
	});

} // dbj_jzon_testing

#undef TEST_STRING