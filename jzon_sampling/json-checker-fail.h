#pragma once

#include "../common.h"

#if DBJ_HAS_CXX20 == 0
// forget it for C++20
namespace dbj_jzon_testing
{

	TU_REGISTER([] 
		{
		jzon::document doc;

		TU_CHECK(doc.parse(u8R"json("A JSON payload should be an object or array, not a string.")json"));
		TU_CHECK(false == doc.parse(u8R"json(["Unclosed array")json"));
		TU_CHECK(false == doc.parse(u8R"json({unquoted_key: "keys must be quoted"})json"));
		TU_CHECK(false == doc.parse(u8R"json(["extra comma",])json"));
		TU_CHECK(false == doc.parse(u8R"json(["double extra comma",,])json"));
		TU_CHECK(false == doc.parse(u8R"json([   , "<-- missing value"])json"));
		TU_CHECK(false == doc.parse(u8R"json(["Comma after the close"],)json"));
		TU_CHECK(false == doc.parse(u8R"json(["Extra close"]])json"));
		TU_CHECK(false == doc.parse(u8R"json({"Extra comma": true,})json"));
		TU_CHECK(false == doc.parse(u8R"json({"Extra value after close": true} "misplaced quoted value")json"));
		TU_CHECK(false == doc.parse(u8R"json({"Illegal expression": 1 + 2})json"));
		TU_CHECK(false == doc.parse(u8R"json({"Illegal invocation": alert()})json"));
		TU_CHECK(false == doc.parse(u8R"json({"Numbers cannot have leading zeroes": 013})json"));
		TU_CHECK(false == doc.parse(u8R"json({"Numbers cannot be hex": 0x14})json"));
		TU_CHECK(false == doc.parse(u8R"json(["Illegal backslash escape: \x15"])json"));
		TU_CHECK(false == doc.parse(u8R"json([\naked])json"));
		TU_CHECK(false == doc.parse(u8R"json(["Illegal backslash escape: \017"])json"));
		TU_CHECK(doc.parse(u8R"json([[[[[[[[[[[[[[[[[[[["Too deep"]]]]]]]]]]]]]]]]]]]])json"));
		TU_CHECK(false == doc.parse(u8R"json({"Missing colon" null})json"));
		TU_CHECK(false == doc.parse(u8R"json({"Double colon":: null})json"));
		TU_CHECK(false == doc.parse(u8R"json({"Comma instead of colon", null})json"));
		TU_CHECK(false == doc.parse(u8R"json(["Colon instead of comma": false])json"));
		TU_CHECK(false == doc.parse(u8R"json(["Bad value", truth])json"));
		TU_CHECK(false == doc.parse(u8R"json(['single quote'])json"));
		TU_CHECK(false == doc.parse(u8R"json(["	tab	character	in	string	"])json"));
		TU_CHECK(false == doc.parse(u8R"json(["tab\   character\   in\  string\  "])json"));
		TU_CHECK(false == doc.parse(u8R"json(["line
break"])json"));
		TU_CHECK(false == doc.parse(u8R"json(["line\
break"])json"));
		TU_CHECK(false == doc.parse(u8R"json([0e])json"));
		TU_CHECK(false == doc.parse(u8R"json([0e+])json"));
		TU_CHECK(false == doc.parse(u8R"json([0e+-1])json"));
		TU_CHECK(false == doc.parse(u8R"json({"Comma instead if closing brace": true,)json"));
		TU_CHECK(false == doc.parse(u8R"json(["mismatch"})json"));
		});

} // namespace dbj_jzon_testing
#endif // not C++20