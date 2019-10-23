#pragma once

/*
valstat 2 -- no pairs necessary
*/

//#include <iostream>
//#include <stdlib.h>
//#include <stdio.h>
//#include <utility>
//#include <optional>
#include <charconv>

#include "../dbj--nanolib/dbj++tu.h"
#include "picojson.h"

namespace valstat_2 {
	using namespace std;
	using namespace dbj::nanolib;

	inline string json_status(const char* file, long line) {
		auto normalized_file_name = v_buffer::replace ( v_buffer::format("%s", file)  , '\\', '/');
		v_buffer::buffer_type buffy = v_buffer::format("{ \"file\" : \"%s\", \"line\" : %d }", normalized_file_name.data() , line);
		return { buffy.data() };
	}

	// native version -- one step from C
	//template< typename T>
	// struct [[nodiscard]] valstat  { using value_type = T; T * value; const char * status; };

	// no pair version
	template< typename T>
	struct [[nodiscard]] valstat{ using value_type = T; optional<T> value; optional<string> status; };

	//
	template< typename T>
	ostream& operator << (ostream& os, valstat<T> vt)
	{
		os << "{ ";
		if (vt.value)
			os << "{ " << *vt.value << " }";
		else
			os << "{ }";
		os << ",";
		if (vt.status)
			os << "{ " << *vt.status << " }";
		else
			os << "{ }";
		return os << " } ";
	}
	//
	template< typename T>
	inline valstat<T> convert(string_view sv) noexcept(true)
	{
		T rezult;
		if (auto [p, e] = from_chars(sv.data(), sv.data() + sv.size(), rezult); e == std::errc())
		{
			// valstat info state
			return { {rezult}, { json_status(__FILE__, __LINE__)  } };
		}
		else {
			// valstat error state
			return { {}, { json_status(__FILE__, __LINE__)  } };
		}
	}
	// Test Unit aka "Unit Test" ;)
	TU_REGISTER([]
		{
			using namespace std::literals;
			cout << endl << "valstat: " << convert<int>("42"sv) ;
			cout << endl << "valstat: " << convert<float>("4.2"sv) ;
		}
	);

} // namespace valstat_2 
