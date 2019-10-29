#pragma once

#include "../common.h"

#include <typeinfo>
#include <cstdio>

namespace dbj::nanoplay {

	TU_REGISTER([]
		{
		pprintf(DBJ_FG_GREEN
			"\n\nSampling pprintpp msvc port\n{s}\n" 
			DBJ_RESET, 
			DBJ_FILE_LINE_TSTAMP);
 			// DBJ  -- added 
#ifdef _MSC_VER
	// NOTE: make sure you set the font that can display your glyphs
	// (WIN10 console + ) Lucida Console seems ok for cyrillic glyphs
			pprintf("\n\n{s}\n\n", u8"Dusan Jovanovic in cyrillic: Душан Јовановић"); // utf8 encoded char
		// prints pointer -- pprintf("{}\n\n", u8"Душан Јовановић" ); // utf8 encoded char
#endif // _MSC_VER

			pprintf("This is \\{ how you mask } {s}.\n", "curly braces");

			pprintf("Of course it's possible to add more formatting flags:\n"
				"{x}, {10x}, {#10x}, and {#010x} use the normal printf capabilities.\n",
				0x123, 0x123, 0x123, 0x123);

			pprintf("{20}\n", 1.23456);

#ifdef __clang__
			// this is apparently too complex for: 
			// Microsoft (R) C/C++ Optimizing Compiler Version 19.23.28106.4 for x86
			// pprintpp.hpp(156, 1) : fatal error C1202 : recursive type or function dependency context too complex
			// 
			pprintf("{35s} | Format str -> Result\n"
				"{35s} | ---------------------\n"
				"{35s} | \\{s}     -> \"{s}\"\n"
				"{35s} | \\{10s}   -> \"{10s}\"\n"
				"{35s} | \\{}      -> \"{}\"\n"
				"{35s} | \\{x}     -> \"{x}\"\n"
				"{35s} | \\{10}    -> \"{10}\"\n"
				"{35s} | \\{10x}   -> \"{10x}\"\n"
				"{35s} | \\{#10x}  -> \"{#10x}\"\n"
				"{35s} | \\{#010x} -> \"{#010x}\"\n"
				"{35s} | \\{}      -> \"{}\"\n"
				"{35s} | \\{10}    -> \"{10}\"\n"
				"{35s} | \\{5.2}   -> \"{5.2}\"\n",
				"Meaning", "---------------------",
				"String \"abc\"", "abc",
				"String \"abc\" + min width", "abc",
				"value 0x123, default", 0x123,
				"value 0x123, hex", 0x123,
				"minimum width", 0x123,
				"hex + min width", 0x123,
				"hex + min width + hex prefix", 0x123,
				"hex + min w. + hex prefix + 0-pad", 0x123,
				"FP", 12.345,
				"FP + min width", 12.34567890123456789,
				"FP + width + max precision", 12.34567890123456789
			);
#endif // __clang__
		});

/*
Example:
 auto compile_time_format_ =  AUTOFORMAT("{} {}", 123, 1.23f)
 must return: "%d %f"
 */
#define PP_TEST_(printf_format_correct_str, ...) \
    assert(!strcmp(AUTOFORMAT(__VA_ARGS__), printf_format_correct_str))

 TU_REGISTER([] {

	 pprintf(DBJ_FG_GREEN "\n\nSampling pprintpp msvc port AUTOFORMAT macro\n\n{s }\n" DBJ_RESET, DBJ_FILE_LINE_TSTAMP);

	 PP_TEST_("", "");

	 PP_TEST_("%%", "%%");
	 PP_TEST_("%d %f", "{} %f", 123, 1.23f);
	 PP_TEST_("%f %d", "%f {}", 1.23f, 123);

	 PP_TEST_(" { ", " \\{ ");
	 PP_TEST_("{}", "\\{}");
	 PP_TEST_(" { %d } ", " \\{ {} } ", 123);

	 PP_TEST_("%p", "{}", nullptr);
	 PP_TEST_("%p", "{}", reinterpret_cast<void*>(0));

	 // For safety reasons:
	 // Only print strings as strings, if the user also writes {s}
	 PP_TEST_("%p", "{}", "str");
	 PP_TEST_("%s", "{s}", "str");

	 PP_TEST_("%c", "{}", static_cast<char>(123));

	 PP_TEST_("%d", "{}", static_cast<short>(123));
	 PP_TEST_("%d", "{}", 123);
	 PP_TEST_("%ld", "{}", 123l);
	 PP_TEST_("%lld", "{}", 123ll);

	 PP_TEST_("%u", "{}", 123u);
	 PP_TEST_("%lu", "{}", 123ul);
	 PP_TEST_("%llu", "{}", 123ull);

	 PP_TEST_("%x", "{x}", 123u);
	 PP_TEST_("%lx", "{x}", 123ul);
	 PP_TEST_("%llx", "{x}", 123ull);

	 PP_TEST_("%d", "{}", true);

	 PP_TEST_("%f", "{}", 1.0f);
	 PP_TEST_("%lf", "{}", 1.0);

	 PP_TEST_("%10d", "{10}", 123);
	 PP_TEST_("%10x", "{10x}", 123u);
	 PP_TEST_("%#10x", "{#10x}", 123u);

	 // Give the user hex if asked for explicitly.
	 PP_TEST_("%x", "{x}", 123);
	 PP_TEST_("%lx", "{x}", 123l);
	 PP_TEST_("%llx", "{x}", 123ll);

	 pprintf( DBJ_FG_GREEN_BOLD  "\n\nGreen, green, green!\n\n " DBJ_RESET " All tests passed.\n");

	 //pprintf("{s} {} {1} {} {} {} {} {} {} {} {} {} {} {} {} {#x}\n",
		// "1", 2u, 3.0, 4.0f, 5ull, '6', 7ul, 8, 9, 10, 11, 12, 13, 14, 15, 16u);

	 });

#undef PP_TEST_

} // namespace dbj::nanoplay 