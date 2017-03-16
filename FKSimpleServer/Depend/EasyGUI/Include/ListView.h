/**
*	created:		2013-5-28   20:18
*	filename: 		ListView
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#pragma once
//------------------------------------------------------------------------
#include "commctrl.h"
#include "basewnd.h"
//------------------------------------------------------------------------
class CEGListView:public CEGBaseWnd
{
public:
	CEGListView(void);
	virtual~CEGListView(void);
public:
	BOOL		Create( HWND hParents, int x, int y, int width, int height );
	int			InsertColumn(int nWidth, TCHAR * pText, int index);
	int			AddColumn(int nWidth, TCHAR * pszText);
	BOOL		SetSubItemString(int index, int item, TCHAR * pszItemText);
	BOOL		SetSubItemInt( int index, int item, int iValue );
	int			AddItem(TCHAR * pszText);
	int			AddItemParamed(TCHAR * pszText, UINT Param);
public:
	BOOL		DelItem( int index )
	{
		return ListView_DeleteItem( m_hWnd, index );
	}
	int			FindByParam( UINT	Param )
	{
		LVFINDINFO	lfi;
		lfi.flags = LVFI_PARAM;
		lfi.lParam = Param;
		return ListView_FindItem( m_hWnd, -1, &lfi );
	}
	VOID		Clear()
	{
		ListView_DeleteAllItems( m_hWnd );
	}
	int			GetColumnCount()
	{
		HWND	hHeader = ListView_GetHeader(m_hWnd);
		if( hHeader != NULL )
		{
			return Header_GetItemCount( hHeader);
		}
		return 0;
	}
	int			GetItemCount()
	{
		return ListView_GetItemCount(m_hWnd);
	}
	int			GetCurSel()
	{
		LVHITTESTINFO	lvhittest;
		GetCursorPos( &lvhittest.pt );
		ScreenToClient( m_hWnd, &lvhittest.pt );
		ListView_HitTest( m_hWnd, &lvhittest );
		return lvhittest.iItem;
	}
	UINT		GetItemParam( int index )
	{
		LV_ITEM		lvi;
		TCHAR	szText[200];
		lvi.pszText = szText;
		lvi.cchTextMax =200;
		lvi.mask = LVIF_TEXT|LVIF_PARAM;
		lvi.iItem = index;
		lvi.iSubItem = 0;
		if( ListView_GetItem( m_hWnd, &lvi ) )
			return (UINT)lvi.lParam;
		return 0;
	}
};
//------------------------------------------------------------------------