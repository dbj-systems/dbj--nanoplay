#pragma once
#include "../common.h"
/*
taming optional and any -- together
*/
namespace nano_play {

    using namespace std;
     // (c) 2019 by dbj.org -- free to use
        template<typename T>
    inline T optival
    (std::optional<T> opt, T dflt_ = T{})
        noexcept // daring
    {
        return opt.value_or(dflt_);
    }
   // (c) 2019 by dbj.org
        // Licence CC BY SA 4.0
        template<typename T>
    inline decltype(auto) any_opt(T v_)
    {
        // store the argument as the std::any
        std::any any_{ v_ };
        // the proxy function
        // returning the T & v_
        auto proxy_ = 
            [=]()->T const&
        {
            return v_;
        };
        // on first call return the proxy and any 
        // in the pair
        return make_pair(proxy_, any_);
    }


    inline auto const & driver = [](auto const & va_pair) {

        auto value_ = va_pair.first();
        auto any_   = va_pair.second;

        // move out
        return va_pair;
    };

    TU_REGISTER([] {
        DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE); DBJ_PRINT(" " DBJ_RESET);
 
        auto [proxy, anyval] = any_opt(42);
        int i42 = proxy();
        std::any any_42 = anyval; // std::any holding int 42

        auto the_va_pair = driver(any_opt(13));

     });

} // nano_play