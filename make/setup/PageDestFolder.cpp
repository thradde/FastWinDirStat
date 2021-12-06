// PageDestFolder.cpp	- Implementation of CPageDestFolder
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
#include "setup.h"
#include "osspecific.h"
#include ".\pagedestfolder.h"

namespace
{
	bool MySplitPath(const CString& path, CString& drive, CStringArray& folders);
	CString MyCombinePath(CString drive, const CStringArray& folders, int folderCount);
	CString MyCombinePath(CString drive, const CStringArray& folders);


	bool MySplitPath(const CString& path, CString& drive, CStringArray& folders)
	{
		drive.Empty();
		folders.RemoveAll();
		
		CString rest= path;

		if (rest.GetLength() >= 2 && rest[1] == _T(':'))
		{
			if (rest.GetLength() >= 3 && rest[2] == _T('\\'))
			{
				drive= rest.Left(3);
				rest= rest.Mid(3);
			}
			else
			{
				drive= rest.Left(2);
				rest= rest.Mid(2);
			}
		}
		else if (rest.GetLength() >= 2 && rest.Left(2) == _T("\\\\"))
		{
			int serverEnd= rest.Find(_T('\\'), 2);
			if (serverEnd == -1)
				return false;	// server specified but no share
			int shareEnd= rest.Find(_T('\\'), serverEnd + 1);
			if (shareEnd == -1)
				return false;	// share must have trailing backslash
			if (shareEnd == serverEnd + 1)
				return false;	// empty share
			drive= rest.Left(shareEnd + 1);
			rest= rest.Mid(shareEnd + 1);
		}
		else if (rest.GetLength() >= 1 && rest[0] == _T('\\'))
		{
			drive= _T("\\");
			rest= rest.Mid(1);
		}

		while (!rest.IsEmpty())
		{
			CString f;
			int k= rest.Find(_T('\\'));
			if (k == -1)
			{
				f= rest;
				rest.Empty();
			}
			else
			{
				f= rest.Left(k);
				rest= rest.Mid(k + 1);
				if (rest.IsEmpty())
					return false;	// trailing backslash
			}
			if (f.IsEmpty())
				return false;		// empty component
			
			folders.Add(f);
		}

		ASSERT(MyCombinePath(drive, folders) == path);
		return true;
	}

	CString MyCombinePath(CString drive, const CStringArray& folders, int folderCount)
	{
		CString path= drive;
		for (int i=0; i < folderCount; i++)
		{
			if (i > 0)
				path+= _T("\\");
			path+= folders[i];
		}
		return path;
	}

	CString MyCombinePath(CString drive, const CStringArray& folders)
	{
		return MyCombinePath(drive, folders, folders.GetCount());
	}

	#ifdef _DEBUG
	void TestMySplitPath()
	{
		CString drive;
		CStringArray folders;

		ASSERT(!MySplitPath(_T("C:\\trailingbackslash\\"), drive, folders));
		ASSERT(!MySplitPath(_T("C:\\emptycomponent\\\\programme"), drive, folders));

		ASSERT(MySplitPath(_T("C:\\programme\\windirstat"), drive, folders));
		ASSERT(drive == _T("C:\\"));
		ASSERT(folders.GetCount() == 2);
		ASSERT(folders[0] == _T("programme"));
		ASSERT(folders[1] == _T("windirstat"));

		ASSERT(MySplitPath(_T("C:relative\\windirstat"), drive, folders));
		ASSERT(drive == _T("C:"));
		ASSERT(folders.GetCount() == 2);
		ASSERT(folders[0] == _T("relative"));
		ASSERT(folders[1] == _T("windirstat"));

		ASSERT(MySplitPath(_T("C:\\"), drive, folders));
		ASSERT(drive == _T("C:\\"));
		ASSERT(folders.GetCount() == 0);

		ASSERT(MySplitPath(_T("C:"), drive, folders));
		ASSERT(drive == _T("C:"));
		ASSERT(folders.GetCount() == 0);

		ASSERT(MySplitPath(_T("\\programme\\windirstat"), drive, folders));
		ASSERT(drive == _T("\\"));
		ASSERT(folders.GetCount() == 2);
		ASSERT(folders[0] == _T("programme"));
		ASSERT(folders[1] == _T("windirstat"));

		ASSERT(!MySplitPath(_T("\\\\server"), drive, folders));
		ASSERT(!MySplitPath(_T("\\\\server\\share"), drive, folders));
		ASSERT(!MySplitPath(_T("\\\\server\\\\programme"), drive, folders));

		ASSERT(MySplitPath(_T("\\\\server\\share\\"), drive, folders));
		ASSERT(drive == _T("\\\\server\\share\\"));
		ASSERT(folders.GetCount() == 0);

		ASSERT(MySplitPath(_T("\\\\server\\share\\programme"), drive, folders));
		ASSERT(drive == _T("\\\\server\\share\\"));
		ASSERT(folders.GetCount() == 1);
		ASSERT(folders[0] == _T("programme"));
	}
	#endif
}


IMPLEMENT_DYNAMIC(CPageDestFolder, CMyPropertyPage)

CPageDestFolder::CPageDestFolder()
	: CMyPropertyPage(CPageDestFolder::IDD)
	, m_destFolder(_T(""))
{
#ifdef _DEBUG
	TestMySplitPath();
#endif
}

CPageDestFolder::~CPageDestFolder()
{
}

