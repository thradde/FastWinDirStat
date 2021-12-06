// layout.cpp	- Implementation of CLayout
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
#include "options.h"
#include ".\layout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLayout::CLayout(CWnd *dialog, LPCTSTR name)
{
	ASSERT(dialog != NULL);
	m_dialog= dialog;
	m_name= name;
	
	// This is necessary because OnGetMinMaxInfo() will be called
	// before OnInitDialog!
	m_originalDialogSize.cx= 0;
	m_originalDialogSize.cy= 0;
}

int CLayout::AddControl(CWnd *control, double movex, double movey, double stretchx, double stretchy)
{
	SControlInfo info;
	
	info.control= control;
	info.movex= movex;
	info.movey= movey;
	info.stretchx= stretchx;
	info.stretchy= stretchy;
	
	return m_control.Add(info);
}

void CLayout::AddControl(UINT id, double movex, double movey, double stretchx, double stretchy)
{
	AddControl(m_dialog->GetDlgItem(id), movex, movey, stretchx, stretchy);
}

void CLayout::OnInitDialog(bool centerWindow)
{
	m_dialog->SetIcon(GetApp()->LoadIcon(IDR_MAINFRAME), false);

	CRect rcDialog;
	m_dialog->GetWindowRect(rcDialog);
	m_originalDialogSize= rcDialog.Size();

	for (int i=0; i < m_control.GetSize(); i++)
	{
		CRect rc;
		m_control[i].control->GetWindowRect(rc);
		m_dialog->ScreenToClient(rc);
		m_control[i].originalRectangle= rc;
	}
	
	CRect sg;
	m_dialog->GetClientRect(sg);
	sg.left= sg.right - m_sizeGripper._width;
	sg.top= sg.bottom - m_sizeGripper._width;
	m_sizeGripper.Create(m_dialog, sg);

	i= AddControl(&m_sizeGripper, 1, 1, 0, 0);
	m_control[i].originalRectangle= sg;

	CPersistence::GetDialogRectangle(m_name, rcDialog);
	m_dialog->MoveWindow(rcDialog);
	if (centerWindow)
		m_dialog->CenterWindow();
}

void CLayout::OnDestroy()
{
	CRect rc;
	m_dialog->GetWindowRect(rc);
	CPersistence::SetDialogRectangle(m_name, rc);
}

void CLayout::OnSize()
{
	CRect rc;
	m_dialog->GetWindowRect(rc);
	CSize newDialogSize= rc.Size();

	CSize diff= newDialogSize - m_originalDialogSize;

	// The DeferWindowPos-stuff prevents the controls
	// from overwriting each other.

	HDWP hdwp= BeginDeferWindowPos(m_control.GetSize());

	for (int i=0; i < m_control.GetSize(); i++)
	{
		CRect rc= m_control[i].originalRectangle;

		CSize move(int(diff.cx * m_control[i].movex), int(diff.cy * m_control[i].movey));
		CRect stretch(0, 0, int(diff.cx * m_control[i].stretchx), int(diff.cy * m_control[i].stretchy));
		
		rc+= move;
		rc+= stretch;

		hdwp= DeferWindowPos(hdwp, *m_control[i].control, NULL, rc.left, rc.top, rc.Width(), rc.Height(), 
			SWP_NOOWNERZORDER|SWP_NOZORDER);
	}

	EndDeferWindowPos(hdwp);
}

void CLayout::OnGetMinMaxInfo(MINMAXINFO *mmi)
{
	mmi->ptMinTrackSize.x= m_originalDialogSize.cx;
	mmi->ptMinTrackSize.y= m_originalDialogSize.cy;
}


/////////////////////////////////////////////////////////////////////////////

const int CLayout::CSizeGripper::_width = 14;

CLayout::CSizeGripper::CSizeGripper()
{
}

void CLayout::CSizeGripper::Create(CWnd *parent, CRect rc)
{
	VERIFY(CWnd::Create(
		AfxRegisterWndClass(
			0, 
			AfxGetApp()->LoadStandardCursor(IDC_ARROW), 
			(HBRUSH)(COLOR_BTNFACE + 1), 
			0
		), 
		_T(""), 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, 
		rc, 
		parent, 
		IDC_SIZEGRIPPER
	));
}

BEGIN_MESSAGE_MAP(CLayout::CSizeGripper, CWnd)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

void CLayout::CSizeGripper::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(rc);

	ASSERT(rc.Width() == _width);
	ASSERT(rc.Height() == _width);

	CPoint start;
	CPoint end;

	start.x= 1; 
	start.y= _width;
	end.x= _width; 
	end.y= 1;

	DrawShadowLine(&dc, start, end);

	start.x+= 4;
	end.y+= 4;

	DrawShadowLine(&dc, start, end);

	start.x+= 4;
	end.y+= 4;

	DrawShadowLine(&dc, start, end);

	// Do not call CWnd::OnPaint() for painting messages
}

void CLayout::CSizeGripper::DrawShadowLine(CDC *pdc, CPoint start, CPoint end)
{
	{
		CPen lightPen(PS_SOLID, 1, GetSysColor(COLOR_3DHIGHLIGHT));
		CSelectObject sopen(pdc, &lightPen);

		pdc->MoveTo(start);
		pdc->LineTo(end);
	}

	start.x++;
	end.y++;

	{
		CPen darkPen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
		CSelectObject sopen(pdc, &darkPen);	

		pdc->MoveTo(start);
		pdc->LineTo(end);

		start.x++;
		end.y++;

		pdc->MoveTo(start);
		pdc->LineTo(end);
	}
}

UINT CLayout::CSizeGripper::OnNcHitTest(CPoint point)
{
	ScreenToClient(&point);

	if (point.x + point.y >= _width)
		return HTBOTTOMRIGHT;
	else
		return 0;
}

// $Log: layout.cpp,v $
// Revision 1.5  2004/11/05 16:53:07  assarbad
// Added Date and History tag where appropriate.
//
