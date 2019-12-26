#ifndef DBJ_STACK_ALLOCATOR_INC_
#define DBJ_STACK_ALLOCATOR_INC_

#ifndef DBJ_CPLUSPLUS
#if defined(_MSVC_LANG) && !defined(__clang__)
#define DBJ_CPLUSPLUS (_MSC_VER == 1900 ? 201103L : _MSVC_LANG)
#else
#define DBJ_CPLUSPLUS __cplusplus
#endif
#endif

#if ! (DBJ_CPLUSPLUS > 201402L )
#error C++17 or greater is required ...
#endif

// https://codereview.stackexchange.com/a/31575

#include <cstddef>
#include <cassert>
#include <vector>
#include <new>

namespace stackalloc_space 
{
    constexpr static const size_t MAX_ARENA{ 0xFFFF };

    /// <summary>
    /// stack memory arena
    /// adapted by dbjdbj from Howard Hinant
    /// </summary>
    template <std::size_t N>
    class stack_arena final
    {
    public:

        // constexpr std::size_t alignment = alignof(std::max_align_t);
        // same as
        constexpr static std::size_t alignment = alignof(double);
        using type = stack_arena;

        static_assert(N < MAX_ARENA, "max stack_arena size is 64kb");
    private:

        alignas(type::alignment) char buf_[N]{ 0 };

        char* ptr_{ buf_ };

        std::size_t align_up(std::size_t n) const noexcept
        {
            return (n + (alignment - 1)) & ~(alignment - 1);
        }

        bool pointer_in_buffer(char* p) const noexcept
        {
            return buf_ <= p && p <= buf_ + N;
        }

    public:

        static std::size_t size() noexcept { return N; }
        std::size_t used() const noexcept { return static_cast<std::size_t>(ptr_ - buf_); }
        void reset() { ptr_ = buf_; }

        char* allocate(std::size_t n)
        {
            assert(pointer_in_buffer(ptr_) && "stack_allocator has outlived stack_arena");
            n = align_up(n);
            if (size_t(buf_ + N - ptr_) >= n)
            {
                char* r = ptr_;
                ptr_ += n;
                return r;
            }
            return static_cast<char*>(::operator new(n));
        }

        void deallocate(char* p, std::size_t n) noexcept
        {
            assert(pointer_in_buffer(ptr_) && "stack_allocator has outlived stack_arena");
            if (pointer_in_buffer(p))
            {
                n = align_up(n);
                if (p + n == ptr_)
                    ptr_ = p;
            }
            else
                ::operator delete(p);
        }
    }; // stack_arena



    template <class T,  size_t size_template_arg >
    struct stack_allocator final
    {
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T value_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        using type = stack_allocator;
        using arena_type = stack_arena<size_template_arg>;

        constexpr static size_t size_{ size_template_arg };

        // The following must be the same for all allocators.
        template <typename U>
        struct rebind
        {
            typedef stack_allocator<U, size_template_arg> other;
        };

        // Default constructor, copy constructor, rebinding constructor, and destructor.
// Empty for stateless allocators.
        stack_allocator() { }

        stack_allocator(const stack_allocator&) { }

        template <typename U> stack_allocator(const stack_allocator<U, size_template_arg>&) { }

        ~stack_allocator() { }

        T* allocate(std::size_t n)
        {
#ifndef NDEBUG
            if (n == 0) {
                return nullptr;
            }
#endif
            return reinterpret_cast<T*>(arena_.allocate(n * sizeof(T)));
        }
        void deallocate(T* p, std::size_t n) noexcept
        {
            arena_.deallocate(reinterpret_cast<char*>(p), n * sizeof(T));
        }

        // template <typename U, std::size_t M> friend struct stack_allocator;
    private:
        arena_type arena_;
    }; // stack_allocator


} // namespace stackalloc_space 

#endif // DBJ_STACK_ALLOCATOR_INC_
