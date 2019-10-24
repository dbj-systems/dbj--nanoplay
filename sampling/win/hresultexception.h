// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
/*
OpenLiveWriter/src/unmanaged/OpenLiveWriter.CppUtils/Include/HResultException.h
*/

#pragma once
#include "../../common.h"

namespace dbj::nanoplay::win {

	using namespace std;
	using namespace dbj::nanolib;

	// The return value of COM functions and methods is an HRESULT.
	// This is not a handle to anything, but is merely a 32-bit value
	// with several fields encoded in the value.
	class hresult
	{
	public:
		hresult(HRESULT hr, string_view file, long line, string_view timestamp)
			:
			hr_(hr),
			file_(v_buffer::make(file)),
			m_line(line),
			timestamp_(v_buffer::make(timestamp))
		{
		}

		HRESULT code() const { return hr_; }
		v_buffer::buffer_type file() const { return file_; }
		long line() const { return m_line; }
		v_buffer::buffer_type time_stamp() const { return timestamp_; }

	private:
		HRESULT hr_;
		v_buffer::buffer_type	file_;
		long					m_line;
		v_buffer::buffer_type	timestamp_;
	};

	inline auto
		DBJ_CHECK_HRESULT(DWORD hr, string_view file, int line, string_view timestamp)
	{
		if (FAILED(hr))
			return hresult(hr, file, line, timestamp);

		// very dubious? is this default "not an error HRESULT" ?
		return hresult(0, file, line, timestamp);
	}

#define DBJ_HRESULT(hr) hresult(hr, __FILE__, __LINE__, __TIMESTAMP__)
#define DBJ_WIN32_HRESULT(dwError) DBJ_HRESULT(HRESULT_FROM_WIN32(dwError))
#define CHECK_HRESULT(hr) DBJ_CHECK_HRESULT(hr, __FILE__, __LINE__, __TIMESTAMP__)

#ifdef ORIGINAL_IMPLEMENTATION

	class HResultException
	{
	public:
		HResultException(HRESULT hr, const CStringA& file, int line, const CStringA& timestamp)
			: m_hr(hr), m_file(file), m_line(line), m_timestamp(timestamp)
		{
		}

		HRESULT GetErrorCode() const { return m_hr; }
		CStringA GetFile() const { return m_file; }
		int GetLine() const { return m_line; }
		CStringA GetTimestamp() const { return m_timestamp; }

	private:
		HRESULT m_hr;
		CStringA m_file;
		int m_line;
		CStringA m_timestamp;
	};


#define THROW_HRESULT(hr) throw HResultException(hr, __FILE__, __LINE__, __TIMESTAMP__)
#define THROW_WIN32(dwError) THROW_HRESULT(HRESULT_FROM_WIN32(dwError))
#define CHECK_HRESULT(hr) _INLINE_CHECK_HRESULT(hr, __FILE__, __LINE__, __TIMESTAMP__)

	inline void _INLINE_CHECK_HRESULT(DWORD hr, const CStringA& file, int line, const CStringA& timestamp)
	{
		if (FAILED(hr))
			throw HResultException(hr, file, line, timestamp);
	}

#endif // ORIGINAL_IMPLEMENTATION

} // dbj