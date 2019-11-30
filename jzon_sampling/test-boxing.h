#pragma once

#include "../common.h"

#define _USE_MATH_DEFINES // for MSVC
#include <math.h>

namespace dbj_jzon_testing 
{
	using namespace jzon;

#define TEST_NOTNAN(x) TU_CHECK(value(x).is_number()); TU_CHECK(value(-x).is_number());

	TU_REGISTER([] {
		TEST_NOTNAN(0.0);
		TEST_NOTNAN(1.0);
		TEST_NOTNAN(1.0 / 3.0);
		TEST_NOTNAN(5.45);
		TEST_NOTNAN(7.62);
		TEST_NOTNAN(1e40);
		TEST_NOTNAN(M_E);
		TEST_NOTNAN(M_LOG2E);
		TEST_NOTNAN(M_LOG10E);
		TEST_NOTNAN(M_LN2);
		TEST_NOTNAN(M_LN10);
		TEST_NOTNAN(M_PI);
		TEST_NOTNAN(M_PI_2);
		TEST_NOTNAN(M_PI_4);
		TEST_NOTNAN(M_1_PI);
		TEST_NOTNAN(M_2_PI);
		TEST_NOTNAN(M_2_SQRTPI);
		TEST_NOTNAN(M_SQRT2);
		TEST_NOTNAN(M_SQRT1_2);
		TEST_NOTNAN(DBL_MIN);
		TEST_NOTNAN(DBL_EPSILON);
		TEST_NOTNAN(DBL_MAX);
		TEST_NOTNAN(INFINITY);
		TEST_NOTNAN(NAN);
		});
#undef TEST_NOTNAN

#define TEST_TYPE(x) TU_CHECK( false == value(x).is_number()); TU_CHECK(value(x).type() == x)

	TU_REGISTER([] {
		TEST_TYPE(type::null);
		TEST_TYPE(type::boolean);
		TEST_TYPE(type::string);
		TEST_TYPE(type::array);
		TEST_TYPE(type::object);
		});
#undef TEST_TYPE

#define TEST_ERROR(x) TU_CHECK( false == value(x).is_number()); TU_CHECK(static_cast<error>(value(x).type()) == x)

	TU_REGISTER([] {
		TEST_ERROR(error::expecting_string);
		TEST_ERROR(error::expecting_value);
		TEST_ERROR(error::invalid_literal_name);
		TEST_ERROR(error::invalid_number);
		TEST_ERROR(error::invalid_string_char);
		TEST_ERROR(error::invalid_string_escape);
		TEST_ERROR(error::invalid_surrogate_pair);
		TEST_ERROR(error::missing_colon);
		TEST_ERROR(error::missing_comma_or_bracket);
		TEST_ERROR(error::unexpected_character);
		});

#undef TEST_ERROR

} // namespace dbj_jzon_testing 

