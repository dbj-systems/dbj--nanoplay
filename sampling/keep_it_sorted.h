#pragma once

#include "../common.h"

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
		incoming has 3 states possible: append, prepend and insert
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

			if (show_size) DBJ_PRINT(" sequence [size:" ,size_() ,"]");

			DBJ_PRINT(" {");
			auto walker = begin_ ;
			if (walker != end_ )
			{
				// first sequence element
				// no leading comma
				DBJ_PRINT(" " ,*walker);
				walker++;
				while (walker != end_ ) {
					DBJ_PRINT(" , " ,*walker);
					walker++;
				}
			}
			DBJ_PRINT(" }");
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

			DBJ_PRINT("\nMerged sorted\n");sequence_print(sorted_);	DBJ_PRINT("\n");
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

				DBJ_PRINT("\ntwo_calls merged sorted\n"); sequence_print(sorted_); DBJ_PRINT("\n");
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

				::dbj::nanolib::ostrmng::prinf(" First  ppack: " ,boolalpha ,args_set_1 );
				::dbj::nanolib::ostrmng::prinf(" Second ppack: " ,boolalpha ,args_set_2);

				return tuple_cat(args_set_1, args_set_2);
			};
		};

		TU_REGISTER([]
			{
				DBJ_PRINT(DBJ_FG_CYAN_BOLD DBJ_FILE_LINE); DBJ_PRINT(" " DBJ_RESET);
				using namespace std;
				using ::dbj::nanolib::ostrmng::prinf;
			//driver< std::list  >({ 1,3,5 }, { 6,4,2 });
			//// driver< std::deque >({ 1,3,5 }, { 6,4,2 });

			//two_calls<std::list>(1, 3, 5)(6, 4, 2);

			auto merged_pack = two_ppacks(1, false, 'X')(6.32f, "Hola!", 3.14156);
			prinf(" Merged  ppack: " , merged_pack);

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