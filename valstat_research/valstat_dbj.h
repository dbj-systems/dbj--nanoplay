#pragma once
/*
valstat 2 -- no pairs
*/

#include "valstat"
#include "../common.h"

namespace dbj
{
	using namespace std;

	/*
	dbj imagined company
	this type alias is all they have and need to use std::valstat
	this makes them fully std::valstat compliant
	this also makes other people better understand their API's

	NOTE: std::string is not the most performant json
		  string implementation. dbj, they usually use vector<char> or even
		  unique_ptr<char[]>
		  that issue is largely mitigated by not using strings
		  but handles to the strings registry as the status type
	*/
	template< typename T>
	using valstat = future_std::valstat<T, std::string >;

	// descriptive output
	// the  verbose valstat consuming
	// for testing purposes only
	// dbj usualy do njot use <iostream> in a production code
	template<typename T>
	inline std::ostream& operator << (std::ostream& os, const dbj::valstat<T> vt)
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

} // dbj

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

namespace valstat_testing_space {
	using namespace std;
	inline auto driver = [](auto function_, char const* prompt_ = nullptr, bool testing_c_interop = false)
	{
		if (prompt_) cout << endl << endl << prompt_ << endl;

		cout << boolalpha << endl << DBJ_FG_CYAN << "valstat:" << DBJ_RESET;

		// structured binding of a result from a C function
		auto [value, status] = function_();

		cout << "\nvalue: ";
		if (value)
			cout << DBJ_FG_CYAN_BOLD << *value << DBJ_RESET;
		else
			cout << DBJ_FG_CYAN_BOLD << "{ empty }" << DBJ_RESET;

		cout << " / status:";
		if (status)
			if ( false == testing_c_interop)
				cout << DBJ_FG_RED_BOLD << *status << DBJ_RESET;
			else
				cout << DBJ_FG_RED_BOLD << status << DBJ_RESET;
		else
			cout << DBJ_FG_CYAN_BOLD << "{ empty }" << DBJ_RESET;

		cout << endl;
	};
} // interop_testing_space