// typeview.h	- Declaration of CExtensionListControl and CTypeView
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
// Last modified: $Date: 2004/11/12 22:14:16 $

#pragma once

#include "dirstatdoc.h"	// SExtensionRecord

class CTypeView;

//
// CExtensionListControl.
//
class CExtensionListControl: public COwnerDrawnListControl
{
protected:
	// Columns
	enum
	{
		COL_EXTENSION,
		COL_COLOR,
		COL_DESCRIPTION,
		COL_BYTES,
		COL_BYTESPERCENT,
		COL_FILES
	};

	// CListItem. The items of the CExtensionListControl.
	class CListItem: public COwnerDrawnListItem
	{
	public:
		CListItem(CExtensionListControl *list, LPCTSTR extension, SExtensionRecord r);

		virtual bool DrawSubitem(int subitem, CDC *pdc, CRect rc, UINT state, int *width, int *focusLeft) const;
		virtual CString GetText(int subitem) const;

		CString GetExtension() const;
		int GetImage() const;
		int Compare(const CSortingListItem *other, int subitem) const;

	private:
		void DrawColor(CDC *pdc, CRect rc, UINT state, int *width) const;

		CString GetDescription() const;
		CString GetBytesPercent() const;

		double GetBytesFraction() const;

		CExtensionListControl *m_list;
		CString m_extension;
		SExtensionRecord m_record;
		mutable CString m_description;
		mutable int m_image;
	};

public:
	CExtensionListControl(CTypeView *typeView);
	virtual bool GetAscendingDefault(int column);
	void Initialize();
	void SetExtensionData(const CExtensionData *ed);
	void SetRootSize(LONGLONG totalBytes);
	LONGLONG GetRootSize();
	void SelectExtension(LPCTSTR ext);
	CString GetSelectedExtension();

protected:
	CListItem *GetListItem(int i);

	CTypeView *m_typeView;
	LONGLONG m_rootSize;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT mis);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


//
// CTypeView. The upper right view, which shows the extensions and their
// cushion colors.
//
class CTypeView: public CView
{
protected:
	CTypeView();
	DECLARE_DYNCREATE(CTypeView)

public:
	virtual ~CTypeView();
	CDirstatDoc* GetDocument() const;
	void SysColorChanged();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	bool IsShowTypes();
	void ShowTypes(bool show);

	void SetHighlightExtension(LPCTSTR ext);

protected:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnDraw(CDC* pDC);
	void SetSelection();

	bool m_showTypes;		// Whether this view shall be shown (F8 option)
	CExtensionListControl m_extensionListControl;	// The list control

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
public:
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif
};

#ifndef _DEBUG  // Debugversion in typeview.cpp
inline CDirstatDoc* CTypeView::GetDocument() const
   { return reinterpret_cast<CDirstatDoc*>(m_pDocument); }
#endif


// $Log: typeview.h,v $
// Revision 1.9  2004/11/12 22:14:16  bseifert
// Eliminated CLR_NONE. Minor corrections.
//
// Revision 1.8  2004/11/12 00:47:42  assarbad
// - Fixed the code for coloring of compressed/encrypted items. Now the coloring spans the full row!
//
// Revision 1.7  2004/11/08 00:46:26  assarbad
// - Added feature to distinguish compressed and encrypted files/folders by color as in the Windows 2000/XP explorer.
//   Same rules apply. (Green = encrypted / Blue = compressed)
//
// Revision 1.6  2004/11/05 16:53:08  assarbad
// Added Date and History tag where appropriate.
//
