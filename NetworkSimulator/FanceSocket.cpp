// AkingSocket.cpp : implementation file
//

#include "stdafx.h"
#include "TCPIP.h"
#include "TCPIPDlg.h"
#include "FanceSocket.h"
#include "GlobalVar.h"
#define WM_PROCESS_SHOW_RECEIVE 2001
/////////////////////////////////////////////////////////////////////////////
// CAkingSocket

FanceSocket::FanceSocket()
{
}

FanceSocket::~FanceSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CAkingSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CAkingSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CAkingSocket member functions

void FanceSocket::OnAccept(int nErrorCode) 
{
	CString		strAddress;
	UINT		nPort = 0;
	CTCPIPDlg*	dlg=(CTCPIPDlg*)AfxGetApp()->GetMainWnd();
	if (dlg == nullptr)
		return;
	if (GlobalVar::s_strServiceTypeName == TCP_SERVER_NAME)
	{
		Accept(dlg->m_DealTcpSocket);
		dlg->m_DealTcpSocket.GetPeerName(strAddress, nPort);
		dlg->m_strListViewMsg.Format("%02d:%02d:%02d  IP:%s �˿�:%d�Ѻ�TCP�������������ӣ�\n",
			dlg->m_SystemTime.wHour, dlg->m_SystemTime.wMinute, dlg->m_SystemTime.wSecond, strAddress, nPort);
		dlg->m_ListViewReceives.InsertString(0,dlg->m_strListViewMsg);
	}
	else if (GlobalVar::s_strServiceTypeName == TCP_CLIENT_NAME)
	{
		Accept(dlg->m_ClientSocket);
	}
	dlg->UpdateData(FALSE);

	CAsyncSocket::OnAccept(nErrorCode);
}

void FanceSocket::OnReceive(int nErrorCode) 
{
	CTCPIPDlg* dlg=(CTCPIPDlg*)AfxGetApp()->GetMainWnd();
	if (dlg == nullptr)
		return;
	memset(dlg->m_pReceiveBuffer,0,4096);
	dlg->m_nReceiveBufferLen = Receive(dlg->m_pReceiveBuffer,4096);
	PostMessage(dlg->m_hWnd,WM_PROCESS_SHOW_RECEIVE,0,0);

	CAsyncSocket::OnReceive(nErrorCode);
}

void FanceSocket::OnClose(int nErrorCode) 
{
	CTCPIPDlg* dlg=(CTCPIPDlg*)AfxGetApp()->GetMainWnd();
	if (dlg == nullptr)
		return;
	if(GlobalVar::s_strServiceTypeName == TCP_SERVER_NAME)
	{
		CString	strAddress;
		UINT	nPort;
		GetSystemTime(&dlg->m_SystemTime);
		dlg->m_DealTcpSocket.GetPeerName(strAddress,nPort);
		dlg->m_strListViewMsg.Format("%02d:%02d:%02d  IP:%s �˿�:%d�Ѻ�TCP�������Ͽ����ӣ�\n",
			dlg->m_SystemTime.wHour,dlg->m_SystemTime.wMinute,dlg->m_SystemTime.wSecond,strAddress,nPort);
		dlg->m_ListViewReceives.InsertString(0,dlg->m_strListViewMsg);
	}
	if(GlobalVar::s_strServiceTypeName == TCP_CLIENT_NAME)
	{
		dlg->m_ClientSocket.Close();
		dlg->m_BtnConnectAndDisconnect.SetWindowText("����");
		GetSystemTime(&dlg->m_SystemTime);
		dlg->m_strListViewMsg.Format("%02d:%02d:%02d  �Ѻ�TCP�������Ͽ����ӣ�\n",
			dlg->m_SystemTime.wHour,dlg->m_SystemTime.wMinute,dlg->m_SystemTime.wSecond);
		dlg->m_ListViewReceives.InsertString(0,dlg->m_strListViewMsg);
		GlobalVar::s_bIsTcpClientStart = false;
	}

	CAsyncSocket::OnClose(nErrorCode);
}



void FanceSocket::OnConnect(int nErrorCode)
{
	CTCPIPDlg* dlg=(CTCPIPDlg*)AfxGetApp()->GetMainWnd();
	if (dlg == nullptr)
		return;
	LPVOID lpMsgBuf = nullptr;
	if(!nErrorCode)
	{
		dlg->m_strListViewMsg.Format("%02d:%02d:%02d  ���� IP:%s �˿�:%d �ɹ� ��\n",
			dlg->m_SystemTime.wHour,dlg->m_SystemTime.wMinute,dlg->m_SystemTime.wSecond,dlg->m_strIP,dlg->m_nRemotePort);
		dlg->m_ListViewReceives.InsertString(0,dlg->m_strListViewMsg);

		GlobalVar::s_bIsTcpClientStart = true;
		dlg->m_BtnConnectAndDisconnect.SetWindowText("�Ͽ�");
	}
	else
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL,nErrorCode,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
							(LPTSTR)&lpMsgBuf,
							0,
							NULL);
		CString strMsg;
		strMsg.Format("%02d:%02d:%02d ����ʧ��!\n\n ����ID:%d. ��������:%s",
			dlg->m_SystemTime.wHour, dlg->m_SystemTime.wMinute, dlg->m_SystemTime.wSecond, nErrorCode, lpMsgBuf);
		dlg->m_ListViewReceives.InsertString(0,strMsg);

		dlg->m_ClientSocket.Close();
	}
	CAsyncSocket::OnConnect(nErrorCode);
}
