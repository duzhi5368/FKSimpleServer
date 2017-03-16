#pragma once
//------------------------------------------------------------------------
#include "Form.h"
#include "ProgressBar.h"
#include "Label.h"
//------------------------------------------------------------------------
class CProgressForm : public CEGForm
{
public:
	CProgressForm(void);
	virtual ~CProgressForm(void);
	BOOL	Create(HWND hParents, const TCHAR * pszTitle, const TCHAR * pszLabel, BOOL bSmooth);
	BOOL	Show(const TCHAR * pszTitle, int x, int y, int imin = 0, int imax = 100);
	VOID	StepIt(const TCHAR * pszLabelText = nullptr);
	VOID	SetRange(int imin, int imax);
	VOID	SetPos(int pos);
	VOID	SetStep(int nStep);
	BOOL	OnClose(){ Hide(); return FALSE; }
protected:
	CEGProgressBar	m_pbMain;
	CEGLabel		m_lbText;
};
//------------------------------------------------------------------------