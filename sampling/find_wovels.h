#pragma once

#include "../common.h"

// always use namespace,to avoid name clashes
namespace dbj::nanoplay {

	// make 'sv' the string_view literal available
	using namespace std::string_view_literals;

	// declare and define compile time
	// string view literal
	// 'constexpr' guarantees compile time
	// notice the use of 'sv'
	constexpr auto vowels = "eaiouEAIOU"sv;

	// compile time function to count literals 
	// in the input 
	// again 'constexpr' guarantees compile time
	// inline gurantees we can include this header many times
	// without making accidental duplicates of `count_vowels`
	// 'in_' argument has 'std::string_view' passed by value
	// pass by value is preffered standard C++ method
	// of functions arguments passing
	//  'std::string_view' is standard C++ preffered type
	// to pass strings into functions
	inline constexpr size_t
		count_vowels(std::string_view in_)
	{
		// return type is size_t
		// we can count very large number of vowels
		// but all at compile time
		size_t rezult{};
		// this is C+17 'range for'
		// we cast implicitly references to input elements
		// from, `char const &` to `int const &`
		// cost of that is very likely 0
		for (char const& ch_ : in_)
			for ( char const& v_ : vowels)
				// there is no if() here
				// we simply add 0's or 1's, to the rezult
				// false is 0, true is 1
				// the correct by the book way of coding that is
				// static cast from bool to int
				// rezult +=  static_cast<int>( v_ == ch_ ) ;
				rezult += (v_ == ch_ ? 1 : 0 );

		// we do not use unnecesary braces { } above

		return rezult;
	}

	constexpr const char abracd[] = "abra ca dabra";
	// runtime speed of this call is 0 (zero)
	// all happens at compile time
	// notice how we pass normal string literal
	// no need to create string_view
	constexpr size_t r1
		= count_vowels(abracd);

	// no runtime tests necessary
	// `static_assert()` is compile time assert
	// failure message is optional
	static_assert(r1 == 5,
		"compile time calculation failed, 'abra ca dabra', must contain 5 vowels");

	TU_REGISTER(
		[] {
			DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE); DBJ_PRINT(" " DBJ_RESET);

			std::string input_{ "abra cadabra" };
			[[maybe_unused]] size_t rezult = count_vowels(input_);
		}
	);

}

/*
---------------------------------------------------------------------
The optimized version
aka: optimized version of the above
---------------------------------------------------------------------
*/
inline auto count = [](auto const& array) constexpr -> unsigned long {
	return (sizeof(array) / sizeof(array[0]));
};

template<typename A_ >
inline constexpr size_t array_count(A_ const& array)
{
	return (sizeof(array) / sizeof(array[0]));
};


namespace dbj_find_wovels
{
	using namespace std::string_view_literals;

	inline constexpr size_t
		count_vowels(const char* p_, size_t len_)
	{
		constexpr char vowels[] = "eaiouEAIOU";
		constexpr size_t vowels_count = array_count(vowels);

		size_t rezult{};
		while (--len_) {
			size_t vc_ = vowels_count;
			while (--vc_)
				rezult += (vowels[vc_] == p_[len_]);
		}
		return rezult;
	}

	// compile time tests
	constexpr const char abra_[] = "abra ca dabra";
	constexpr size_t r1
		= count_vowels(abra_, array_count(abra_));

	static_assert(r1 == 5,
		"compile time calculation failed, 'abra ca dabra', must contain 5 vowels");

	// run time test
	TU_REGISTER([]
		{
			DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE); DBJ_PRINT(" " DBJ_RESET);

#define SPECIMEN "bimbila bambili bumbili" 
			[[maybe_unused]] size_t r1 = count_vowels(
				SPECIMEN,
				array_count(SPECIMEN)
			);
#undef SPECIMEN
		});

}