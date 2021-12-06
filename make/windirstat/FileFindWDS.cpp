// FileFindWDS.cpp	- Implementation of CFileFindWDS
//
// WinDirStat - Directory Statistics
// Copyright (C) 2004 Assarbad
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: assarbad@users.sourceforge.net
//
// Last modified: $Date: 2004/11/29 07:07:47 $

#include "StdAfx.h"
#include "FileFindWDS.h"
#include "windirstat.h"

CFileFindWDS::CFileFindWDS(void)
{
}

CFileFindWDS::~CFileFindWDS(void)
{
}

// Function to access the file attributes from outside
DWORD CFileFindWDS::GetAttributes() const
{
	ASSERT(m_hContext != NULL);
	ASSERT_VALID(this);

	if (m_pFoundInfo != NULL)
		return ((LPWIN32_FIND_DATA)m_pFoundInfo)->dwFileAttributes;
	else
		return INVALID_FILE_ATTRIBUTES;
}

// Wrapper for file size retrieval
// This function tries to return compressed file size whenever possible.
// If the file is not compressed the uncompressed size is being returned.
ULONGLONG CFileFindWDS::GetCompressedLength() const
{
	// Try to use the NT-specific API
	if (GetApp()->GetComprSizeApi()->IsSupported())
	{
		ULARGE_INTEGER ret;
		ret.LowPart = GetApp()->GetComprSizeApi()->GetCompressedFileSize(GetFilePath(), &ret.HighPart);
		
		// Check for error
		if ((GetLastError() != NO_ERROR) && (ret.LowPart == INVALID_FILE_SIZE))
			// IN case of an error return size from CFileFind object
			return GetLength();
		else
			return ret.QuadPart;
	}
	else
		// Use the file size already found by the finder object
		return GetLength();
}

// $Log: FileFindWDS.cpp,v $
// Revision 1.3  2004/11/29 07:07:47  bseifert
// Introduced SRECT. Saves 8 Bytes in sizeof(CItem). Formatting changes.
//
// Revision 1.2  2004/11/28 14:40:06  assarbad
// - Extended CFileFindWDS to replace a global function
// - Now packing/unpacking the file attributes. This even spares a call to find encrypted/compressed files.
//
// Revision 1.1  2004/11/25 23:07:24  assarbad
// - Derived CFileFindWDS from CFileFind to correct a problem of the ANSI version
//
