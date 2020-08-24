#pragma once
#include "../common.h"
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
		DBJ_PRINT( first_param, params ... ) ;
	};
} // dbj

namespace dbj::win {

class ConsoleUTF8 {
 public:
  ConsoleUTF8() : original_cp(::GetConsoleOutputCP()) {
    ::SetConsoleOutputCP(65001 /*CP_UTF8*/);
  }
  ~ConsoleUTF8() { ::SetConsoleOutputCP(original_cp); }

 private:
  UINT original_cp;
};

/// -------------------------------------------------------------------------
std::wstring narrow_utf8_to_wstring(std::string_view view) {
  int size = MultiByteToWideChar(65001 /*CP_UTF8*/, 0, view.data(),
                                 static_cast<int32_t>(view.size()), nullptr, 0);

  if (size == 0) {
    return {};
  }

  std::wstring result(size, L'*');
  size = MultiByteToWideChar(65001 /*CP_UTF8*/, 0, view.data(),
                             static_cast<int32_t>(view.size()), result.data(),
                             size);
  _ASSERT(size > 1);

  return result;
}

/// -------------------------------------------------------------------------
inline std::string wide_utf8_to_string(std::wstring_view value) {
  int size = WideCharToMultiByte(65001 /*CP_UTF8*/, 0, value.data(),
                                 static_cast<int32_t>(value.size()), nullptr, 0,
                                 nullptr, nullptr);
  if (size == 0) {
    return {};
  }

  std::string result(size, '?');
  size = WideCharToMultiByte(65001 /*CP_UTF8*/, 0, value.data(),
                             static_cast<int32_t>(value.size()), result.data(),
                             size, nullptr, nullptr);

  _ASSERT(size > 1);

  return result;
}

}  // namespace dbj::win
