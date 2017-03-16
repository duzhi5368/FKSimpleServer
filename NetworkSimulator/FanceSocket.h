#if !defined(AFX_AKINGSOCKET_H__35E1C928_7A72_4EE2_AE47_1BCC69EAE67A__INCLUDED_)
#define AFX_AKINGSOCKET_H__35E1C928_7A72_4EE2_AE47_1BCC69EAE67A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AkingSocket.h : header file
//
#include <WINSOCK2.H>
#include <afxsock.h>



/////////////////////////////////////////////////////////////////////////////
// CAkingSocket command target

class FanceSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	FanceSocket();
	virtual ~FanceSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAkingSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CAkingSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
public:
	virtual void OnConnect(int nErrorCode);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AKINGSOCKET_H__35E1C928_7A72_4EE2_AE47_1BCC69EAE67A__INCLUDED_)
