#pragma once
#include "../common.h"

namespace dbj::nanolib {

	TU_REGISTER(
		[] {
			fmt::memory_buffer mb_;
			fmt::format_to(mb_, "The answer is {}.", 42);

		});
} // namespace dbj::nanolib 

