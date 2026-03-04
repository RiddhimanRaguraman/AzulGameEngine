//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "File.h"

namespace Azul
{
	// holds the directory internally to File.cpp
	static char BaseDir[File::BASE_DIR_SIZE] = { 0 };

	File::Error File::ClearBaseDir() noexcept
	{
		std::memset(BaseDir, 0, sizeof(BaseDir));
		return File::Error::SUCCESS;
	}

	File::Error File::GetBaseDir(char* const pDirOut, size_t DirOutSize) noexcept
	{
		if (pDirOut == nullptr)
		{
			return File::Error::BASE_DIR_NULLPTR_FAIL;
		}

		const size_t len = std::strlen(BaseDir);
		if (len == 0)
		{
			return File::Error::BASE_DIR_FAIL; // no base dir set
		}

		if (DirOutSize < len + 1)
		{
			return File::Error::BASE_DIR_INSUFFICIENT_SPACE_FAIL;
		}

		std::memcpy(pDirOut, BaseDir, len + 1);
		return File::Error::SUCCESS;
	}

	File::Error File::SetBaseDir(const char* const pDir) noexcept
	{
		if (pDir == nullptr)
		{
			return File::Error::BASE_DIR_NULLPTR_FAIL;
		}

		const size_t len = std::strlen(pDir);
		if (len >= File::BASE_DIR_SIZE)
		{
			return File::Error::BASE_DIR_INSUFFICIENT_SPACE_FAIL;
		}

		std::memcpy(BaseDir, pDir, len + 1);
		return File::Error::SUCCESS;
	}

	bool File::IsHandleValid(File::Handle fh) noexcept
	{
		DWORD flags;
		return GetHandleInformation(fh, &flags) != 0;
	}

	File::Error File::GetFileAsBuffer(const char* const pFileName, unsigned char*& pBuff, unsigned int& numBytes) noexcept
	{
		File::Handle fh;
		File::Error  ferror;

		ferror = File::Open(fh, pFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);

		// Get the size
		ferror = File::Seek(fh, File::Position::END, 0);
		assert(ferror == File::Error::SUCCESS);

		DWORD length;
		ferror = File::Tell(fh, length);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Seek(fh, File::Position::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		unsigned char* buff = new unsigned char[length];
		assert(buff);

		ferror = File::Read(fh, buff, length);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Close(fh);

		numBytes = (unsigned int)length;
		pBuff = buff;
		return ferror;
	}

    DWORD File::privGetFileDesiredAccess(File::Mode mode) noexcept
    {
        switch (mode)
        {
        case Mode::WRITE:      return GENERIC_WRITE;
        case Mode::READ:       return GENERIC_READ;
        case Mode::READ_WRITE: return GENERIC_WRITE | GENERIC_READ;
        default:               return 0;
        }
    }

    DWORD File::privGetSeek(File::Position location) noexcept
    {
        switch (location)
        {
        case File::Position::BEGIN:   return FILE_BEGIN;
        case File::Position::CURRENT: return FILE_CURRENT;
        case File::Position::END:     return FILE_END;
        default:                      return FILE_BEGIN;
        }
    }

    File::Error File::Open(File::Handle& fh, const char* const fileName, File::Mode mode, bool UseBaseAddr) noexcept
    {
		if (fileName == nullptr)
		{
			return File::Error::OPEN_FILENAME_FAIL;
		}

		// Opening one that is already open
		if (IsHandleValid(fh))
		{
			return File::Error::OPEN_FAIL;
		}

        DWORD accessFlags = privGetFileDesiredAccess(mode);
        if (accessFlags == 0)
        {
            return File::Error::OPEN_FAIL;
        }

		// Build full path when requested
		char fullPath[File::BASE_DIR_SIZE + MAX_PATH] = { 0 };
		const char* path = fileName;
		if (UseBaseAddr)
		{
			const size_t baseLen = std::strlen(BaseDir);
			if (baseLen == 0)
			{
				return File::Error::OPEN_BASE_DIR_FAIL;
			}

			strncpy_s(fullPath, BaseDir, _TRUNCATE);
			if (fullPath[baseLen - 1] != '\\' && fullPath[baseLen - 1] != '/')
			{
				strncat_s(fullPath, "\\", _TRUNCATE);
			}
			strncat_s(fullPath, fileName, _TRUNCATE);
			path = fullPath;
		}

		fh = CreateFileA(path,
			accessFlags,
			FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
			nullptr,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		if (fh == INVALID_HANDLE_VALUE)
		{
			return File::Error::OPEN_FAIL;
		}

		return File::Error::SUCCESS;
	}

	File::Error File::Close(File::Handle& fh) noexcept
	{
		if (!IsHandleValid(fh))
		{
			return File::Error::CLOSE_FAIL;
		}

		if (!CloseHandle(fh))
		{
			return File::Error::CLOSE_FAIL;
		}

		fh = reinterpret_cast<Handle>((LONG_PTR)0xFFFFFF);
		return File::Error::SUCCESS;
	}

	File::Error File::Write(File::Handle fh, const void* const buffer, const DWORD inSize) noexcept
	{
		DWORD bytesWritten = 0;
		if (!WriteFile(fh, buffer, inSize, &bytesWritten, nullptr) || bytesWritten != inSize)
		{
			return File::Error::WRITE_FAIL;
		}
		return File::Error::SUCCESS;
	}

	File::Error File::Read(File::Handle fh, void* const buffer, const DWORD inSize) noexcept
	{
		DWORD bytesRead = 0;
		if (!ReadFile(fh, buffer, inSize, &bytesRead, nullptr) || bytesRead != inSize)
		{
			return File::Error::READ_FAIL;
		}
		return File::Error::SUCCESS;
	}

	File::Error File::Seek(File::Handle fh, File::Position location, int offset) noexcept
	{
        DWORD origin = privGetSeek(location);

		DWORD result = SetFilePointer(fh, offset, nullptr, origin);
		if (result == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
		{
			return File::Error::SEEK_FAIL;
		}
		return File::Error::SUCCESS;
	}

	File::Error File::Tell(File::Handle fh, DWORD& offset) noexcept
	{
		DWORD cur = SetFilePointer(fh, 0, nullptr, FILE_CURRENT);

		if (cur == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
		{
			return File::Error::TELL_FAIL;
		}

		offset = cur;
		return File::Error::SUCCESS;
	}

	File::Error File::Flush(File::Handle fh) noexcept
	{
		if (!FlushFileBuffers(fh))
		{
			return File::Error::FLUSH_FAIL;
		}
		return File::Error::SUCCESS;
	}

}

// --- End of File ---
