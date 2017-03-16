/**
*	created:		2013-5-28   20:19
*	filename: 		ListView
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../EasyGuiDepend.h"
#include "../Include/listview.h"
//------------------------------------------------------------------------
CEGListView::CEGListView(void)
{
	InitCommonControls();
}
//------------------------------------------------------------------------
CEGListView::~CEGListView(void)
{
}
//------------------------------------------------------------------------
BOOL CEGListView::Create( HWND hParents, int x, int y, int width, int height )
{
	m_hWnd = CreateWindowEx( WS_EX_LEFT|WS_EX_LTRREADING|
		WS_EX_RIGHTSCROLLBAR|WS_EX_NOPARENTNOTIFY|WS_EX_CLIENTEDGE|LVS_EX_HEADERDRAGDROP|
		LVS_EX_FULLROWSELECT, 
		L"SysListView32", NULL, WS_VISIBLE | WS_CHILDWINDOW|WS_BORDER|
		WS_HSCROLL|WS_VSCROLL|LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS,
		x, y, width, height, hParents, NULL, GetModuleHandle(NULL), 0 );
	if( m_hWnd == NULL ) 
		return FALSE;
	if( !HandleWnd())
	{
		DestroyWindow( m_hWnd );
		return FALSE;
	}

	return TRUE;
}
//------------------------------------------------------------------------
int	CEGListView::InsertColumn(int nWidth, TCHAR * pText, int index)
{
	LV_COLUMN	lvc;
	lvc.pszText = pText;
	lvc.cchTextMax = (int)_tcslen(pText);
	lvc.cx = nWidth;
	lvc.fmt = LVCFMT_LEFT;
	lvc.mask = LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	return ListView_InsertColumn( m_hWnd, index, &lvc);
}
//------------------------------------------------------------------------
int CEGListView::AddColumn(int nWidth, TCHAR * pszText)
{
	return InsertColumn(nWidth, pszText, GetColumnCount());
}
//------------------------------------------------------------------------
BOOL CEGListView::SetSubItemString(int index, int item, TCHAR * pszItemText)
{
	LV_ITEM		lvi;
	lvi.pszText = pszItemText;
	lvi.cchTextMax = (int)_tcslen(pszItemText);
	lvi.mask = LVIF_TEXT;
	lvi.iItem = index;
	lvi.iSubItem = item;
	return ListView_SetItem( m_hWnd, &lvi );
}
//------------------------------------------------------------------------
BOOL CEGListView::SetSubItemInt( int index, int item, int iValue )
{
	TCHAR	szValue[200];
	wsprintf(szValue, L"%d", iValue);
	return SetSubItemString( index, item, szValue );
}
//------------------------------------------------------------------------
int CEGListView::AddItem(TCHAR * pszText)
{
	LV_ITEM		lvi;
	lvi.pszText = pszText;
	lvi.cchTextMax = (int)_tcslen(pszText);
	lvi.mask = LVIF_TEXT;
	lvi.iItem = GetItemCount();
	lvi.iSubItem = 0;
	return ListView_InsertItem( m_hWnd, &lvi );
}
//------------------------------------------------------------------------
int CEGListView::AddItemParamed(TCHAR * pszText, UINT	Param)
{
	LV_ITEM		lvi;
	lvi.pszText = pszText;
	lvi.cchTextMax = (int)_tcslen(pszText);
	lvi.mask = LVIF_TEXT|LVIF_PARAM;
	lvi.iItem = GetItemCount();
	lvi.iSubItem = 0;
	lvi.lParam = Param;
	return ListView_InsertItem( m_hWnd, &lvi );
}
//------------------------------------------------------------------------