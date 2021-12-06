// pagefinished.h	- Declaration of CPageFinished
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

#pragma once

#include "mypropertypage.h"
#include "afxwin.h"


//
// CPageFinished. The very last wizard page querying whether windirstat shall be started.
//
class CPageFinished: public CMyPropertyPage
{
	DECLARE_DYNAMIC(CPageFinished)
	enum { IDD = IDD_FINISHED };

public:
	CPageFinished();
	virtual ~CPageFinished();

	CString m_message;			// [in] empty if successful, else error message
	bool m_canStartApplication;	// [in] true, if windirstat.exe successfully installed

	BOOL m_startApplication;	// [out]

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();

	DECLARE_MESSAGE_MAP()

	CButton m_ctlStartApplication;
	CString m_instruction;
};

// $Log: PageFinished.h,v $
// Revision 1.3  2004/11/05 16:53:05  assarbad
// Added Date and History tag where appropriate.
//
