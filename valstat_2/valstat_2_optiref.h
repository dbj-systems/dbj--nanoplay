#ifndef _INC_VAL_STAT_OPTIREF_
#define _INC_VAL_STAT_OPTIREF_

#include "valstat_2.h"

namespace dbj::nanoplay {

	using namespace std;

	/*
	handling no copy/no move types
     */
	namespace {
		struct adamant final{
			inline static const char* genus = "tenacious";
			adamant(const adamant&) = delete;
			adamant & operator = (const adamant&) = delete;
			adamant(adamant&&) = delete;
			adamant& operator = (adamant&&) = delete;
			// convention:
			// type::vt
			// is the encapsulated valstat for the type
			using vt = valstat< reference_wrapper<adamant> >;

			friend ostream& operator << (ostream& os, const adamant& vt)
			{
				return os << "\nadamant::genus = " << vt.genus;
			}
		};

	}

	TU_REGISTER([] {
			adamant steadfast{};

			auto info = [&]( ) -> adamant::vt { return  { {steadfast}, {"status message"} }; };

			auto error = [&]( ) -> adamant::vt { return  { {}, {"status message"} }; };

			auto ok = [&]( ) -> adamant::vt { return  { {steadfast}, {} }; };

			auto empty = [&]( ) -> adamant::vt { return  { {}, {} }; };

			auto consumer = []( auto producer ) {
				// auto [val, stat] = producer();
				// sampling through the verbose stream output op.
				cout << producer();
			};

			consumer(info);
			consumer(error);
			consumer(ok);
			consumer(empty);

		});

} // dbj::nanoplay

#endif // _INC_VAL_STAT_OPTIREF_



