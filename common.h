#ifndef _DBJ_INC_COMMON_
#define _DBJ_INC_COMMON_

// #pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wpragma-once-outside-header"

// MSVC STD LIB aka STL
// _ITERATOR_DEBUG_LEVEL 2 is the DEBUG build default
// and that will produce segv with dbj stack alocator 
// that was not tested with stack allocator I suppose?
// https://docs.microsoft.com/en-us/cpp/standard-library/iterator-debug-level?view=vs-2019

#if (_ITERATOR_DEBUG_LEVEL != 0)
#undef _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL  0
#endif

/*
Why was this used?
#if (WINVER < NTDDI_WIN10_RS3)
#else
#error dbj++ requires Windows builds above REDSTONE 3 or above
#endif
*/


#include <cassert>
#include <future>
#include <iomanip>
#include <random>
#include <thread>

/// NOTE: among other things this ensures synchronized output
///       unless one uses naked <stdio.h> or god forbid <iostream>
#define DBJ_NANO_LIB_MT
#define NANO_PRINTF_IN_USE

#if 0
// iuse it as local, for when developing
#include "../dbj--nanolib/dbj++valstat.h"
#include "../dbj--nanolib/dbj++tu.h"
#else
// use it as a submodule
#include "dbj--nanolib/dbj++valstat.h"
#include "dbj--nanolib/dbj++tu.h"
#endif

/*
rudimentary runtime version checks
https://docs.microsoft.com/en-us/windows/desktop/sysinfo/getting-the-system-version
*/
#include <VersionHelpers.h>

namespace testing_space {

/// #undef driver 

	template< typename FP>
	inline void driver(
		FP function_,
		char const* prompt_ = nullptr,
		bool testing_c_interop = false)
	{
		using namespace std;
		namespace dbjlog = dbj::nanolib::logging ;

		if (prompt_) dbjlog::log(prompt_);

		dbjlog::log(DBJ_FG_CYAN, "valstat:", DBJ_RESET);

		// structured binding of a result from a C function
		auto [value, status] = function_();

		dbjlog::log("value: ");
		if (value)
			dbjlog::log(DBJ_FG_CYAN_BOLD, *value, DBJ_RESET);
		else
			dbjlog::log(DBJ_FG_CYAN_BOLD, "{ empty }", DBJ_RESET);

		dbjlog::log(" / status:") ;
		if (status)
			if (false == testing_c_interop)
				dbjlog::log(DBJ_FG_RED_BOLD, *status, DBJ_RESET);
			else
				dbjlog::log(DBJ_FG_RED_BOLD, status, DBJ_RESET);
		else
			dbjlog::log(DBJ_FG_CYAN_BOLD, "{ empty }", DBJ_RESET);

	}
} // interop_testing_space

#include <io.h>
#include <stdio.h>

namespace dbj {
/// -----------------------------------------------------
/// we will not assume stdout is int 1 and stderr is int 2
/// redirect stderr, stdout to file
/// warning: no error checking whatsoever

	struct redirector final {

		/// we do not assume std streams are any type known to us
		/// thus we can not use them as compile time template args 
		/// thus macro cludge helps this time
#define DBJ_REDIRECTOR_RESOLVER(which_) (which_ == 2 ? stderr : stdout )

		template< unsigned std_stream_ordinal_ >
		struct std_redirector final 
		{
			int fd{};
			fpos_t pos{};
			FILE* stream{};
			bool revert{ false };

			std_redirector() = default;
			std_redirector(std_redirector const &) = delete ;
			std_redirector(std_redirector &&) = delete ;

			std_redirector(char const* filename_, bool revert_on_destruct_ ) noexcept
				: revert(revert_on_destruct_)
			{
				FILE * std_stream_ = DBJ_REDIRECTOR_RESOLVER(std_stream_ordinal_);
				fflush(std_stream_);
				fgetpos(std_stream_, &(this->pos) );
				this->fd = _dup(_fileno(std_stream_));
				errno_t err = freopen_s(&stream, filename_, "w", std_stream_);

				if (err != 0) {
					perror("error on freopen");
					exit(EXIT_FAILURE); // a bit drastic?
				}
			}

			~std_redirector()
			{
				/// if you do this stderr might not outpout to file but to 
				/// non existent console
				/// if this happens too soon
				fflush(stream);
				if (this->revert) {
					FILE* std_stream_ = DBJ_REDIRECTOR_RESOLVER(std_stream_ordinal_);
					/*int dup2_rezult_ =*/ _dup2(fd, _fileno(std_stream_));
					_close(fd);
					clearerr(std_stream_);
					fsetpos(std_stream_, &pos);
				}
			}
		}; // std_redirector

#undef DBJ_REDIRECTOR_RESOLVER

		using std_err_redirector = std_redirector</*stderr*/ 2>;
		using std_out_redirector = std_redirector</*stdout*/ 1>;

		std_err_redirector * err_redir_{};
		std_out_redirector * out_redir_{};

		/// BIG ASSUMPTION
		/// we assume stderr will be redirected to a particular file
		/// AND we assume stdout will NOT be redirected to the same file
		/// if second file name is not given stdout output will still apear on the terminal
		/// disciplined server side components will simply not do that if unwanted
		redirector(
			bool revert_on_destruct_,
			char const * err_log_file_name_ , 
			char const* out_log_file_name_ = nullptr )
		{
			DBJ_ASSERT( err_log_file_name_ );
			err_redir_ = new std_err_redirector{ err_log_file_name_ , revert_on_destruct_ };

#ifdef DBJ_REDIRECTOR_HEADER
			::fprintf(stderr, "\n\n*******************************************************************************");
			::fprintf(stderr, "\n*******************************************************************************");
			::fprintf(stderr, "\n*****                                                                     *****");
			::fprintf(stderr, "\n*****  LOG BEGIN                                                          *****");
			::fprintf(stderr, "\n*****                                                                     *****");
			::fprintf(stderr, "\n*******************************************************************************");
			::fprintf(stderr, "\n*******************************************************************************\n\n");
#endif // DBJ_REDIRECTOR_HEADER

			if (out_log_file_name_)
				out_redir_ = new std_out_redirector { out_log_file_name_, revert_on_destruct_ };
		}

		~redirector() {
			if (err_redir_) {
				delete err_redir_; err_redir_ = nullptr;}
			if (out_redir_) {
				delete out_redir_; out_redir_ = nullptr;}
		}

		/// no copy no move
		redirector(redirector const &) = delete;
		redirector(redirector &&) = delete;

	}; // redirector 
	} // dbj

#endif // _DBJ_INC_COMMON_
