
/*
github.com/OpenLiveWriter/OpenLiveWriter/src/unmanaged/OpenLiveWriter.CppUtils/Include/log_file.h
*/

#pragma once

#include "../../common.h"
#include <ShlObj.h>
#include "hresultexception.h"

// unavoidabler 
// this fili is really sily
#define FILI __FILE__ , __LINE__

namespace dbj::nanoplay {
	namespace win {

		using namespace dbj::nanolib;
		// P1028R2 -- page 4
		using native_handle_vt = win32_valstat_trait< HANDLE>;

		/*
		get to the HANDLE of file opened
		*/
		inline native_handle_vt::return_type
			open_file(const char* path) noexcept
		{
			HANDLE h = ::CreateFileA(path,
				GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE |
				FILE_SHARE_DELETE,
				nullptr,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				nullptr
			);
			if (INVALID_HANDLE_VALUE == h)
			{
				// win32_code type erases into system_code
				return native_handle_vt::error(win32::error_code{}, __FILE__, __LINE__);
			}
			return native_handle_vt::ok(h);
		} // open_file

		using buffer_vt = win32_valstat_trait<v_buffer::buffer_type>;

		buffer_vt::return_type log_file_path(string_view applicationName) {
			// determine the directory where we should write the log file
			char app_data_path_[MAX_PATH * 3]{0};
#ifdef _DEBUG
			HRESULT hr = ::SHGetFolderPathA(
				NULL,			// no owner window
				CSIDL_PERSONAL,
				NULL,
				SHGFP_TYPE_CURRENT,
				app_data_path_);
#else
			HRESULT hr = ::SHGetFolderPathAndSubDirA(
				NULL,			// no owner window
				CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE,
				NULL,
				SHGFP_TYPE_CURRENT,
				applicationName.data(),
				app_data_path_);
#endif
			if (SUCCEEDED(hr))
			{
				// form the path to the log file
				v_buffer::buffer_type buff_ =
					v_buffer::format("%s\\%s.log", app_data_path_, applicationName);

				return buffer_vt::ok(buff_);
			}
			else
			{
				return buffer_vt::error(win32::error_code{}, FILI);
			}
		} // log_file_path

		/*
		https://docs.microsoft.com/en-gb/windows/win32/api/fileapi/nf-fileapi-setfilepointer?redirectedfrom=MSDN
		it seems -1 indicated error?
		*/
		__int64 file_seek(HANDLE hf, __int64 distance, DWORD MoveMethod)
		{
			LARGE_INTEGER li;

			li.QuadPart = distance;

			li.LowPart = ::SetFilePointer(hf,
				li.LowPart,
				&li.HighPart,
				MoveMethod);

			if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError()
				!= NO_ERROR)
			{
				li.QuadPart = -1;
			}

			return li.QuadPart;
		}
#if 0
		class log_file final
		{
			v_buffer::buffer_type	log_file_path_;
			v_buffer::buffer_type	facility_;
			inline static long	sequence_number_{ 10000 };

			log_file() = delete;
			log_file(const log_file&) = delete;
			log_file(log_file&&) = delete;

			log_file(v_buffer::buffer_type lf_path_, const  char* facility)
				: log_file_path_(lf_path_), facility_(v_buffer::make(facility))
			{
			}

			using log_file_instance_vt = win32_valstat_trait< reference_wrapper<log_file>  >;

			static log_file_instance_vt::return_type
				instance(
					const  char* applicationName = nullptr,
					const  char* facility = nullptr)
			{
				/*
				this happens on the first visit + once
				if on that ocassion applicationName or
				facility are not given app exits
				*/
				static auto return_package_ = [&]()
					-> log_file_instance_vt::return_type
				{
					_ASSERTE(applicationName);
					_ASSERTE(facility);
					auto [log_file_path_, status] = log_file_path(applicationName);
					if (log_file_path_)
					{
						// this is where the static single instance is
						static log_file ssi_(*log_file_path_, facility);
						return log_file_instance_vt::ok(ssi_);
					}
					return log_file_instance_vt::error("could not make log file instance", __FILE__, __LINE__); ;
				}();
				return return_package_;
			}

