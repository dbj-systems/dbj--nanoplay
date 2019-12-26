#ifndef DBJ_ALIGNED_ALLOCATOR_INC_
#define DBJ_ALIGNED_ALLOCATOR_INC_

#ifdef _WIN32
#include <malloc.h>
#endif
#include <cstdint>
#include <vector>

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

// https://gist.github.com/donny-dont/1471329

namespace allignedalloc_space {

	/**
	 * Allocator for aligned data.
	 *
	 * Modified from the Mallocator from Stephan T. Lavavej.
	 * <http://blogs.msdn.com/b/vcblog/archive/2008/08/28/the-mallocator.aspx>
	 */
	template <typename T, std::size_t Alignment>
	class aligned_allocator
	{
	public:

		// The following will be the same for virtually all allocators.
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;
		typedef std::size_t size_type;
		typedef ptrdiff_t difference_type;

		T* address(T& r) const
		{
			return &r;
		}

		const T* address(const T& s) const
		{
			return &s;
		}

		std::size_t max_size() const
		{
			// The following has been carefully written to be independent of
			// the definition of size_t and to avoid signed/unsigned warnings.
			return (static_cast<std::size_t>(0) - static_cast<std::size_t>(1)) / sizeof(T);
		}


		// The following must be the same for all allocators.
		template <typename U>
		struct rebind
		{
			typedef aligned_allocator<U, Alignment> other;
		};

		bool operator!=(const aligned_allocator& other) const
		{
			return !(*this == other);
		}

		void construct(T* const p, const T& t) const
		{
			void* const pv = static_cast<void*>(p);

			new (pv) T(t);
		}

		void destroy(T* const p) const
		{
			p->~T();
		}

		// Returns true if and only if storage allocated from *this
		// can be deallocated from other, and vice versa.
		// Always returns true for stateless allocators.
		bool operator==(const aligned_allocator& other) const
		{
			return true;
		}


		// Default constructor, copy constructor, rebinding constructor, and destructor.
		// Empty for stateless allocators.
		aligned_allocator() { }

		aligned_allocator(const aligned_allocator&) { }

		template <typename U> aligned_allocator(const aligned_allocator<U, Alignment>&) { }

		~aligned_allocator() { }


		// The following will be different for each allocator.
		T* allocate(const std::size_t n) const
		{
			// The return value of allocate(0) is unspecified.
			// Mallocator returns NULL in order to avoid depending
			// on malloc(0)'s implementation-defined behavior
			// (the implementation can define malloc(0) to return NULL,
			// in which case the bad_alloc check below would fire).
			// All allocators can return NULL in this case.
			if (n == 0) {
				return NULL;
			}

			// All allocators should contain an integer overflow check.
			// The Standardization Committee recommends that std::length_error
			// be thrown in the case of integer overflow.
			// alas DBJDBJ does not throw, so he will just calmly exit the app
			if (n > max_size())
			{
				perror("aligned_allocator<T>::allocate() - Integer overflow.");
				exit( EXIT_FAILURE);
			}

			// Mallocator wraps malloc().
			void* const pv = _mm_malloc(n * sizeof(T), Alignment);

			// Allocators should throw std::bad_alloc in the case of memory allocation failure.
			// alas DBJDBJ does not throw, so he will just calmly exit the app
			if (pv == NULL)
			{
				perror("aligned_allocator<T>::allocate() - memory allocation failure");
				exit(EXIT_FAILURE);
			}

			return static_cast<T*>(pv);
		}

		void deallocate(T* const p, const std::size_t n) const
		{
			_mm_free(p);
		}


		// The following will be the same for all allocators that ignore hints.
		template <typename U>
		T* allocate(const std::size_t n, const U* /* const hint */) const
		{
			return allocate(n);
		}


		// Allocators are not required to be assignable, so
		// all allocators should have a private unimplemented
		// assignment operator. Note that this will trigger the
		// off-by-default (enabled under /Wall) warning C4626
		// "assignment operator could not be generated because a
		// base class assignment operator is inaccessible" within
		// the STL headers, but that warning is useless.
	private:
		aligned_allocator& operator=(const aligned_allocator&);
	}; // aligned_allocator

} // namespace allignedalloc_space 

#endif // DBJ_ALIGNED_ALLOCATOR_INC_
