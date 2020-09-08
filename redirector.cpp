#include <io.h>
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>

/// -----------------------------------------------------
/// we will not assume stdout is int 1 and stderr is int 2
/// redirect stderr, stdout to file

/// also we do not assume std streams are any type known to us
/// thus we can not use them as compile time template args 
/// thus macro cludge helps this time
#define DBJ_STREAM_(which_) (which_ == 2 ? stderr : stdout )

namespace dbj {

	struct redirector final {

		template< unsigned std_stream_ordinal_ >
		struct std_redirector final
		{
			int fd{};
			fpos_t pos{};
			FILE* stream{};
			bool revert{ false };

			std_redirector() = default;
			std_redirector(std_redirector const&) = delete;
			std_redirector(std_redirector&&) = delete;

			std_redirector(char const* filename_, bool revert_on_destruct_) noexcept
				: revert(revert_on_destruct_)
			{
				FILE* std_stream_ = DBJ_STREAM_(std_stream_ordinal_);
				fflush(std_stream_);
				fgetpos(std_stream_, &(this->pos));
				this->fd = _dup(_fileno(std_stream_));
				errno_t err = freopen_s(&stream, filename_, "w", std_stream_);

				if (err != 0) {
					perror("error on freopen");
					exit(EXIT_FAILURE); // a bit drastic?
				}
			}

			~std_redirector()
			{
				/// if this happens too soon and
				/// if you do this stderr might not outpout to file but to 
				/// non existent console
				fflush(stream);
				if (this->revert) {
					FILE* std_stream_ = DBJ_STREAM_(std_stream_ordinal_);
					/*int dup2_rezult_ =*/ _dup2(fd, _fileno(std_stream_));
					_close(fd);
					clearerr(std_stream_);
					fsetpos(std_stream_, &pos);
				}
			}
		}; // std_redirector

		using std_err_redirector = std_redirector</*stderr*/ 2>;
		using std_out_redirector = std_redirector</*stdout*/ 1>;

		std_err_redirector* err_redir_{};
		std_out_redirector* out_redir_{};

		/// BIG ASSUMPTION
		/// we assume stderr will be redirected to a particular file
		/// AND we assume stdout will NOT be redirected to the same file
		/// if second file name is not given stdout output will still apear on the terminal
		/// disciplined server side components will simply not do that if unwanted
		redirector(
			bool revert_on_destruct_,
			char const* err_log_file_name_,
			char const* out_log_file_name_ = nullptr)
		{
			_ASSERTE(err_log_file_name_);
			err_redir_ = new std_err_redirector{ err_log_file_name_ , revert_on_destruct_ };

#ifdef DBJ_REDIRECTOR_HEADER
			::fprintf(stderr, "\n\n*******************************************************************************");
			::fprintf(stderr, "\n*****");
			::fprintf(stderr, "\n*****  STDERR LOG BEGIN [" __TIMESTAMP__ "]");
			::fprintf(stderr, "\n*****\n\n");
#endif // DBJ_REDIRECTOR_HEADER

			if (out_log_file_name_) {

				if (!strcmp(err_log_file_name_, out_log_file_name_))
				{
					errno = EINVAL;
					perror(__FILE__ "\n\nCan not redirect stdout and stderr to the same file!\n\n");
					exit(EXIT_FAILURE);
				}
				out_redir_ = new std_out_redirector{ out_log_file_name_, revert_on_destruct_ };
#ifdef DBJ_REDIRECTOR_HEADER
				::fprintf(stdout, "\n\n*******************************************************************************");
				::fprintf(stdout, "\n*****");
				::fprintf(stdout, "\n*****  STDOUT LOG BEGIN [" __TIMESTAMP__ "]");
				::fprintf(stdout, "\n*****\n\n");
#endif // DBJ_REDIRECTOR_HEADER
			}
		}

		~redirector() {
			if (err_redir_) {
				delete err_redir_; err_redir_ = nullptr;
			}
			if (out_redir_) {
				delete out_redir_; out_redir_ = nullptr;
			}
		}

		/// no copy no move
		redirector(redirector const&) = delete;
		redirector(redirector&&) = delete;

	}; // redirector 
} // dbj

static auto sneaky_output_redirect_on_startup = []()
{
	static char stderr_log_fname_[1024]{};
	static char stdout_log_fname_[1024]{};

	int rez = sprintf_s(stderr_log_fname_, 1024, "%s.stderr.log" __argv[0]);
	_ASSERTE(rez > 0);

	rez = sprintf_s(stdout_log_fname_, 1024, "%s.stdout.log" __argv[0]);
	_ASSERTE(rez > 0);

	// https://stackoverflow.com/a/46869216/10870835
	static dbj::redirector redirect_{
		/*revert_on_destruct_*/ false ,
		stderr_log_fname_ ,
		stdout_log_fname_ };

	return true;

}();

#undef DBJ_STREAM_