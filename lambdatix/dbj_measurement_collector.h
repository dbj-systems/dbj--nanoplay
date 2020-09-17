
#ifndef DBJ_MEASURE_NANO_LIB_
#define DBJ_MEASURE_NANO_LIB_

// NOTE: NDEBUG is standard !
#if !defined( _DEBUG ) &&  !defined( DEBUG ) && !defined(NDEBUG) 
#define NDEBUG
#endif // !_DEBUG and !DEBUG and NDEBUG

#ifdef __clang__
#pragma clang system_header
#endif // __clang__

/// ---------------------------------------------------------------------
#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/// ---------------------------------------------------------------------

namespace dbj::timing {

	struct collector final {
		char name_[0xFF]{ 0 };
		float min_{};
		float max_{};
		int count_{};

		explicit collector(const char* newname) noexcept : min_(0), max_(0), count_(0) {
			strncpy_s(name_, newname, strlen(newname));
		}

		void add(float new_time_)
		{
			if (count_ == 0) {
				min_ = new_time_;
				max_ = new_time_;
				count_ += 1;
				return;
			}
			count_ += 1;
			if (new_time_ < min_)
				min_ = new_time_;
			else if (new_time_ > max_)
				max_ = new_time_;
			else {/* no op */ }
		}

		static void report(collector& clctr_, void (*cb) (const char*, float, float, int)) {
			cb(clctr_.name_, clctr_.min_, clctr_.max_, clctr_.count_);
		}

	};
	/// ---------------------------------------------------------------------
	static inline int randomizer(int max_ = 0xFF, int min_ = 1)
	{
		static auto _ = [] {
			srand((unsigned)time(NULL)); return true;
		}();

		return (rand() % max_ + min_);
	}
	/// ---------------------------------------------------------------------
	static inline auto driver = [](collector& clctr_, auto specimen)
	{
		volatile clock_t time_point_ = clock();
		specimen();
		/// float rez = (float)(clock() - time_point_) / CLOCKS_PER_SEC;
		clctr_.add((float)(clock() - time_point_) / CLOCKS_PER_SEC);
		/// printf("%12s %.3f sec ", prompt_, rez);
	};
} // dbj::timing
/// ---------------------------------------------------------------------
#endif // !DBJ_MEASURE_NANO_LIB_
