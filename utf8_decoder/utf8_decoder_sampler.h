#pragma once

#include "utf8_decoder.h"
#include "../common.h"

namespace utf_decoder_sampler {
#define SPECIMEN u8"ひらがな"

	TU_REGISTER(
		[] {
			// functions returning void,that deal with errors
			// example
			// before valstat the return was 'void' and perror() was used internaly
			auto [val, stat] = dbj::utf8_print_code_points(stdout, (uint8_t*)SPECIMEN);

			// we do not need special value for val
			// just is it here or not here
			if (!val)
				printf("Error!");

			if (stat)
				printf(" %s\n", stat);
			// interop status is char *
		});

	TU_REGISTER(
		[] {
			printf("\n\nIterating over u8 string literal: %s\n\n", (char const *)SPECIMEN);
			unsigned  cnt_ = 1;

			// msvc c++20 , type of elem is int
			// same is for clang and g++
			// iteration is byte by byte, total of 14 times
			for ( auto const & elem : SPECIMEN )
			{
				cnt_ += 1;
				printf("%s", (const char *)elem);
			}

			printf("\n\nIterated %d times", cnt_);
			bool dumsy = true;
			if (dumsy) return;
		}
	);
#undef SPECIMEN
} // utf_decoder_sampler
