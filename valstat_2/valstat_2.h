#pragma once
/*
valstat 2 -- no pairs
*/

#include "../common.h"
#include <charconv>

namespace dbj::nanoplay {

	inline namespace valstat_2 {

		using namespace std;
		using namespace dbj::nanolib;

		string make_status(const char* , long , const char *, const char* = nullptr);

		// no pair version
		// status == string
		template< typename T>
		struct [[nodiscard]] valstat{
			using value_type = T;
			optional<T> value;
			optional<string> status;
		};

		// descriptive output
		// the  verbose valstat consuming
		template<typename T>
		inline ostream& operator << (ostream& os, const valstat<T> vt)
		{
			os << "\nvalstat state: ";
			if (!vt.value && vt.status) os << "ERROR";
			if (vt.value && !vt.status) os << "OK";
			if (vt.value && vt.status) os << "INFO";
			if (!vt.value && !vt.status) os << "EMPTY";

			os << " , content: \n{"; 
			if (vt.value)
				os << "\n { value: " << *vt.value << " }";
			else
				os << "\n { value: empty }";
			os << " ,";
			if (vt.status)
				os << "\n { status: " << *vt.status << " }";
			else
				os << "\n { status: empty }";
			return os << "\n}\n";
		}

	/*
	it turns out status as a string sub-concept allows for total
	decoupling from the valstat value half.
	which in turn allows for pick-n-mix of statuses
	which in turn means status content can be decided at the
	very moment of prepraring a return, not before

	Example:

	valstat<bool> fun ( bool arg_ ) {

	if ( is_win32_error() )
	  return {{}, { make_win32_status( GetLastError(), __FILE__, __LINE__ ) }};

	if ( is_posix_error() )
	  return {{}, { make_posix_status( std::errc::ENOMEM, __FILE__, __LINE__ ) }} ;

	  return {{ arg_ }, {}};

	 }

	bellow is just a message+file+line status, for sampling the valstat
	  it is in a JSON format as evey other string as a status
	  has no reason not to be
	*/
		inline string make_status(const char* file, long line, const char * time_stamp, const char* msg)
		{
			auto nix_path = v_buffer::replace(v_buffer::format("%s", file), '\\', '/');
			v_buffer::buffer_type buffy = v_buffer::format(
				R"({ "message" : "%s", "file" : "%s", "line" : %d, "timestamp" : "%s" })",
				(msg ? msg : "unknown"), nix_path.data(), line, time_stamp);

			return { buffy.data() };
		}

		namespace posix {
			inline std::string errc_to_message(std::errc posix_err_code) 
			{
				::std::error_code ec = std::make_error_code(posix_err_code);
				return fmt::format("{}", ec.message().c_str());
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
				return fmt::format("{}", ec.message().c_str());
			}

			inline auto code_to_message(win32::error_code code) -> std::string
			{
				if (code.v)
					return error_message(code.v);
				return fmt::format("{}", "No error");
			};
		} // win32

	} // namespace valstat_2 
} // dbj::nanoplay
