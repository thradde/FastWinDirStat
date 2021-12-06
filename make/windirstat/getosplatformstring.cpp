// getosplatformstring.cpp	- Implementation of GetOsPlatformString()
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
// Last modified: $Date: 2004/11/05 16:53:07 $

#include "stdafx.h"
#include "windirstat.h"
#include "getosplatformstring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString GetOsPlatformString()
{
	CString ret;

	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize= sizeof(osvi);

	if (!GetVersionEx(&osvi))
	{
		return LoadString(IDS__UNKNOWN_);
	}

	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
			ret= _T("Windows Server 2003");
		else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
			ret= _T("Windows XP");
		else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
			ret= _T("Windows 2000");
		else if (osvi.dwMajorVersion <= 4)
			ret= _T("Windows NT");
		else
            ret.Format(_T("Windows %u.%u"), osvi.dwMajorVersion, osvi.dwMinorVersion);
		if (lstrlen(osvi.szCSDVersion) > 0)
		{
			CString s;
			s.Format(_T(" (%s)"), osvi.szCSDVersion);
			ret+= s;
		}
		break;

	case VER_PLATFORM_WIN32_WINDOWS:
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			ret= _T("Windows 95");
			if (osvi.szCSDVersion[1] == _T('C') || osvi.szCSDVersion[1] == _T('B'))
				ret+= _T(" OSR2");
		} 
		else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			ret= _T("Windows 98");
			if (osvi.szCSDVersion[1] == _T('A'))
				ret+= _T(" SE");
		} 
		else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			ret= _T("Windows ME");
		}
		else
		{
			ret.Format(_T("<platform %u %u.%u>"), osvi.dwPlatformId, osvi.dwMajorVersion, osvi.dwMinorVersion);
		}
		break;

	case VER_PLATFORM_WIN32s:
		ret= _T("Win32s\n"); // ooops!!
		break;

	default:
		ret.Format(_T("<platform id %u>"), osvi.dwPlatformId);
		break;
	}

	return ret;
}



 

// $Log: getosplatformstring.cpp,v $
// Revision 1.3  2004/11/05 16:53:07  assarbad
// Added Date and History tag where appropriate.
//
