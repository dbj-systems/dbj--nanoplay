#include "../common.h"

/// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/aligned-malloc?view=vs-2019

// Any R&D beyond this one file requires investment

#include <malloc.h>
#include <stdio.h>

namespace dbj::mem {

    // runtime check if x is power of 2
    inline bool is_power_of_two( size_t n)
    {
        if (n == 0) return false;
        return (ceil(log2(n)) == floor(log2(n)));
    }

    // Note alignment should be 2^N where N is any positive int.

    template< size_t exponent >
    struct power_of_two final {
        static_assert( exponent < 8, "for modern CPU allignment there is no reason to go over 2 ^ 7");
        constexpr static size_t value{ 1 << exponent  };
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

        static bool is_aligned( void * ptr_ ) { return (((unsigned long long)ptr_ % alignment) == 0); }

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

        static bool is_aligned_and_offset( void * ptr_ ) { return (((((unsigned long long)ptr_) + off_set) % alignment) == 0); }

    }; // alloc_offset

} // dbj::mem

TU_REGISTER([] 
    {

    /// size_t  alignment = 64, off_set = 5;

    using aligned = dbj::mem::alloc<6>;
    using aligned_ofst = dbj::mem::alloc_offset<6, 5>;

    auto report = [&](void* ptr_ ) -> void * {
        if (ptr_ == nullptr)
        {
            DBJ_PRINT("Error allocating aligned memory.");
            return nullptr ;
        }

         DBJ_PRINT("This pointer, %p, is %s aligned on %zu\n",
                ptr_, ( aligned::is_aligned(ptr_) ? "" : "not" ), aligned::alignment);

         return ptr_;
    };

    auto report_2 = [&](void* ptr_) -> void* {
        if (ptr_ == NULL)
        {
            DBJ_PRINT("Error allocation aligned offset memory.");
            return nullptr ;
        }
        DBJ_PRINT("This pointer, %p, is %s offset by %zu on alignment of %zu\n",
            ptr_, (aligned_ofst::is_aligned_and_offset(ptr_) ? "" : "not"), aligned_ofst::off_set, aligned_ofst::alignment);

        return ptr_;
    };

    void* ptr{};

    // Using _aligned_malloc
    ptr = report(  aligned::malloc(100) /* _aligned_malloc(100, alignment)*/ ) ;

    if (!ptr) return;
    // Using _aligned_realloc
    ptr = report( aligned::realloc(ptr, 200)   /*_aligned_realloc(ptr, 200, alignment)*/);
    
    aligned::free(ptr); 

    // Using _aligned_offset_malloc
    ptr = report_2(  aligned_ofst::malloc(100) );
    if (ptr == nullptr )
        return;

    // Using _aligned_offset_realloc
    ptr =  report_2(aligned_ofst::realloc(ptr, 200));

    aligned_ofst::free(ptr);

    });

