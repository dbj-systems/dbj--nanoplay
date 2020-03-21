#pragma once

#include "../common.h"

namespace dbj::playground {


	inline namespace narf 
	{
		// the key abstraction
		template <typename T, std::size_t N>
		using narwal = std::reference_wrapper<T[N]>;

		template<typename T, size_t N, typename array_type = T(&)[N] >
		inline	narwal<T,N> every(narwal<T,N> arf, T val)
		{
			for (auto& elem : array_type(arf) ) { elem = val; }
			return arf;
		}

		/// <summary>
		/// callback for every element of array reference
		/// 
		/// bool callback( T & value, size_t index ) ;
		/// false return breaks the iteration
		/// </summary>
		template<typename T, size_t N, typename F, typename array_type = T(&)[N] >
		inline narwal<T, N> every(narwal<T, N> arf, F cb_)
		{
			size_t idx_{};
			for (auto elem : array_type(arf) ) { if (!cb_(elem, idx_++)) break; }
			return arf;
		}

		TU_REGISTER([]
			{
				using namespace std;
				constexpr auto N = 0xF;
				using bufy_t = narwal<char, N>;

				char charr[N]{};
				bufy_t bottom(charr);
				// populate
				bufy_t mufy = every( bottom, '*' );
				// print
					cout << endl << "mufy: {";
				every(mufy, [](auto val, size_t idx) {
					cout << setw(3) << val;	return true; // to proceed
				});
					cout << " }\n";
			});

		template<typename T, size_t N, typename array_type = T[N] >
		inline	array_type& every(T(&arf)[N], T val)
		{
			for (auto& elem : arf) { elem = val; }
			return arf;
		}

		template<typename T, size_t N, typename array_type = T[N], typename F >
		inline array_type& every(T(&arf)[N], F cb_)
		{
			size_t idx_{};
			for (auto& elem : arf) { if (!cb_(elem, idx_++)) break; }
			return arf;
		}

		TU_REGISTER([]
			{
				using charray_type = char[0xF];
				charray_type charray;
				// decay to point to T*
				auto pointer_to_char = every(charray, '*');
				// preserving the ref to array
				charray_type& charray_ref = every(charray, '*');
				// normal native arr usage
				charray_ref[0] = '*';
				assert(charray[0] == charray_ref[0]);

				every(charray_ref, [](char& val_, size_t idx_) -> bool { val_ = char(65 + idx_); return true; });
			});
	} // narf
} // namespace dbj::playground 