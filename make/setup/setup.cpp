// setup.cpp	- Implementation of CSetupApp
//
// WinDirStat - Directory Statistics
// Copyright (C) 2003-2005 Bernhard Seifert
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
// Last modified: $Date: 2005/04/10 16:49:23 $

#include "stdafx.h"
#include "setup.h"

#include "osspecific.h"

#include "pagewelcome.h"
#include "pagedestfolder.h"
#include "pageshortcuts.h"
#include "pageready.h"
#include "progressdlg.h"
#include "pagefinished.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// #define TESTSLEEP	//#########

namespace
{
	// CreateLink - uses the Shell's IShellLink and IPersistFile interfaces 
	//              to create and store a shortcut to the specified object. 
	//
	// Parameters:
	// pszPathObj  - address of a buffer containing the path of the object. 
	// pszPathLink - address of a buffer containing the path where the 
	//                Shell link is to be stored.
	// pszDesc     - address of a buffer containing the description of the
	//                Shell link.
	void CreateLink(LPCTSTR pszPathObj, LPCTSTR pszPathLink, LPCTSTR pszDesc)
	{
		USES_CONVERSION;

		CComPtr<IShellLink> sl;
		HRESULT hr= CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&sl);
		MdThrowFailed(hr, LoadString(IDS_ERRORCREATINGSHORTCUT) + _T("CoCreateInstance(CLSID_ShellLink)"));

		hr= sl->SetPath(pszPathObj);
		MdThrowFailed(hr, LoadString(IDS_ERRORCREATINGSHORTCUT) + _T("IShellLink->SetPath()"));

		hr= sl->SetDescription(pszDesc);
		MdThrowFailed(hr, LoadString(IDS_ERRORCREATINGSHORTCUT) + _T("IShellLink->SetDescription()"));

		CComPtr<IPersistFile> pf;
		hr= sl->QueryInterface(IID_IPersistFile, (LPVOID*)&pf);
		MdThrowFailed(hr, LoadString(IDS_ERRORCREATINGSHORTCUT) + _T("QueryInterface(IPersistFile)"));

		hr= pf->Save(T2CW(pszPathLink), TRUE);
		MdThrowFailed(hr, LoadString(IDS_ERRORCREATINGSHORTCUT) + _T("PersistFile->Save()"));
	}
}



CSetupApp _theApp;

