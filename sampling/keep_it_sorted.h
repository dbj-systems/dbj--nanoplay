#pragma once

#include "../common.h"

#include <list>
#include <deque>
#include <string>
#include <iterator>
// https://stackoverflow.com/a/54383242/10870835
#include <iostream>

#pragma region tuple print

#include <tuple>

namespace detail {
	template<class TupType, size_t... I>
	inline
		std::ostream& tuple_print(std::ostream& os,
			const TupType& _tup, std::index_sequence<I...>)
	{
		os << "(";
		(..., (os << (I == 0 ? "" : ", ") << std::get<I>(_tup)));
		os << ")";
		return os;
	}
}

template<class... T>
inline 
std::ostream& operator<< (std::ostream& os, const std::tuple<T...>& _tup)
{
	return detail::tuple_print(os, _tup, std::make_index_sequence<sizeof...(T)>());
}

#pragma endregion tuple print

/*
The problem statement

Imagine we have a sequence container (such as vector, deque, list, string, …any other STL-compatible custom container)
that has already been sorted. Operating on sorted containers is quite frequent in day-to-day code. And imagine we have
some objects to be added to the container. It may be one or several objects or a range (a container) of objects
(in general case, unsorted). It’s important that after all these insertions our container should remain sorted.

The solution
*/

namespace dbj_nanoplay {

	namespace detail {

		/*
		sorted means lowermost is on the left
		incoming has 3 siutations: append, prepend and insert
		*/
		template<typename Sequence >
		inline void push_(Sequence& sorted_, typename Sequence::value_type incoming_)
		{
			// prepend
			if (incoming_ < sorted_.front())
			{
				sorted_.push_front(incoming_);
				return;
			}
			// append
			if (sorted_.back() < incoming_) {
				sorted_.push_back(incoming_);
				return;
			}
			// insert left to the first element bigger to incoming
			auto walker = sorted_.begin();
			while (*walker < incoming_) walker++;
			sorted_.insert(walker, incoming_);

		}
	} // detail

		template<typename Sequence >
		inline void assign_sorted(Sequence& sorted_, Sequence added_) {
			for (auto element_ : added_)
			{
				detail::push_(sorted_, element_);
			}
		}


		template<typename Iterator >
		inline void sequence_print( Iterator begin_ , Iterator end_ , bool show_size = true ) {

			using namespace std;

			auto size_ = [&] { return  size_t( distance( begin_, end_ ) );  };

			if (show_size) cout << " sequence [size:" << size_() << "]";

			cout << " {";
			auto walker = begin_ ;
			if (walker != end_ )
			{
				// first sequence element
				// no leading comma
				cout << " " << *walker; 
				walker++;
				while (walker != end_ ) {
					cout << " , " << *walker;
					walker++;
				}
			}
			cout << " }";
		}

		// print a sequence with a comma in between elements
	template<typename Sequence >
	inline void sequence_print(Sequence const & seq_, bool show_size = true ) {
		using namespace std;
		sequence_print( begin(seq_) , end( seq_ ) , show_size );
	}

	namespace {

		using namespace std;

		template <template< typename> typename sequence_type, typename ... Args  >
		inline void variadicoid (
			Args ... sorted_args_, 
			Args ... unsorted_args_
		)
		{
			sequence_type sorted_{ sorted_args_  ...   };
			sequence_type added_{ unsorted_args_  ...  };
		}	

		// call
		// driver< list >({1,2,3},{7,5,4}) ;
		//
		template <template< typename > typename SEQ, typename value_type >
		inline void driver(
			initializer_list<value_type> sorted_args_, 
			initializer_list<value_type> unsorted_args_
		)
		{
			SEQ sorted_( sorted_args_.begin(), sorted_args_.end()    );
			SEQ added_( unsorted_args_.begin(), unsorted_args_.end() );

			assign_sorted(sorted_, added_);

			printf("\nMerged sorted\n");sequence_print(sorted_);	printf("\n");
		}

		// second call does it all
		template <template< typename> typename SEQ, typename ... Args  >
		auto two_calls ( Args ... sorted_args_ )
		{
			// return second lambda to be called
			// to actually do the job
			// after it takes the second param pack
			return [=](	auto ... unsorted_args_ ) 
			{
				    // make sequence instances
				SEQ sorted_ = { sorted_args_ ... };
				SEQ unsorted_ = { unsorted_args_ ... };

				assign_sorted(sorted_, unsorted_);

				printf("\ntwo_calls merged sorted\n"); sequence_print(sorted_); printf("\n");
			};
		};

		// second call does it all
		template <typename ... Args  >
		auto two_ppacks(Args ... ppack_1 )
		{
			// return second lambda to be called
			// to actually do the job
			// after it takes the second param pack
			return [=](auto ... ppack_2 )
			{
				// make sequence instances
				auto args_set_1 = make_tuple( ppack_1 ... );
				auto args_set_2 = make_tuple( ppack_2 ... );

				cout << endl << " First  ppack: " << boolalpha << args_set_1 ;
				cout << endl << " Second ppack: " << boolalpha << args_set_2 << endl;

				return tuple_cat(args_set_1, args_set_2);
			};
		};

		TU_REGISTER([] 
			{
				using namespace std;
			//driver< std::list  >({ 1,3,5 }, { 6,4,2 });
			//// driver< std::deque >({ 1,3,5 }, { 6,4,2 });

			//two_calls<std::list>(1, 3, 5)(6, 4, 2);

			auto merged_pack = two_ppacks(1, false, 'X')(6.32f, "Hola!", 3.14156);
			cout << endl << " Merged  ppack: " << boolalpha << merged_pack;

			//variadicoid<list>(1,2);

			//char charr[]{'A','B','C','D'};

			//sequence_print( charr );
			//
			//printf("\n\n\n");
			//sequence_print(string{"STD::STRING"});
			//printf("\n\n\n");

		});

	}
}