void CPageDestFolder::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DESTFOLDER, m_destFolder);
}


BEGIN_MESSAGE_MAP(CPageDestFolder, CMyPropertyPage)
	ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
END_MESSAGE_MAP()


BOOL CPageDestFolder::OnSetActive()
{
	BOOL b= CMyPropertyPage::OnSetActive();

	GetPropertySheet()->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	return b;
}

BOOL CPageDestFolder::OnInitDialog()
{
	CMyPropertyPage::OnInitDialog();

	CString s;
	HRESULT hr= S_OK;

	CShFolderApi shfolder;
	if (shfolder.IsSupported())
	{
		hr= shfolder.SHGetFolderPath(m_hWnd, CSIDL_PROGRAM_FILES, NULL, 0, s.GetBuffer(_MAX_PATH));
		s.ReleaseBuffer();
	}

	if (!shfolder.IsSupported() || FAILED(hr) || s.IsEmpty())
	{
		UINT u= GetWindowsDirectory(s.GetBuffer(_MAX_PATH), MAX_PATH);
		s.ReleaseBuffer();
		if (u == 0)
			s= _T("C:");
		else
			s= s.Left(2); // "C:"
	}

	ASSERT(s.GetLength() > 0);

	if (s[s.GetLength() - 1] != _T('\\'))
		s+= _T("\\");

	s+= _T("windirstat");

	m_destFolder= s;
	UpdateData(false);
	return TRUE;
}

LRESULT CPageDestFolder::OnWizardNext()
{
	//CreateDirectory("C:\\blbla\\a", NULL);
	// ERROR_PATH_NOT_FOUND

	//CreateDirectory("C:\\a:", NULL);
	//ERROR_INVALID_NAME

	//CreateDirectory("C:\\", NULL);
	// ERROR_ACCESS_DENIED	

	UpdateData();

	if (PathHasInvalidCharacters(m_destFolder))
	{
		AfxMessageBox(IDS_PATHHASINVALIDCHARS);
		return -1;
	}

	m_destFolder= MyGetFullPathName(m_destFolder);
	UpdateData(false);

	CString drive;
	CStringArray folders;
	if (!MySplitPath(m_destFolder, drive, folders))
	{
		AfxMessageBox(IDS_PATHISINVALID);
		return -1;
	}

	CString test;
	if (folders.GetCount() > 0 && folders[folders.GetCount() - 1].CompareNoCase(_T("windirstat")) == 0)
		test= MyCombinePath(drive, folders, folders.GetCount() - 1);
	else
		test= MyCombinePath(drive, folders);

	try
	{
		if (!SetCurrentDirectory(test))
		{
			CString msg;
			msg.FormatMessage(IDS_DIRECTORYsDOESNOTEXIST, test);
			if (IDYES != AfxMessageBox(msg, MB_YESNO))
				return -1;
		}
		DeepCreateDirectory(drive, folders);
	}
	catch (CException *pe)
	{
		pe->ReportError();
		pe->Delete();
		return -1;
	}

	if (m_destFolder.CompareNoCase(GetAppFolder()) == 0)
	{
		AfxMessageBox(IDS_CANNOTINSTALLTOSETUPDIR);
		return -1;
	}

	return CMyPropertyPage::OnWizardNext();
}

bool CPageDestFolder::PathHasInvalidCharacters(CString path)
{
	return (path.FindOneOf(_T("/*?\"<>")) != -1);
}

void CPageDestFolder::DeepCreateDirectory(CString drive, const CStringArray& folders)
{
	if (!SetCurrentDirectory(drive))
		MdThrowStringExceptionF(IDS_CANNOTCHANGETOss, drive, MdGetWinerrorText(GetLastError()));

	CString current= drive;

	for (int i=0; i < folders.GetCount(); i++)
	{
		if (i > 0)
			current+= _T("\\");
		current+= folders[i];

		if (!SetCurrentDirectory(current))
		{
			if (!CreateDirectory(current, NULL))
				MdThrowStringExceptionF(IDS_CANNOTCREATEDIRss, current, MdGetWinerrorText(GetLastError()));
		}
	}
}


void CPageDestFolder::OnBnClickedBrowse()
{
	CString sDisplayName;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(bi));

	CString title= LoadString(IDS_CHOOSEDESTFOLDER);
	bi.hwndOwner= m_hWnd;
	bi.pszDisplayName= sDisplayName.GetBuffer(_MAX_PATH);
	bi.lpszTitle= title;
	bi.ulFlags= BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_NEWDIALOGSTYLE;
	
	LPITEMIDLIST pidl= SHBrowseForFolder(&bi);
	sDisplayName.ReleaseBuffer();

	if (pidl != NULL)
	{
		CString sDir;

		LPSHELLFOLDER pshf;
		HRESULT hr= SHGetDesktopFolder(&pshf); 
		ASSERT(SUCCEEDED(hr));
		
		STRRET strret;
		strret.uType= STRRET_CSTR;
		hr= pshf->GetDisplayNameOf(pidl, SHGDN_FORPARSING, &strret);
		ASSERT(SUCCEEDED(hr));
		sDir= MyStrRetToString(pidl, &strret);

		CoTaskMemFree(pidl);
		pshf->Release();

		m_destFolder= sDir;
		UpdateData(false);
	}
}

// $Log: PageDestFolder.cpp,v $
// Revision 1.4  2004/11/05 16:53:05  assarbad
// Added Date and History tag where appropriate.
//
