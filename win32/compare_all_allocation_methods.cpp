
// MSVC STD LIB aka STL
// _ITERATOR_DEBUG_LEVEL 2 is the DEBUG build default
// and that will produce segv with dbj stack alocator 
// that was not tested with stack allocator I suppose?
// https://docs.microsoft.com/en-us/cpp/standard-library/iterator-debug-level?view=vs-2019

#if (_ITERATOR_DEBUG_LEVEL != 0)
#undef _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL  0
#endif

// https://docs.microsoft.com/en-us/cpp/parallel/concrt/how-to-use-alloc-and-free-to-improve-memory-performance?view=vs-2019
// allocators.cpp
// compile with: /EHsc 
#define STRICT  1
#include <windows.h>


#define DBJ_NANO_CALLOC(N_, TSIZE_) (T_*)HeapAlloc(GetProcessHeap(), 0, N_ * TSIZE_ )

#define DBJ_NANO_MALLOC(S_)HeapAlloc(GetProcessHeap(), 0, S_)

#define DBJ_NANO_FREE(P_) HeapFree(GetProcessHeap(), 0, (void*)P_)

// #undef DBJ_PPL
#define DBJ_PPL

#ifdef DBJ_PPL
#include <ppl.h>
#endif




#include <crtdbg.h>
#include <iostream>
// #include <vector>
#include <algorithm> // swap()

#if (_ITERATOR_DEBUG_LEVEL != 0)
#undef _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL  0
#endif


#undef MAKE_IT_SLOW
#define MAKE_IT_SLOW

// DBJ added anonymous namespace 
namespace {

#ifdef DBJ_PPL  
    using namespace concurrency;
#endif

    using namespace std;

    // A type that defines the new and delete operators. These operators 
    // call the global new and delete operators, respectively.
    struct new_delete final
    {
        static void* operator new(size_t size)
        {
            return ::operator new(size);
        }

        static void operator delete(void* p)
        {
             ::operator delete(p);
        }

        int _data;
    };

    // A type that defines the new and delete operators. These operators 
    // call the C Runtime malloc and free functions, respectively.
    struct malloc_free final
    {
        static void* operator new(size_t size)
        {
            return malloc(size);
        }
        static void operator delete(void* p)
        {
             free(p);
        }

        int _data;
    };

#ifdef DBJ_PPL
    // A type that defines the new and delete operators. These operators 
    // call the Concurrency Runtime Alloc and Free functions, respectively.
    struct Alloc_Free final
    {
    public:
        static void* operator new(size_t size)
        {
            return Alloc(size);
        }
        static void operator delete(void* p)
        {
             Free(p);
        }

        int _data;
    };
#endif // ! DBJ_PPL
    // A type that defines the new and delete operators. 
    struct dbj_alloc_free final
    {
    public:
        static void* operator new(size_t size)
        {
            return DBJ_NANO_MALLOC(size);
        }
        static void operator delete(void* p)
        {
            DBJ_NANO_FREE(p);
        }

        int _data;
    };

    struct guardian final 
    {
        guardian( void * specimen_ ) : data(specimen_)  {}
        ~guardian( ) {}

            void* data{};
    };

    // Calls the provided work function and returns the number of milliseconds 
    // that it takes to call that function.
    template <class Function>
    constexpr __int64 timed_call(Function&& f)
    {
        __int64 begin = GetTickCount64();
        f();
        return GetTickCount64() - begin;
    }

#ifdef MAKE_IT_SLOW
    // Exchanges the contents of a[index1] with a[index2].
    // deliberate slow-down in here 
    template<class T>
    void swap(T* a, int index1, int index2)
    {
        // For serious slow down, 
        // allocate memory from the heap.
        // again
        T* temp = new T;

        *temp = a[index1];
        a[index1] = a[index2];
        a[index2] = *temp;

        delete temp;
    }
#endif // MAKE_IT_SLOW

    constexpr  const int size_ = 0xFFFF * 100 ; // six and a half mil, approx
    constexpr  const int size_half_ = size_ / 2;
    constexpr  const int repeat = 11;
    // Computes the time that it takes to reverse the elements of a 
    // large array of the specified type.
    template <typename T>
    constexpr __int64 reverse_array()
    {

#undef SWAP
#ifdef MAKE_IT_SLOW
#define SWAP swap(a, index, size_ - index - 1)
#else // MAKE_IT_SLOW
using std::swap;
#define SWAP std::swap(a[index], a[size_ - index - 1])
#endif // ! MAKE_IT_SLOW

        T* a = new T[size_];

        __int64 time = 0;

        // Repeat the operation several times to amplify the time difference.
        for (int i = 0; i < repeat; ++i)
        {
            time += timed_call([&] {
#ifdef DBJ_PPL                
                parallel_for(0, size_half_,
                    [&](int index) {
                        SWAP;
                    });
#else  // ! DBJ_PPL                    
        for(int j = 0; j < size_half_; ++j )
            [&](int index) {   SWAP;  } ;
#endif //  DBJ_PPL
                });
        }
#undef SWAP
        delete[] a;
        return time;
    }

} // anon ns
// -----------------------------------------------------------------------------

extern "C" int compare_all_allocation_methods( int argc, const char ** argv )
{  
#if defined (__clang__) || defined(__GNUC__)
    wcout << "\n" << __VERSION__ << "\n\n" ;
#else
    wcout << "\nMSVC " << _MSC_VER << "\n\n";
#endif

   // Compute the time that it takes to reverse large arrays of 
   // different types.

   // new_delete
   wcout << L"Took " << reverse_array<new_delete>() 
         << " ms with new/delete." << endl;

   // malloc_free
   wcout << L"Took " << reverse_array<malloc_free>() 
         << " ms with malloc/free." << endl;

#ifdef DBJ_PPL
   // Alloc_Free
   wcout << L"Took " << reverse_array<Alloc_Free>() 
         << " ms with PPL Alloc/Free." << endl;
#endif // ! DBJ_PPL

   // dbj_alloc_free
   wcout << L"Took " << reverse_array<dbj_alloc_free>()
         << " ms with win32 heap alloc free" << endl;

   system("@echo .");
   system("@echo .");
   system("@pause");

   return 42;
}