BEGIN_MESSAGE_MAP(CSetupApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CSetupApp::CSetupApp()
{
	m_shortcutStartMenu= false;
	m_shortcutDesktop= false;
	m_shortcutSendTo= false;
	m_startApplication= false;
}

BOOL CSetupApp::InitInstance()
{
	InitCommonControls();
	AfxOleInit();
	CWinApp::InitInstance();
	AfxEnableControlContainer();

	m_canStartApplication= false;

	if (!CheckPrerequisits())
		return false;

	if (!PreInstallSequence())
		return false;

	DoInstall();

	PostInstallSequence();

	if (m_startApplication)
		DoStartApplication();

	return false;
}

bool CSetupApp::CheckPrerequisits()
{
	CString wdsexe= GetAppFolder() + _T("\\windirstat.exe");
	if (!FileExists(wdsexe))
	{
		CString msg;
		msg.FormatMessage(IDS_FILEsDOESNOTEXIST, wdsexe);
		AfxMessageBox(msg);
		return false;
	}
	return true;
}

bool CSetupApp::PreInstallSequence()
{
	CPropertySheet sheet;
	sheet.m_psh.dwFlags&= ~PSH_HASHELP;

	CPageWelcome pageWelcome;
	CPageDestFolder pageDestFolder;
	CPageShortcuts pageShortcuts;
	CPageReady pageReady;

	sheet.AddPage(&pageWelcome);
	sheet.AddPage(&pageDestFolder);
	sheet.AddPage(&pageShortcuts);
	sheet.AddPage(&pageReady);

	sheet.SetWizardMode();

	if (ID_WIZFINISH == sheet.DoModal())
	{
		m_destFolder= pageDestFolder.m_destFolder;
		m_shortcutStartMenu= pageShortcuts.m_shortcutStartMenu;
		m_shortcutDesktop= pageShortcuts.m_shortcutDesktop;
		m_shortcutSendTo= pageShortcuts.m_shortcutSendTo;
		return true;
	}
	else
	{
		return false;
	}
}

void CSetupApp::DoInstall()
{
	try
	{
		CProgressDlg progress;

		CopyWinDirStatFiles(&progress);
		if (m_shortcutStartMenu)
			CreateStartMenuShortcut(&progress);
		if (m_shortcutDesktop)
			CreateDesktopShortcut(&progress);
		if (m_shortcutSendTo)
			CreateSendToShortcut(&progress);
	}
	catch (CException *pe)
	{
		m_errorMessage= MdGetExceptionMessage(pe);
		pe->Delete();
		if (m_canStartApplication)
		{
			CString msg;
			msg.FormatMessage(IDS_HOWEVERWINDIRSTATOKINs, m_destFolder);
			m_errorMessage+= msg;
		}
	}
}

void CSetupApp::PostInstallSequence()
{
	CPropertySheet sheet;
	sheet.m_psh.dwFlags&= ~PSH_HASHELP;

	CPageFinished pageFinished;
	pageFinished.m_message= m_errorMessage;
	pageFinished.m_canStartApplication= m_canStartApplication;

	sheet.AddPage(&pageFinished);
	sheet.SetWizardMode();
	if (ID_WIZFINISH == sheet.DoModal())
	{
		m_startApplication= pageFinished.m_startApplication;
	}
}

void CSetupApp::CopyWinDirStatFiles(CProgressDlg *progress)
{
	CStringArray files;
	progress->SetTotal(GetFilesToCopy(files));

	for (int i=0; i < files.GetSize(); i++)
	{
		CString dest= m_destFolder + _T("\\") + GetBaseNameFromPath(files[i]);
		// CopyFileEx is not available on Windows 95.
		// So we must do it ourselves.
		CopyOneFile(files[i], dest, progress);
	}
}

void CSetupApp::CopyOneFile(LPCTSTR src, LPCTSTR dst, CProgressDlg *progress)
{
	const UINT BUFFERSIZE = 16 * 1024;

	CString msg;
	msg.FormatMessage(IDS_COPYINGs, src);
	progress->SetStatus(msg);

#ifdef TESTSLEEP
	Sleep(2000);
#endif

	CFileException *fe= new CFileException;

	CFile fsrc;
	if (!fsrc.Open(src, CFile::modeRead|CFile::shareDenyWrite, fe))
		throw fe;

	CFile fdst;
	if (!fdst.Open(dst, CFile::modeCreate|CFile::modeReadWrite, fe))
		throw fe;

	fe->Delete();

	struct _cleanup
	{
		BYTE *buffer;
		_cleanup() { buffer= NULL; }
		~_cleanup() { delete[] buffer; }
	} cleanup;

	cleanup.buffer= new BYTE[BUFFERSIZE];

	UINT r= fsrc.Read(cleanup.buffer, BUFFERSIZE);
	while (r > 0)
	{
		fdst.Write(cleanup.buffer, r);
		progress->AddProgress(r);
		r= fsrc.Read(cleanup.buffer, BUFFERSIZE);
	}

	if (GetBaseNameFromPath(dst).CompareNoCase(_T("windirstat.exe")) == 0)
		m_canStartApplication= true;
}

DWORD CSetupApp::GetFilesToCopy(CStringArray& files)
{
	files.RemoveAll();

	ULONGLONG total= 0;

	CFileFind finder;

	BOOL b= finder.FindFile(GetAppFolder() + _T("\\windirstat.*"));
	while (b)
	{
		b= finder.FindNextFile();
		if (finder.IsDirectory())
			continue;
		files.Add(finder.GetFilePath());
                // Retrieve file size
		total+= finder.GetLength();
	}
	finder.Close();

	b= finder.FindFile(GetAppFolder() + _T("\\wdsr*.dll"));
	while (b)
	{
		b= finder.FindNextFile();
		if (finder.IsDirectory())
			continue;
		files.Add(finder.GetFilePath());
		// Retrieve file size
                total+= finder.GetLength();
	}
	finder.Close();

	b= finder.FindFile(GetAppFolder() + _T("\\wdsh*.chm"));
	while (b)
	{
		b= finder.FindNextFile();
		if (finder.IsDirectory())
			continue;
		files.Add(finder.GetFilePath());
		// Retrieve file size
                total+= finder.GetLength();
	}
	finder.Close();

	return (DWORD)total;
}


void CSetupApp::CreateStartMenuShortcut(CProgressDlg *progress)
{
	progress->SetStatus(LoadString(IDS_CREATEINGSTARTMENUSHORTCUT));
	// First, try CSIDL_COMMON_PROGRAMS ("valid on NT only")
	try
	{
		if (PlatformIsWindows9x())
			MdThrowStringException(_T("Platform W9x: Common Start Menu unsupported"));

		CreateShortcutInSpecialFolder(CSIDL_COMMON_PROGRAMS);
	}
	catch (CException *pe)
	{
		TRACE(MdGetExceptionMessage(pe));
		pe->Delete();

		// Second, try CSIDL_PROGRAMS
		CreateShortcutInSpecialFolder(CSIDL_PROGRAMS);
	}
}

void CSetupApp::CreateDesktopShortcut(CProgressDlg *progress)
{
	progress->SetStatus(LoadString(IDS_CREATEINGDESKTOPSHORTCUT));
	// First, try CSIDL_COMMON_DESKTOPDIRECTORY ("valid on NT only")
	try
	{
		CreateShortcutInSpecialFolder(CSIDL_COMMON_DESKTOPDIRECTORY);
	}
	catch (CException *pe)
	{
		TRACE(MdGetExceptionMessage(pe));
		pe->Delete();

		// Second, try CSIDL_DESKTOPDIRECTORY
		CreateShortcutInSpecialFolder(CSIDL_DESKTOPDIRECTORY);
	}
}

void CSetupApp::CreateSendToShortcut(CProgressDlg *progress)
{
	progress->SetStatus(LoadString(IDS_CREATEINGSENDTOSHORTCUT));
	// There is no CSIDL_COMMON_SENDTO.
	CreateShortcutInSpecialFolder(CSIDL_SENDTO);
}

void CSetupApp::CreateShortcutInSpecialFolder(int csidl)
{
#ifdef TESTSLEEP
	Sleep(2000);
#endif

	CShFolderApi shf;

	if (!shf.IsSupported())
	{
		MdThrowStringExceptionF(IDS_CANNOTCREATESHORTCUT);
	}
	CString path;

	HRESULT hr= shf.SHGetFolderPath(NULL, csidl, NULL, 0, path.GetBuffer(_MAX_PATH));
	path.ReleaseBuffer();

	if (hr != S_OK)
		MdThrowStringExceptionF(IDS_SHGETFOLDERPATHdFAILEDs, csidl, MdGetWinerrorText(hr));

	path+= _T("\\WinDirStat.lnk");

	CreateLink(GetDestExecutable(), path, _T("WinDirStat"));
}


void CSetupApp::DoStartApplication()
{
	try
	{
		MyShellExecute(NULL, _T("open"), GetDestExecutable(), NULL, NULL, SW_SHOWNORMAL);
	}
	catch (CException *pe)
	{
		pe->ReportError();
		pe->Delete();
	}
}

CString CSetupApp::GetDestExecutable()
{
	return m_destFolder + _T("\\windirstat.exe");
}

// $Log: setup.cpp,v $
// Revision 1.11  2005/04/10 16:49:23  assarbad
// - Some smaller fixes including moving the resource string version into the rc2 files
//
// Revision 1.10  2004/12/24 10:39:27  bseifert
// Added Polish setup and  Polish help file.
//
// Revision 1.9  2004/12/19 10:52:36  bseifert
// Minor fixes.
//
// Revision 1.8  2004/11/19 06:55:36  bseifert
// Implemented SendTo. Synced resource.h's. Minor corrections in Russian windirstat.rc.
//
// Revision 1.7  2004/11/07 20:14:30  assarbad
// - Added wrapper for GetCompressedFileSize() so that by default the compressed file size will be shown.
//
// Revision 1.6  2004/11/07 10:14:04  bseifert
// Minor bugfix in error handling.
//
// Revision 1.5  2004/11/05 16:53:05  assarbad
// Added Date and History tag where appropriate.
//
