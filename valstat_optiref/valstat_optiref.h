#ifndef _INC_VAL_STAT_MONSTER_
#define _INC_VAL_STAT_MONSTER_

#include "../dbj--nanolib/dbj++tu.h"
//#include <optional>
//#include <utility>

namespace dbj {

	using namespace std;

	template< typename T>
	using optiref = optional< reference_wrapper< T >>;

	// no template guides are not allowed on templated aliases
	// https://stackoverflow.com/questions/41008092/class-template-argument-deduction-not-working-with-alias-template
	// template <class T>
	// optiref<T>->optiref<T>{};
	// ditto ...
	template<typename T>
	auto make_optiref( T & tref_) -> optiref<T>
	{
		return { tref_ };
	}

	// dissalow temporaries
	template<typename T> auto make_optiref(T&& tref_)->optiref<T> = delete;

	template< typename T>
	using valstat_optiref_type = pair< optiref<T>, optiref<string>  >;

	template<typename T>
	auto make_valstat_optiref(T& val_, string status_ = {}) 
		-> valstat_optiref_type<T>
	{
		return { make_optiref(val_) , make_optiref(status_) };
		//auto opti1 = make_optiref(val_);
		//auto opti2 = make_optiref(status_);
		//return make_pair( opti1, opti2 );
	}


	TU_REGISTER([] {

		using vsm1 = valstat_optiref_type<int>;

		auto empty = vsm1{ } ;
		auto empty2 = vsm1() ;

		int fty2 = 42;
		string info_msg = "INFO!";
		auto one = optiref<int>{ fty2 };
		auto two = optiref<string>{ info_msg };
		auto info = vsm1{ one, two };

		// or
		auto info_2 = vsm1{ optiref<int>{ fty2 }, optiref<string>{ info_msg } };

		// or simply?
		auto monster_info = vsm1{ make_optiref(fty2), make_optiref( info_msg )  };

		// crazy simple ?
		auto bijou = make_valstat_optiref( fty2 );

		});
}

#endif // !_INC_VAL_STAT_MONSTER_



