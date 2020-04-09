#pragma once

#include <stdio.h>
#include <stdint.h>

#include "../valstat_research/valstat_dbj_c_interop.h"

#ifdef __cplusplus
namespace dbj {
	extern "C" {
#endif //  __cplusplus

		uint32_t
			utf8_decode(uint32_t* state, uint32_t* codep, uint32_t byte);

		struct interop_int countCodePoints(uint8_t* s);

		struct interop_void utf8_print_code_points(FILE*, uint8_t* s);

#ifdef __cplusplus
	}
} // namespace dbj
#endif //  __cplusplus
