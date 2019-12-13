#pragma once
#include <random>
/* this can speed up things considerably. but test comprehensively first! */
namespace dbj {

	namespace u8speciemens
	{
		using namespace std::string_view_literals;

		// std::u8string_view literals
		constexpr auto  singles = u8"│┤┐└┴┬├─┼┘┌"sv;
		constexpr auto 	doubles = u8"╣║╗╝╚╔╩╦╠═╬"sv;
		constexpr auto 	doublesv = u8"╟╨╥╙╓╫╢╖╜"sv;
		constexpr auto 	singlesv = u8"╕╒╪╞╧╡╘╤╛"sv;
		// "アルファベット" arufabetto/ an alphabet
		// requires proper font, e.g. "MS Gothic"
		constexpr auto 	arufabetto = u8"アルファベット"sv;
	}

	inline std::random_device random_device_;
	inline std::mt19937 the_generator_(random_device_());

	// usage:
	// std::uniform_int_distribution<> from_25_to_64_(25, 63);
	// const int random_number = from_25_to_64_(the_generator_);

	inline void assume(bool cond)
	{
#if defined(__clang__) // Must go first -- clang also defines __GNUC__
		__builtin_assume(cond);
#elif defined(__GNUC__)
		if (!cond) {
			__builtin_unreachable();
		}
#elif defined(_MSC_VER)
		__assume(cond);
#else
		// Do nothing.
#endif
	}
#ifndef DBJ_VERIFY
	[[noreturn]] inline void terror
	(char const* msg_, char const* file_, const unsigned line_, char const* timestamp_)
	{
		assert(msg_ != nullptr);	assert(file_ != nullptr);	assert(line_ > 0);
		std::fprintf(stderr, "\n\nTerminating ERROR:%s\n%s (%d)[%s]", msg_, file_, line_, timestamp_);
		std::exit(EXIT_FAILURE);
	}

#define DBJ_VERIFY_(x, file, line, timestamp ) if (false == (x) ) ::dbj::terror( #x ", failed", file, line, timestamp )
#define DBJ_VERIFY(x) DBJ_VERIFY_(x,__FILE__,__LINE__, __TIMESTAMP__)
#endif // DBJ_VERIFY

	template<typename T, typename ... A>
	inline auto
		print(T const& first_param, A const& ... params)
	{
		std::cout << first_param;
		// if there are  more params
		if constexpr (sizeof...(params) > 0) {
			// recurse
			print(params...);
		}
	};
} // dbj

