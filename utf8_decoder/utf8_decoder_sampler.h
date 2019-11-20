#pragma once

#include "utf8_decoder.h"
#include "../common.h"

namespace utf_decoder_sampler {

	TU_REGISTER(
		[] {
			
			printf("\n\nIterating over u8 string literal: u8\"ひらがな\"\n\n");
			unsigned  cnt_ = 1;

			// msvc c++20 , type of elem is int
			// same is for clang and g++
			// iteration is byte by byte, total of 14 times
			for (auto& elem : u8"ひらがな")
			{
				cnt_ += 1;
				printf("%c", elem);
				// this line makes the above line produce nothing
				// printf("\n %02d %04X ", cnt_, elem);
			}

			printf("\n\nIterated %d times", cnt_ );
			bool dumsy = true;
			if (dumsy) return;
		}
	);

} // utf_decoder_sampler