			// from main we must call the factory op above
			friend int main(int, char * []);

		public:

			void log_assert_failed(int sourceLine, const  char* source_file_, const  char* time_stamp_)
			{
				try
				{
					v_buffer::buffer_type strError =
						v_buffer::format("ASSERT Failed: Line %ld, %s (%s)", sourceLine, source_file_, time_stamp_);
					append_entry_("Error", strError.data());
				}
				catch (...)
				{
					_ASSERTE(false);
				}
			}

			void log_error(const dbj::hresult& e)
			{
				// log_error(e.GetErrorCode(), e.GetLine(), e.GetFile(), e.GetTimestamp());
				log_error(e.code(), e.line(), e.file(), e.time_stamp());
			}

			void log_error(HRESULT hr, long sourceLine, const  char* source_file_, const  char* time_stamp_)
			{
				v_buffer::buffer_type strError =
					v_buffer::format("HRESULT %#08x: Line %ld, %s (%s)", hr, sourceLine, source_file_, time_stamp_);
				// not entirely safe?
				append_entry_("Error", strError.data());
			}
		private:
			void append_entry_(const  char* lpszCategory, const  char* lpszMessage)
			{
				// must have initialized to write an entry
				if (log_file_path_.size() == 0)
				{
					_ASSERTE(false);
					return;
				}

				try
				{
					// get the current time
					SYSTEMTIME currentTime;
					::GetSystemTime(&currentTime);
					currentTime.wDay;

					// calculate the log entry
					v_buffer::buffer_type log_entry_ =
						v_buffer::format("%s,%lu,%s,%05ld,%02hu-%02hu-%4hu %02hu:%02hu:%02hu,\"%s\",\"\"\r\n",
							facility_,
							::GetCurrentProcessId(),
							lpszCategory,
							::InterlockedIncrement(&sequence_number_),
							currentTime.wMonth,
							currentTime.wDay,
							currentTime.wYear,
							currentTime.wHour,
							currentTime.wMinute,
							currentTime.wSecond,
							lpszMessage);


					// Try to write the message.  Incrementally back off, waiting for the file to 
					// become available.  (The first backoff is 0ms intentionally -- to give up our 
					// scheduling quantum -- allowing another thread to run. Subsequent backoffs 
					// increase linearly at 10ms intervals with up to 10 retries)
					for (int i = 0; i < 10; i++)
					{
						if (do_append_entry(log_entry_))
							break;

						//	Sleep. Back off linearly, but not more than 2s.
						::Sleep(min(i * 10, 2000));
					}
				}
				catch (...)
				{
					// writing an entry should always be safe!
					_ASSERTE(false);
				}
			}

			bool do_append_entry(v_buffer::buffer_type strLogEntry)
			{
				// try to open the file		
				HANDLE file_handle = ::CreateFile(log_file_path_, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS);
				if (INVALID_HANDLE_VALUE != file_handle)
				{
					DWORD dwBufferSize = strLogEntry.size();
					DWORD dwBytesWritten = 0;
					if (int file_seek_rez_ = dbj::win::file_seek(file_handle, NULL, FILE_END);
						file_seek_rez_ > -1)
					{
						::WriteFile(file_handle, (const void*)strLogEntry.data(), dwBufferSize, &dwBytesWritten);
					}
					else {
						// ?
					}
					_ASSERTE(dwBufferSize == dwBytesWritten);

					bool flush_file_result = ::FlushFileBuffers(file_handle);
					bool close_handle_result = ::CloseHandle(file_handle);

					return true;
				}
				else
				{
					return false;
				}
			}

			friend log_file& log_file_instance()
			{
				auto [val, stat] = log_file::instance();
				_ASSERT(val); // program has no point if not having singleton instance
				return (*val); // actually calling reference_wrapper casting op.
			}
		}; // log_file
#endif
	} // win ns
} // dbj ns




#undef FILI
