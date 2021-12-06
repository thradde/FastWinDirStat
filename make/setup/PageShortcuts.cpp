// PageShortcuts.cpp	- Implementation of CPageShortcuts
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
// Last modified: $Date: 2004/11/19 06:55:26 $

#include "stdafx.h"
#include "setup.h"
#include ".\pageshortcuts.h"


IMPLEMENT_DYNAMIC(CPageShortcuts, CMyPropertyPage)

CPageShortcuts::CPageShortcuts()
	: CMyPropertyPage(CPageShortcuts::IDD)
	, m_shortcutStartMenu(true)
	, m_shortcutDesktop(true)
	, m_shortcutSendTo(true)
{
}

CPageShortcuts::~CPageShortcuts()
{
}

void CPageShortcuts::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_SHORTCUTSTARTMENU, m_shortcutStartMenu);
	DDX_Check(pDX, IDC_SHORTCUTDESKTOP, m_shortcutDesktop);
	DDX_Check(pDX, IDC_SHORTCUTSENDTO, m_shortcutSendTo);
}


BEGIN_MESSAGE_MAP(CPageShortcuts, CMyPropertyPage)
END_MESSAGE_MAP()


BOOL CPageShortcuts::OnSetActive()
{
	BOOL b= CMyPropertyPage::OnSetActive();
	GetPropertySheet()->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	return b;
}

LRESULT CPageShortcuts::OnWizardNext()
{
	UpdateData();
	return CMyPropertyPage::OnWizardNext();
}

// $Log: PageShortcuts.cpp,v $
// Revision 1.4  2004/11/19 06:55:26  bseifert
// Implemented SendTo. Synced resource.h's. Minor corrections in Russian windirstat.rc.
//
// Revision 1.3  2004/11/05 16:53:05  assarbad
// Added Date and History tag where appropriate.
//
