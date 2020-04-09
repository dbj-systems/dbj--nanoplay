#pragma once

#include "utf8_decoder.h"
#include "../common.h"

namespace utf_decoder_sampler {

// NOTE: be sure to use /utf-8 cl.exe switch
#define SPECIMEN u8"ひらがな"

	TU_REGISTER(
		[] {
			printf("\n\n UTF8 decoding: %s == ", (char const *)SPECIMEN);
			auto [val, stat] = dbj::utf8_print_code_points(stdout, (uint8_t*)SPECIMEN);

			// we do not need special value for val
			// just is it here or not here
			if (!val)
				printf("Error!");

			// interop status type is char *
			if (stat) printf("\n\n Status: %s", stat);

			{
				auto [val, stat] = dbj::countCodePoints((uint8_t*)SPECIMEN);

				if (!val)
					printf("Error!");
				else
				   printf("\n\n\"%s\", has %d UTF-8 code points aka glyphs", (char *)SPECIMEN, *val);

				if (stat) printf("\n\nStatus: %s\n", stat);
			}

		});

	TU_REGISTER(
		[] {
			printf("\n\nIterating over u8 string literal: %s\n\n", (char const *)SPECIMEN);
			unsigned  cnt_ = 1;

			// msvc c++20 , type of elem is int
			// same is for clang and g++
			// iteration is byte by byte, total of 14 times
			// note: cl.exe /utf-8  is in use
			for ( auto const & elem : SPECIMEN )
			{
				cnt_ += 1;
				printf("%c", elem);
			}

			printf("\n\nIterated %d times", cnt_);
			bool dumsy = true;
			if (dumsy) return;
		}
	);
#undef SPECIMEN
} // utf_decoder_sampler
