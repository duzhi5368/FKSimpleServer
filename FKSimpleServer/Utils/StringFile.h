#pragma once
//------------------------------------------------------------------------
class CStringFile
{
public:
	CStringFile(const char * pszTextFile);
	CStringFile();
	~CStringFile();
public:
	void	Destroy();
	void	MakeDeflate();
	bool	LoadFile(const char * pszTextFile);
	bool	SetData(char * pData, int iSize);
	char*	operator[](int line);
	int		GetLineCount();
private:
	bool	BuildLines();
	int		ProcData();
private:
	char * m_pData;
	int	   m_iDataSize;
	int	   m_iLineCount;
	char** m_pLines;
	bool   m_bBuildInData;
};