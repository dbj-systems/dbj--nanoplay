#pragma once
#include "../common.h"
#include <random>

namespace dbj::nanolib {

	namespace {
		constexpr size_t number_of_iterations = size_t(0xFFFF);
		constexpr size_t bufsiz = size_t(64);

		inline std::random_device random_device_; // obtain a random number from hardware
		inline std::mt19937 the_generator_(random_device_()); // seed the generator
		inline std::uniform_int_distribution<> from_25_to_64_(25, 63); // define the int range
	}

TU_REGISTER(
	[] {
		DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE); DBJ_PRINT(" " DBJ_RESET);

		DBJ_PRINT(DBJ_FG_GREEN_BOLD);
#ifdef __clang__
		DBJ_PRINT("\n\nCLANG %s", __clang_version__);
#else
		DBJ_PRINT("\n\nMSVC %d", _MSVC_LANG);
#endif
#ifdef _WIN64
		DBJ_PRINT(",64 bit");
#else
		DBJ_PRINT(",32 bit");
#endif
#ifdef NDEBUG
		DBJ_PRINT(", Release build\n");
#else
		DBJ_PRINT(", Debug build\n");
#endif
		DBJ_PRINT(DBJ_RESET);

		auto driver = [](char const* prompt_, auto test_) {
			size_t ctr_ = 0;
			time_t st = clock();
			DBJ_PRINT("\n%s", prompt_);
			while (ctr_++ < number_of_iterations) {
				test_(from_25_to_64_(the_generator_));
			}
			DBJ_PRINT(", format_to() total time is: %.3f seconds\n", double((clock() - st)) / CLOCKS_PER_SEC);
		};

		DBJ_PRINT("\nIterating %zu times. Test synopsis:\nfmt::format_to(buf.begin(), \"The answer is{}.\", number_);\n", number_of_iterations);
		DBJ_PRINT(DBJ_FG_GREEN_BOLD "\nNarrow buffers\n" DBJ_RESET );

		driver("using fmt::memory_buffer", [](int number_) {
			fmt::memory_buffer mb_;
			auto rezult = fmt::format_to(mb_, "The answer is {}.", number_);
			});

		driver("using std::vector<char>", [](int number_) {
			std::vector<char> mb_2_(bufsiz, char(0));
			mb_2_.resize(bufsiz);
			auto rezult = fmt::format_to(mb_2_.begin(), "The answer is {}.", number_);
			});

		driver("using std::unique_ptr<char[]>", [](int number_) {
			std::unique_ptr<char[]> mb_3_ = std::make_unique<char[]>(bufsiz);
			auto rezult = fmt::format_to(mb_3_.get(), "The answer is {}.", number_);
			});

		driver("using std::array<char, bufsiz>", [](int number_) {
			std::array<char, bufsiz> mb_3_{ {0} };
			auto rezult = fmt::format_to(mb_3_.begin(), "The answer is {}.", number_);
			});

		DBJ_PRINT(DBJ_FG_GREEN_BOLD "\nWide buffers\n" DBJ_RESET );

		driver("using fmt::wmemory_buffer", [](int number_) {
			fmt::wmemory_buffer mb_;
			auto rezult = fmt::format_to(mb_, L"The answer is {}.", number_);
			});

		driver("using std::vector<wchar_t>", [](int number_) {
			std::vector<wchar_t> mb_2_(bufsiz, char(0));
			mb_2_.resize(bufsiz);
			auto rezult = fmt::format_to(mb_2_.begin(), L"The answer is {}.", number_);
			});

		driver("using std::unique_ptr<wchar_t[]>", [](int number_) {
			std::unique_ptr<wchar_t[]> mb_3_ = std::make_unique<wchar_t[]>(bufsiz);
			auto rezult = fmt::format_to(mb_3_.get(), L"The answer is {}.", number_);
			});

		driver("using std::array<wchar_t, bufsiz>", [](int number_) {
			std::array<wchar_t, bufsiz> mb_3_{ {0} };
			auto rezult = fmt::format_to(mb_3_.begin(), "The answer is {}.", number_);
			});
		DBJ_PRINT("\nDone..\n");
	});
} // namespace dbj::nanolib 

