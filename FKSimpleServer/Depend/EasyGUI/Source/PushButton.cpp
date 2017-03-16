/**
*	created:		2013-5-28   20:27
*	filename: 		PushButton
*	author:			FreeKnight
*	Copyright (C): 	
*	purpose:		
*/
//------------------------------------------------------------------------
#include "../EasyGuiDepend.h"
#include "../Include/pushbutton.h"
//------------------------------------------------------------------------
#ifdef _DEBUG
static int NewCount=0;		// ����
#endif
//------------------------------------------------------------------------
CEGPushButton::CEGPushButton(void)
{
}
//------------------------------------------------------------------------
CEGPushButton::~CEGPushButton(void)
{
}
//------------------------------------------------------------------------
BOOL CEGPushButton::Create(HWND hParents, TCHAR * pText, DWORD dwStyle, int x, int y, int width, int height, DWORD dwExtStyle)
{
	m_hWnd = CreateWindow(_T("BUTTON"), pText, WS_CHILDWINDOW|WS_VISIBLE|BS_PUSHBUTTON|BS_TEXT|dwStyle, x, y, 
		width, height, hParents, NULL, GetModuleHandle( NULL ), 0 );
	if( m_hWnd == NULL ) 
		return FALSE;

#ifdef _DEBUG
	NewCount++;
	TCHAR buff[100];
	wsprintf(buff,L"New CEGPushButton(%p,%p) %d\r\n",m_hWnd,this,NewCount);
	OutputDebugString(buff);
#endif

	if( !HandleWnd() )
	{
		DestroyWindow( m_hWnd );
		return FALSE;
	}
	return TRUE;
}
//------------------------------------------------------------------------