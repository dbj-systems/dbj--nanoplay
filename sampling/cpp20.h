#pragma once

#include "../common.h"

#if _DBJ_HAS_CXX20
#ifdef _MSC_VER


namespace dbj::cpp20 {

	struct specimen final {
		using type = specimen ;
		using value_type = int ;

		value_type operator ()(value_type arg_) { return (arg_ + arg_); }

		value_type* begin() { return data_;	}
		value_type* end  () { return data_ + data_len_ - 1;	}

		value_type& operator [] (size_t idx_) noexcept { assert(idx_ < data_len_); return data_[idx_]; }
		value_type const& operator [] (size_t idx_) const noexcept { assert(idx_ < data_len_ ); return data_[idx_]; }

	private:
		constexpr static size_t data_len_ = 64 ;
		value_type data_[data_len_ - 1]{};
	};

	// here we define some fundamental concepts

	template< typename T> 
	concept has_call_operator = requires {	&T::operator();	};

	template<typename T>
	concept has_value_type = requires { typename T::value_type; };

	template<typename T>
	concept has_begin = requires { &T::begin; };

	template<typename T>
	concept has_end = requires { & T::end; };

	// here we use them together

	template< typename FunObj>
	auto use_fun_obj(FunObj fo_, typename FunObj::value_type value_ )
		requires ( 
			   has_call_operator< FunObj > 
			&& has_value_type< FunObj > 
			&& has_begin< FunObj > 
			&& has_end< FunObj >
			)
	{
		typename FunObj::value_type double_value_ = fo_(value_) ;

		for (auto& el_ : fo_)
			el_ = double_value_;

		return fo_;
	}

	TU_REGISTER(
		[] {
			using namespace std;
			auto spec_ = specimen{};
			auto ret_ = use_fun_obj( spec_, 21 );

			cout << endl;
			for (auto & e_ : ret_)
				cout << setw(3) << e_;
			cout << endl;
		}
	);

} // dbj::cpp20


#endif // _MSC_VER
#endif // _DBJ_HAS_CXX20
