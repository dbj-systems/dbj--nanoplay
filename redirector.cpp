#include <io.h>
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>

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
			std_redirector(std_redirector const&) = delete;
			std_redirector(std_redirector&&) = delete;

			std_redirector(char const* filename_, bool revert_on_destruct_) noexcept
				: revert(revert_on_destruct_)
			{
				FILE* std_stream_ = DBJ_REDIRECTOR_RESOLVER(std_stream_ordinal_);
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
			::fprintf(stderr, "\n*******************************************************************************");
			::fprintf(stderr, "\n*****                                                                     *****");
			::fprintf(stderr, "\n*****  LOG BEGIN                                                          *****");
			::fprintf(stderr, "\n*****                                                                     *****");
			::fprintf(stderr, "\n*******************************************************************************");
			::fprintf(stderr, "\n*******************************************************************************\n\n");
#endif // DBJ_REDIRECTOR_HEADER

			if (out_log_file_name_)
				out_redir_ = new std_out_redirector{ out_log_file_name_, revert_on_destruct_ };
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