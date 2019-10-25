#ifndef _INC_VAL_STAT_OPTIREF_
#define _INC_VAL_STAT_OPTIREF_

#include "valstat_2.h"

namespace dbj::nanoplay {

	using namespace std;
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
			return { {rezult}, { make_status(__FILE__, __LINE__, __TIMESTAMP__)  } };
		}
		else {
			// valstat error state
			return { {}, { make_status(__FILE__, __LINE__, __TIMESTAMP__)  } };
		}
	}
	// Test Unit aka "Unit Test" ;)
	TU_REGISTER([]
		{
			using namespace std::literals;
			cout << endl << convert<int>("42"sv);
			cout << endl << convert<float>("4.2"sv);
		}
	);

	/*
	-----------------------------------------------------------------------
	PARADIGM SHIFTING -- opearator returning valstat makes for richer
	return producing and return consuming logic
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
			if (idx_ >= buff_.size())
				return { {}, { make_status(__FILE__, __LINE__, __TIMESTAMP__,"Index out of bounds") } };

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

#if defined(__clang__)
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
		// paradigm shift, no exception logic, local handling
		cout << endl << ar[5];
		cout << endl << ar[0];

		});

#if defined(__clang__)
	TU_REGISTER([] {
			auto [val,stat] = 0xFFF_to_valstat;

			if (val) {
				cout << endl << (*val)[1];
			}
		});
#endif
	/*
	handling no copy/no move types
     */
	namespace {
		struct adamant final{
			inline static const char* genus = "tenacious";
			adamant(const adamant&) = delete;
			adamant & operator = (const adamant&) = delete;
			adamant(adamant&&) = delete;
			adamant& operator = (adamant&&) = delete;
			// convention:
			// type::vt
			// is the encapsulated valstat for the type
			using vt = valstat< reference_wrapper<adamant> >;

			friend ostream& operator << (ostream& os, const adamant& vt)
			{
				return os << "adamant::genus = " << vt.genus;
			}
		};

	}

	TU_REGISTER([] {
			adamant steadfast{};

			auto info = [&]( ) -> adamant::vt { return  { {steadfast}, {"info message"} }; };

			auto error = [&]( ) -> adamant::vt { return  { {}, {"error message"} }; };

			auto ok = [&]( ) -> adamant::vt { return  { {steadfast}, {} }; };

			auto empty = [&]( ) -> adamant::vt { return  { {}, {} }; };

			auto consumer = []( auto producer ) {
				// auto [val, stat] = producer();
				// sampling through the verbose stream output op.
				cout << producer();
			};

			consumer(info);
			consumer(error);
			consumer(ok);
			consumer(empty);

		});

} // dbj::nanoplay

#endif // _INC_VAL_STAT_OPTIREF_



