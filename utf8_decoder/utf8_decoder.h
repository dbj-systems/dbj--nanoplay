#pragma once

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif //  __cplusplus

uint32_t inline
utf8_decode(uint32_t* state, uint32_t* codep, uint32_t byte);

void utf8_print_code_points(FILE *, uint8_t* s);

#ifdef __cplusplus
}
#endif //  __cplusplus
