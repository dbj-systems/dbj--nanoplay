#pragma once
/*
valstat 2 -- no pairs 
*/

#include "../common.h"
#include <charconv>

namespace valstat_2 {
	using namespace std;
	using namespace dbj::nanolib;

	string make_status( const char* file, long line, const char* msg = nullptr );

	// native version -- one step from C
	//template< typename T>
	// struct [[nodiscard]] valstat  { using value_type = T; T * value; const char * status; };

	// no pair version
	// status == string
	template< typename T>
	struct [[nodiscard]] valstat{ 
		using value_type = T; 
	    optional<T> value; 
		optional<string> status; 
	};

	// descriptive output
	template<typename T>
	ostream& operator << (ostream& os, const valstat<T> vt)
	{
		os << "\nvalstat state: ";
		if (!vt.value &&  vt.status) os << "ERROR";
		if ( vt.value && !vt.status) os << "OK";
		if ( vt.value &&  vt.status) os << "INFO";
		if (!vt.value && !vt.status) os << "EMPTY";

		os << "\n\ncontent: \n{";
		if (vt.value)
			os << "\n { " << *vt.value << " }";
		else
			os << "\n { empty }";
		os << " ,";
		if (vt.status)
			os << "\n { " << *vt.status << " }";
		else
			os << "\n { empty }";
		return os << "\n}\n";
	}
	// sampling
	template< typename T>
	inline valstat<T> convert(string_view sv) noexcept(true)
	{
		T rezult;
		if (auto [p, e] = from_chars(sv.data(), sv.data() + sv.size(), rezult); 
			/* std::errc() is dubious hack from cppreference.com */
			e == std::errc() 
			)
		{
			// valstat info state
			return { {rezult}, { make_status(__FILE__, __LINE__)  } };
		}
		else {
			// valstat error state
			return { {}, { make_status(__FILE__, __LINE__)  } };
		}
	}
	// Test Unit aka "Unit Test" ;)
	TU_REGISTER([]
		{
			using namespace std::literals;
			cout << endl << convert<int>("42"sv) ;
			cout << endl << convert<float>("4.2"sv) ;
		}
	);

	/*
	-----------------------------------------------------------------------
	PARADIGM SHIFTING
	*/
	template< size_t N>
	struct arry final 
	{
		array<char, N> buff_{ };

		// valstat return does not require
		// exception thinking
		// there is always a return
		valstat<char> operator [] (size_t idx_) const noexcept
		{
			if (idx_ >= buff_.size() )
				return { {}, { make_status(__FILE__, __LINE__, "Index out of bounds") } };

			return { { buff_[idx_] } , {} };
		}
	}; // arry

	/* 
	(c) dbj@dbj.org  
	literal to std::array
	but no strings until C++ 2.x
	*/
	template< char ... Chs >
	inline constexpr decltype(auto) operator"" _charay()
	{
		// append '\0'
		return  std::array{ Chs..., char(0) };
	}
	
	template< char ... Chs >
	inline constexpr decltype(auto) operator"" _conv()
	{
		// append '\0'
		return arry<1 + sizeof...(Chs)>{ Chs..., char(0) };
	}

#ifdef __GNUC__ 
	// https://wandbox.org/permlink/ubNTUYDrs2NEaDFz
	// yes we can have valstat returned from UDL 
	template< char ... Chs >
	inline constexpr decltype(auto) operator"" _to_valstat()
	{
		using rtype = arry<1 + sizeof...(Chs)>;
		using vt = valstat<rtype>;
		// append '\0'
		return vt{ { rtype{ Chs..., char(0) } } , {} };
	}
#endif // __GNUC__

	TU_REGISTER([] {

		constexpr auto ar = 123_conv;
		
		// paradigm shift
		// no exception logic
		// local handling
		cout << endl << ar[5] ;
		cout << endl << ar[0] ;

		});

	inline string make_status(const char* file, long line, const char* msg) 
	{
		auto nix_path = v_buffer::replace(v_buffer::format("%s", file), '\\', '/');
		v_buffer::buffer_type buffy = v_buffer::format(
			"{ \"message\" : \"%s\", \"file\" : \"%s\", \"line\" : %d }",
			(msg ? msg : "unknown"), nix_path.data(), line);

		return { buffy.data() };
	}

} // namespace valstat_2 
