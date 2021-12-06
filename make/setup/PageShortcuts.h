// pageshortcuts.h	- Declaration of CPageShortcuts
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
// Last modified: $Date: 2004/11/19 06:55:36 $

#pragma once

#include "mypropertypage.h"

//
// CPageShortcuts. The wizard page querying whether shortcuts shall be created.
//
class CPageShortcuts: public CMyPropertyPage
{
	DECLARE_DYNAMIC(CPageShortcuts)
	enum { IDD = IDD_SHORTCUTS };

public:
	CPageShortcuts();
	virtual ~CPageShortcuts();

	BOOL m_shortcutStartMenu;	// [out]
	BOOL m_shortcutDesktop;		// [out]

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();

	DECLARE_MESSAGE_MAP()

public:
	BOOL m_shortcutSendTo;
};

// $Log: PageShortcuts.h,v $
// Revision 1.4  2004/11/19 06:55:36  bseifert
// Implemented SendTo. Synced resource.h's. Minor corrections in Russian windirstat.rc.
//
// Revision 1.3  2004/11/05 16:53:05  assarbad
// Added Date and History tag where appropriate.
//
