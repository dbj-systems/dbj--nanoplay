#pragma once

#include "../common.h"
#include "../valstat_research/valstat_dbj_status.h"
#include <string_view>

// u8 aka UTF-8, it is a mess in C++20
// u is char16_t aka UTF-16 .. a Windows mess
// U is char32_t aka UTF-32 
// But. printf works with (char const *)HIRAGANA with no warnings

/*
	to call UDL with a macro simply add an empty string literal calling the UDL:
	auto jd = HIRAGANA ""_jzon
*/

namespace jzon {


	using jzon_valstat =  std::valstat< jzon::document, jzon::error >;

	inline namespace jzon_literals 
	{
		// before valstat
		[[nodiscard]] jzon::document  operator"" _before(const char* json_char_star_, size_t len_ ) noexcept 
		{
			jzon::document json_doc_instance_;
			json_doc_instance_.parse(json_char_star_);
			return json_doc_instance_ ;
		}
		// after valstat
		[[nodiscard]] jzon_valstat  operator"" _after(const char* json_char_star_, size_t len_ ) noexcept
		{
			jzon::document json_doc_instance_;
			if (false == json_doc_instance_.parse(json_char_star_)) {
				return { {}, json_doc_instance_.error_code() };
			}
			else {
				return { json_doc_instance_, {} };
			}
		}
	}

#if DBJ_HAS_CXX20 == 0

	TU_REGISTER([] {

        #define HIRAGANA u8"ひらがな"
		{
			// guess the state 
			jzon::document maybe_ok_json_doc_ = HIRAGANA ""_before;

			auto [json_doc, jzon_error] = HIRAGANA ""_after;

			if (json_doc) { /* not in an error state */ }

			if (jzon_error) { /* string literal was a bad json */ }
		}
		auto str_ = dbj::make_status(__FILE__, __LINE__, __TIMESTAMP__, HIRAGANA );
		
		jzon::document json_doc;
		json_doc.parse( str_.c_str());

		if ( json_doc.in_error_state() ) {
			jzon::error  json_err = json_doc.error_code();
			jzon::util::print_error("string", str_.c_str() , json_doc);
		}

		DBJ_PRINT("'message' == %s", json_doc["message"].to_string("null"));

		});
#endif // not c++20

} // jzon

#undef HIRAGANA	
