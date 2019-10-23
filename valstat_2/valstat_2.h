#pragma once

/*
valstat 2 -- no pairs necessary
*/

//#include <iostream>
//#include <stdlib.h>
//#include <stdio.h>
//#include <utility>
//#include <optional>
//#include <string_view>

#include "../dbj--nanolib/dbj++tu.h"
#include "picojson.h"

namespace valstat_2 {
	using namespace std;
	using namespace dbj::nanolib;

	inline string json_status(const char* file, long line) {

		v_buffer::buffer_type buffy = v_buffer::format(R"({ "file" : "%s", "line" : "%d" })", file, line);
		picojson::value J{};

		string err = picojson::parse(J, buffy.data());
		if (!err.empty()) { cerr << err << endl; ; return {}; }

		return J.serialize() ;
	}

	// native version
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
		int rez = atoi(sv.data());
		string stat = json_status(__FILE__, __LINE__);
		return { {rez}, { stat  } };
	}
	// Test Unit aka "Unit Test" ;)
	TU_REGISTER([]
		{
			using namespace std::literals;
			auto s = convert<int>("42"sv);

			auto s_copy = move(s);

			cout << endl << "valstat: " << s_copy;
		}
	);

} // namespace valstat_2 
