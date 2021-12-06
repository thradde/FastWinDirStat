// osspecific.cpp	- Implementation of CShFolderApi
//
// WinDirStat - Directory Statistics
// Copyright (C) 2003-2004 Bernhard Seifert
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
// Author: bseifert@users.sourceforge.net, bseifert@daccord.net
//
// Last modified: $Date: 2004/11/05 16:53:05 $

#include "stdafx.h"
#include "osspecific.h"

#ifdef UNICODE
#define TSPEC "W"
#else
#define TSPEC "A"
#endif

#define GETPROC(name) m_##name = ( m_dll != 0 ? (Type##name)GetProcAddress(m_dll, #name) : NULL )
#define TGETPROC(name) m_##name = ( m_dll != 0 ? (Type##name)GetProcAddress(m_dll, #name TSPEC) : NULL )

#define CHECK(name) if (m_##name == 0) return false


CShFolderApi::CShFolderApi()
{
	m_dll= LoadLibrary(_T("shfolder.dll"));

	TGETPROC(SHGetFolderPath);
}

CShFolderApi::~CShFolderApi()
{
	if (m_dll != NULL)
		FreeLibrary(m_dll);
}

bool CShFolderApi::IsSupported()
{
	CHECK(SHGetFolderPath);
	return true;
}

HRESULT CShFolderApi::SHGetFolderPath(HWND hwnd, int csidl, HANDLE hToken, DWORD dwFlags, LPTSTR pszPath)
{
	ASSERT(IsSupported());
	return (*m_SHGetFolderPath)(hwnd, csidl, hToken, dwFlags, pszPath);
}





// $Log: osspecific.cpp,v $
// Revision 1.3  2004/11/05 16:53:05  assarbad
// Added Date and History tag where appropriate.
//
