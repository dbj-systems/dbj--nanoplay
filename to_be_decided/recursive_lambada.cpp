
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////
static void tprintf(const char* format) // base function
{
    std::cout << format;
}

template<typename T, typename... Targs>
static void tprintf(const char* format, T value, Targs... Fargs) // recursive variadic function
{
    for (; *format != '\0'; format++) {
        if (*format == '%') {
            std::cout << value;
            tprintf(format + 1, Fargs...); // recursive call
            return;
        }
        std::cout << *format;
    }
}

////////////////////////////////////////////////////////////////////////////////////
#ifndef __clang__
static auto print = []( auto const& first, auto const & ... args)
{
    auto inner = [&](auto const& first, auto const & ... args) 
    {
        std::cout << ' ' << first;

        if constexpr (sizeof...(args) > 0)
        {
            print(args...);
        }
        return print;
    };
    return inner( first, args ... ) ;
};
#endif // _MSC_VER

////////////////////////////////////////////////////////////////////////////////////
struct printer final {

    template<typename T, typename ... A>
    printer& operator () (T first, A ... args) {
        std::cout << ' ' << first;

        if constexpr (sizeof...(args) > 0)
        {
            (*this)(args...);
        }
        return *this;
    }
};
////////////////////////////////////////////////////////////////////////////////////
extern "C" int recursive_lambada()
{
#ifndef __clang__
    tprintf("_MSC_FULL_VER : %\n\n", _MSC_FULL_VER);
    print("Hello ", "fancy ", "recursive ", "function ")("from ")("clang ")("and ", "GCC");
#endif

    printer arak{};

    arak("bambi", "bimbi", "bumbi")("bumbele")("njaso");

    return 42;
}
