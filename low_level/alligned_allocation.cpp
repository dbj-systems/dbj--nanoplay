#include "../common.h"

/// https://thephd.github.io/freestanding-noexcept-allocators-vector-memory-hole

/// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/aligned-malloc?view=vs-2019

// Any R&D beyond this one file requires investment

#include <malloc.h>
#include <stdio.h>

namespace dbj::mem {

	// runtime check if x is power of 2
	inline bool is_power_of_two(size_t n)
	{
		if (n == 0) return false;
		return (ceil(log2(n)) == floor(log2(n)));
	}

	// Note alignment should be 2^N where N is any positive int.

	template< size_t exponent >
	struct power_of_two final {
		static_assert(exponent < 8, "for modern CPU allignment there is no reason to go over 2 ^ 7");
		constexpr static size_t value{ 1 << exponent };
	};

	template< size_t alignment_exponent >
	struct alloc final {

		constexpr static size_t alignment{ power_of_two<alignment_exponent>::value };

		static void* malloc(size_t sze_) {
			return _aligned_malloc(sze_, alignment);
		}

		static void* realloc(void* p_, size_t sze_) {
			return _aligned_realloc(p_, sze_, alignment);
		}

		static void free(void* p_) {
			_aligned_free(p_);
		}

		static bool is_aligned(void* ptr_) { return (((unsigned long long)ptr_ % alignment) == 0); }

	}; // alloc 

	template< size_t alignment_exponent, size_t offset >
	struct alloc_offset final {

		constexpr static size_t alignment{ power_of_two<alignment_exponent>::value };

		constexpr static size_t off_set{ offset };

		static void* malloc(size_t sze_) {
			return _aligned_offset_malloc(sze_, alignment, off_set);
		}

		static void* realloc(void* p_, size_t sze_) {
			return _aligned_offset_realloc(p_, sze_, alignment, off_set);
		}

		static void free(void* p_) {
			_aligned_free(p_);
		}

		static bool is_aligned_and_offset(void* ptr_) { return (((((unsigned long long)ptr_) + off_set) % alignment) == 0); }

	}; // alloc_offset

} // dbj::mem

TU_REGISTER_NOT([]
	{
		DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE); DBJ_PRINT(" " DBJ_RESET);

		/// size_t  alignment = 64, off_set = 5;

		using aligned = dbj::mem::alloc<6>;
		using aligned_ofst = dbj::mem::alloc_offset<6, 5>;

		auto report = [&](void* ptr_) -> void* {
			if (ptr_ == nullptr)
			{
				DBJ_PRINT("Error allocating aligned memory.");
				return nullptr;
			}

			DBJ_PRINT("This pointer, %p, is %s aligned on %zu\n",
				ptr_, (aligned::is_aligned(ptr_) ? "" : "not"), aligned::alignment);

			return ptr_;
		};

		auto report_2 = [&](void* ptr_) -> void* {
			if (ptr_ == NULL)
			{
				DBJ_PRINT("Error allocation aligned offset memory.");
				return nullptr;
			}
			DBJ_PRINT("This pointer, %p, is %s offset by %zu on alignment of %zu\n",
				ptr_, (aligned_ofst::is_aligned_and_offset(ptr_) ? "" : "not"), aligned_ofst::off_set, aligned_ofst::alignment);

			return ptr_;
		};

		void* ptr{};

		// Using _aligned_malloc
		ptr = report(aligned::malloc(100) /* _aligned_malloc(100, alignment)*/);

		if (!ptr) return;
		// Using _aligned_realloc
		ptr = report(aligned::realloc(ptr, 200)   /*_aligned_realloc(ptr, 200, alignment)*/);

		aligned::free(ptr);

		// Using _aligned_offset_malloc
		ptr = report_2(aligned_ofst::malloc(100));
		if (ptr == nullptr)
			return;

		// Using _aligned_offset_realloc
		ptr = report_2(aligned_ofst::realloc(ptr, 200));

		aligned_ofst::free(ptr);

	});

void aligned_allocation_test()
	{
		DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE); DBJ_PRINT(" " DBJ_RESET);

		enum { ASZ = 16 };

// https://stackoverflow.com/a/24274753/10870835
// make a nice 16 align macro
#ifndef ALIGN16
#define ALIGN16 __declspec(align(ASZ))
#endif

// align the structure
		struct ALIGN16 CB {
			ALIGN16 bool m1; // and
			ALIGN16 int m2; // align
			ALIGN16 int m3; // each
			ALIGN16 short m4; // element
		};

		printf("\n"
			"\n	struct ALIGN16 CB {"
			"\n		ALIGN16 bool m1; // and"
			"\n		ALIGN16 int m2; // align"
			"\n		ALIGN16 int m3; // each"
			"\n		ALIGN16 short m4; // element"
			"\n	};"
			"\n\n");

		#define PFMT "%4x"
		#define FMT "%llu"
		// now it performs as expected
		printf("sizeof(CB): " FMT "\n", sizeof(CB));
		CB vCb;
		printf("CB:    " PFMT ", " FMT "\n", &vCb,    (UINT_PTR)&vCb    % ASZ);
		printf("CB.m1: " PFMT ", " FMT "\n", &vCb.m1, (UINT_PTR)&vCb.m1 % ASZ);
		printf("CB.m2: " PFMT ", " FMT "\n", &vCb.m2, (UINT_PTR)&vCb.m2 % ASZ);
		printf("CB.m3: " PFMT ", " FMT "\n", &vCb.m3, (UINT_PTR)&vCb.m3 % ASZ);
		printf("CB.m4: " PFMT ", " FMT "\n", &vCb.m4, (UINT_PTR)&vCb.m4 % ASZ);

		{
			enum { ASZ = 64 };
			#define ALIGN64 __declspec(align(ASZ))

			struct ALIGN64 CB {
				ALIGN64 bool m1; // and
				ALIGN64 int m2; // align
				ALIGN64 int m3; // each
				ALIGN64 short m4; // element
			};

			printf("\n"
				"\n	struct ALIGN64 CB {"
				"\n		ALIGN64 bool m1; // and"
				"\n		ALIGN64 int m2; // align"
				"\n		ALIGN64 int m3; // each"
				"\n		ALIGN64 short m4; // element"
				"\n	};"
				"\n\n");

#define PFMT "%4x"
#define FMT "%llu"
			// now it performs as expected
			printf("sizeof(CB): " FMT "\n", sizeof(CB));
			CB vCb;
			printf("CB:    " PFMT ", " FMT "\n", &vCb, (UINT_PTR)&vCb % ASZ);
			printf("CB.m1: " PFMT ", " FMT ", size: " FMT "\n", &vCb.m1, (UINT_PTR)&vCb.m1 % ASZ, sizeof(vCb.m1));
			printf("CB.m2: " PFMT ", " FMT "\n", &vCb.m2, (UINT_PTR)&vCb.m2 % ASZ);
			printf("CB.m3: " PFMT ", " FMT "\n", &vCb.m3, (UINT_PTR)&vCb.m3 % ASZ);
			printf("CB.m4: " PFMT ", " FMT "\n", &vCb.m4, (UINT_PTR)&vCb.m4 % ASZ);

		}

}