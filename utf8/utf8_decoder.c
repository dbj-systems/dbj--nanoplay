//http://bjoern.hoehrmann.de/utf-8/decoder/dfa/
#include "utf8_decoder.h"


// Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

static const uint8_t utf8d[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
  8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
  0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
  0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
  0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
  1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
  1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
  1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
};

uint32_t 
utf8_decode(uint32_t* state, uint32_t* codep, uint32_t byte) 
{
	uint32_t type = utf8d[byte];

	*codep = (*state != UTF8_ACCEPT) ?
		(byte & 0x3fu) | (*codep << 6) :
		(0xff >> type)& (byte);

	*state = utf8d[256 + *state * 16 + type];
	return *state;
}

/// <summary>
/// This function prints out all code points in the string and an error message
/// if unexpected bytes are encountered, or if the string ends with an
/// incomplete sequence.
/// </summary>
/// <param name="file_ptr_">where to print</param>
/// <param name="s">what to print</param>
struct interop_void utf8_print_code_points(FILE * file_ptr_ , uint8_t * s) 
{
	uint32_t codepoint;
	uint32_t state = 0;

	for (; *s; ++s)
		if (!utf8_decode(&state, &codepoint, *s))
		{
			if (state != UTF8_ACCEPT) {
				return (interop_void){ 0, INTEROP_JSON("The string is not well-formed") };
			}
			fprintf(file_ptr_, " U+%04X", codepoint);
		}
	static int T = true;
	return (interop_void) { &T, INTEROP_JSON("OK") };
}

///<summary>
/// Validating and counting characters
/// This function checks if a null - terminated string is a well - 
/// formed UTF - 8 sequence and counts how many code points are in the string.
/// valstat val is pointer to local static so copy or use it quickly 
/// yes it is a cludge in here
///</summary>
struct interop_int countCodePoints(uint8_t * s) 
{
	uint32_t codepoint;
	uint32_t state = 0;
	static int count;

	count = 0;

	for (count = 0; *s; ++s)
		if (!utf8_decode(&state, &codepoint, *s))
			count += 1;

	if (state == UTF8_ACCEPT)
		return (interop_int) { &count, INTEROP_JSON("OK") };

	/*
	paradigm shift: no need to return any special value that means something
	like false, it is enough to return empty val valstat
	*/
	return (interop_int) { 0, INTEROP_JSON("The string argument was malformed!") };
}
