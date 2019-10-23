#ifndef _INC_VAL_STAT_OPTIREF_
#define _INC_VAL_STAT_MONSTER_

#include "../dbj--nanolib/dbj++tu.h"
//#include <optional>
//#include <utility>

namespace dbj {

	using namespace std;

	/*
	OPTIREF
	--------------------------------------------------------------------------------
	solution for optional of references
	*/
	template< typename T>
	using optiref = optional< reference_wrapper< T >>;

	// template guides are not allowed on templated aliases
	// https://stackoverflow.com/questions/41008092/class-template-argument-deduction-not-working-with-alias-template
	// template <class T>
	// optiref<T>->optiref<T>{};
	// ditto ...
	template<typename T>
	inline auto make_optiref( T & tref_) -> optiref<T>
	{
		return { tref_ };
	}

	// disalow temporaries
	template<typename T> auto make_optiref(T&& tref_)->optiref<T> = delete;

	/*
	GENERIC OPTI_PAIR SOLUTION
	------------------------------------------------------------------------------

	OPTI_PAIR == Pair of Optionals

	generic version for no copy / no move Types
	that are also potentially non copy/no move kind
	*/
	template< typename T, typename S>
	using generic_optipair_type = pair< optional<T>, optional<S> >;

	/*
	generic factory
	*/
	template<typename T, typename S>
	inline  auto make_generic_optipair(optional<T> val_ = {}, optional<S> stat_ = {})
		-> generic_optipair_type<T, S>
	{
		return { val_ , stat_ };
	}
	/*
	generic factory for no move/no copy values
	*/
	template<typename T, typename S>
	inline  auto make_generic_optipair( reference_wrapper<T> val_, optional<S> stat_ = {} )
		-> generic_optipair_type<T, S>
	{
		return { make_optiref(val_) , stat_ };
	}
	/*
	dissalow temporary val's
	*/
	//template<typename T, typename S>
	//inline  auto make_generic_optipair(T&& val_, optional<S> stat_ = {})
	//	->generic_optipair_type<T, S> = delete;

	//template<typename T, typename S>
	//inline  auto make_generic_optipair(reference_wrapper<T> && val_, optional<S> stat_ = {})
	//	->generic_optipair_type<T, S> = delete;

	/*
	TESTING GENERIC NOT VALSTAT VERSION
	------------------------------------------------------------------------------
     */
	namespace {
		struct adamant final{
			inline static const char* genus = "tenacious";
			adamant(const adamant&) = delete;
			adamant & operator = (const adamant&) = delete;
			adamant(adamant&&) = delete;
			adamant& operator = (adamant&&) = delete;

			using valstat = generic_optipair_type< reference_wrapper<adamant> , int >;
		};

	}

	TU_REGISTER([] {
			adamant steadfast{};

			auto info = [&]( ) -> adamant::valstat { return  { {steadfast}, {42} }; };

			auto error = [&]( ) -> adamant::valstat { return  { {}, {42} }; };

			auto ok = [&]( ) -> adamant::valstat { return  { {steadfast}, {} }; };

			auto empty = [&]( ) -> adamant::valstat { return  { {}, {} }; };

			auto consumer = []( auto producer ) {
				auto [val, stat] = producer();
			};

			consumer(info);
			consumer(error);
			consumer(ok);
			consumer(empty);

		});
#if SPECIFIC_VALSTAT_VERSION
	/*
	SPECIFIC VALSTAT VERSION
	------------------------------------------------------------------------------
	status == json formated buffer
	note: string is not made to be a buffer
	as anything else in standard C++, buffer type of choice 
	has to be movable, so we pass it by value
    */
	using valstat_status_type = typename v_buffer::buffer_type;

	template< typename T >
	using valstat_optipair_type = pair< optiref<T>, optional<valstat_status_type> >;

	template<typename T >
	inline auto make_valstat_pair(optional<T> val_ = {}, optional<valstat_status_type> stat_ = {})
		-> valstat_optipair_type<T>
	{
		return { val_ , stat_ };
	}
	/*
	variant for references
	*/
	template<typename T >
	inline auto make_valstat_pair(T& val_, optional<valstat_status_type> stat_ = {})
		-> valstat_optipair_type<T>
	{
		return { make_optiref(val_) , stat_ };
	}

	/*
	dissalow temporaries
	*/
	template<typename T >
	auto make_valstat_pair(T&& val_, optional<valstat_status_type> stat_ = {})
		->valstat_optipair_type<T> = delete;


	TU_REGISTER([] {

		using vorf = valstat_optipair_type<int>;

		auto empty = vorf{ } ;
		auto empty2 = vorf() ;

		int fty2 = 42;
		string info_msg = "INFO!";

		// slow motion
		auto one = optiref<int>{ fty2 };
		auto two = optiref<string>{ info_msg };
		auto info = vorf{ one, two };

		// or
		auto info_2 = vorf{ optiref<int>{ fty2 }, optiref<string>{ info_msg } };

		// or simply?
		auto monster_info = vorf{ make_optiref(fty2), make_optiref( info_msg )  };

		// crazy simple ?
		auto bijou = make_optiref_pair(fty2, info_msg );

		});
#endif // SPECIFIC_VALSTAT_VERSION

}

#endif // !_INC_VAL_STAT_MONSTER_



