// globalhelpers.h	- Declaration of global helper functions
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
// Last modified: $Date: 2004/11/28 14:40:06 $

#pragma once


CString GetLocaleString(LCTYPE lctype, LANGID langid);
CString GetLocaleLanguage(LANGID langid);
CString GetLocaleThousandSeparator();
CString GetLocaleDecimalSeparator();
CString FormatBytes(LONGLONG n);
CString FormatLongLongHuman(LONGLONG n);
CString FormatCount(LONGLONG n);
CString FormatDouble(double d);
CString PadWidthBlanks(CString n, int width);
CString FormatFileTime(const FILETIME& t);
CString FormatAttributes(DWORD attr);
CString FormatMilliseconds(DWORD ms);
CString GetParseNameOfMyComputer() throw (CException *);
void GetPidlOfMyComputer(LPITEMIDLIST *ppidl) throw (CException *);
void ShellExecuteWithAssocDialog(HWND hwnd, LPCTSTR filename) throw (CException *);
bool GetVolumeName(LPCTSTR rootPath, CString& volumeName);
CString FormatVolumeNameOfRootPath(CString rootPath);
CString FormatVolumeName(CString rootPath, CString volumeName);
CString PathFromVolumeName(CString name);
void MyGetDiskFreeSpace(LPCTSTR pszRootPath, LONGLONG& total, LONGLONG& unused);
CString GetFolderNameFromPath(LPCTSTR path);
CString GetCOMSPEC();
void WaitForHandleWithRepainting(HANDLE h);
bool FolderExists(LPCTSTR path);
bool DriveExists(const CString& path);
CString GetUserName();
bool IsHexDigit(int c);
CString MyQueryDosDevice(LPCTSTR drive);
bool IsSUBSTedDrive(LPCTSTR drive);
CString GetSpec_Bytes();
CString GetSpec_KB();
CString GetSpec_MB();
CString GetSpec_GB();
CString GetSpec_TB();

// $Log: globalhelpers.h,v $
// Revision 1.15  2004/11/28 14:40:06  assarbad
// - Extended CFileFindWDS to replace a global function
// - Now packing/unpacking the file attributes. This even spares a call to find encrypted/compressed files.
//
// Revision 1.14  2004/11/25 21:13:38  assarbad
// - Implemented "attributes" column in the treelist
// - Adopted width in German dialog
// - Provided German, Russian and English version of IDS_TREECOL_ATTRIBUTES
//
// Revision 1.13  2004/11/25 11:58:52  assarbad
// - Minor fixes (odd behavior of coloring in ANSI version, caching of the GetCompressedFileSize API)
//   for details see the changelog.txt
//
// Revision 1.12  2004/11/12 13:19:44  assarbad
// - Minor changes and additions (in preparation for the solution of the "Browse for Folder" problem)
//
// Revision 1.11  2004/11/07 20:14:30  assarbad
// - Added wrapper for GetCompressedFileSize() so that by default the compressed file size will be shown.
//
// Revision 1.10  2004/11/05 16:53:07  assarbad
// Added Date and History tag where appropriate.
//
