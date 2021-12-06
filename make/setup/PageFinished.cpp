// PageFinished.cpp	- Implementation of CPageFinished
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
#include ".\pagefinished.h"


IMPLEMENT_DYNAMIC(CPageFinished, CMyPropertyPage)

CPageFinished::CPageFinished()
	: CMyPropertyPage(CPageFinished::IDD)
	, m_message(_T(""))
	, m_startApplication(true)
	, m_instruction(_T(""))
{
}

CPageFinished::~CPageFinished()
{
}

void CPageFinished::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MESSAGE, m_message);
	DDX_Control(pDX, IDC_STARTAPPLICATION, m_ctlStartApplication);
	DDX_Check(pDX, IDC_STARTAPPLICATION, m_startApplication);
	DDX_Text(pDX, IDC_INSTRUCTION, m_instruction);
}


BEGIN_MESSAGE_MAP(CPageFinished, CMyPropertyPage)
END_MESSAGE_MAP()


BOOL CPageFinished::OnSetActive()
{
	BOOL b= CMyPropertyPage::OnSetActive();
	
	GetPropertySheet()->SetWizardButtons(PSWIZB_FINISH);
	
	CWnd *cancel= GetPropertySheet()->GetDlgItem(IDCANCEL);
	if (cancel != NULL)
		cancel->ShowWindow(SW_HIDE);

	if (m_message.IsEmpty())
		m_message= LoadString(IDS_SETUPSUCCESSFULL);
	else
		m_message= LoadString(IDS_ANERROROCCURRED) + m_message;

	if (!m_canStartApplication)
	{
		m_ctlStartApplication.ShowWindow(SW_HIDE);
		m_startApplication= false;

		m_instruction= LoadString(IDS_PRESSCLOSETOEXIT);
		GetPropertySheet()->SetFinishText(LoadString(IDS_CLOSE));
	}
	else
	{
		m_instruction= LoadString(IDS_PRESSFINISHTOCOMPLETE);
	}

	UpdateData(false);
	return b;
}

BOOL CPageFinished::OnWizardFinish()
{
	UpdateData();
	return CMyPropertyPage::OnWizardFinish();
}

// $Log: PageFinished.cpp,v $
// Revision 1.3  2004/11/05 16:53:05  assarbad
// Added Date and History tag where appropriate.
//
