// ProgressDlg.cpp	- Implementation of CProgressDlg
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
#include "ProgressDlg.h"



IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

CProgressDlg::CProgressDlg()
	: CDialog(CProgressDlg::IDD, NULL)
	, m_status(_T(""))
{
	VERIFY(Create(IDD, m_pParentWnd));
}

CProgressDlg::~CProgressDlg()
{
	if (IsWindow(m_hWnd))
		DestroyWindow();
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Text(pDX, IDC_STATUS, m_status);
}

void CProgressDlg::SetTotal(DWORD total)
{
	m_progress.SetRange32(0, total);
	m_sofar= 0;
}

void CProgressDlg::AddProgress(DWORD dw)
{
	m_sofar+= dw;
	m_progress.SetPos(m_sofar);
}

void CProgressDlg::SetStatus(LPCTSTR status)
{
	m_status= status;
	UpdateData(false);
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
END_MESSAGE_MAP()



// $Log: ProgressDlg.cpp,v $
// Revision 1.3  2004/11/05 16:53:05  assarbad
// Added Date and History tag where appropriate.
//
