#pragma once

#include "valstat"
#include "../common.h"

namespace dbj {

	inline namespace valstat_2 {

		using namespace std;
		using namespace dbj::nanolib;

		/*
		it turns out status as a string sub-concept allows for total
		decoupling from the valstat value half.
		thus dbj do not need to pre-declare valstat types for different status types too

		Example:

		valstat<bool> fun ( bool arg_ ) {

		if ( is_win32_error() )
			return {{}, { make_win32_status( GetLastError(), __FILE__, __LINE__ ) }};

		if ( is_posix_error() )
			return {{}, { make_posix_status( std::errc::ENOMEM, __FILE__, __LINE__ ) }} ;

			return {{ arg_ }, {}};
			}

		bellow is just a message+file+line status, for sampling the valstat
		it is in a JSON format as every other status message
		*/
		inline std::string
			make_status(const char* file, long line, const char* time_stamp, const char* msg = nullptr)
		{
			auto nix_path = v_buffer::replace(v_buffer::format("%s", file), '\\', '/');
			v_buffer::buffer_type buffy = v_buffer::format(
				R"({ "message" : "%s", "file" : "%s", "line" : %d, "timestamp" : "%s" })",
				(msg ? msg : "unknown"), nix_path.data(), line, time_stamp);

			return  { buffy.data() };
		}

		namespace posix {
			inline std::string errc_to_message(std::errc posix_err_code)
			{
				::std::error_code ec = std::make_error_code(posix_err_code);
				v_buffer::buffer_type buffy = v_buffer::format("%s", ec.message().c_str());
				return { buffy.data() };
			};
		} // posix

		namespace win32 {
			/* win32 error code as a (strong) type */
			struct error_code;
			struct error_code
			{
				int v{ 0 };
				error_code() : v(::GetLastError()) { ::SetLastError(0); }
			};

			constexpr inline bool is_ok(error_code const& ec_) { return ec_.v == 0; }

			/* Last WIN32 error, message */
			inline std::string error_message(int code = 0)
			{
				std::error_code ec(
					(code ? code : ::GetLastError()),
					std::system_category()
				);
				::SetLastError(0); //yes this helps
				v_buffer::buffer_type buffy = v_buffer::format("%s", ec.message().c_str());
				return { buffy.data() };
			}

			inline auto code_to_message(win32::error_code code) -> std::string
			{
				if (code.v)
					return error_message(code.v);
				return { "No error" };
			};
		} // win32

	} // inline namespace valstat_2 
} // dbj