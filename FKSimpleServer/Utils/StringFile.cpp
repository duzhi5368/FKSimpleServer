#include "StringFile.h"
#include "../Base/BaseDepend.h"
//------------------------------------------------------------------------
CStringFile::CStringFile(const char * pszTextFile)
{
	m_pData = nullptr;
	m_iDataSize = 0;
	m_pLines = nullptr;
	m_iLineCount = 0;
	m_bBuildInData = false;
	LoadFile(pszTextFile);
}
//------------------------------------------------------------------------
CStringFile::CStringFile()
{
	m_pData = nullptr;
	m_iDataSize = 0;
	m_pLines = nullptr;
	m_iLineCount = 0;
	m_bBuildInData = false;
}
//------------------------------------------------------------------------
CStringFile::~CStringFile()
{
	Destroy();
}
//------------------------------------------------------------------------
void CStringFile::Destroy()
{
	if (m_pLines != nullptr)
	{
		delete[]m_pLines;
		m_pLines = nullptr;
	}
	m_iDataSize = 0;
	if (m_pData != nullptr)
	{
		if (m_bBuildInData)
			delete[]m_pData;
		m_pData = nullptr;
	}
	m_iLineCount = 0;
}
//------------------------------------------------------------------------
void CStringFile::MakeDeflate()
{
	int i = 0;
	char * p;
	char * p1;
	bool	bInString = false;
	for (i = 0; i < GetLineCount(); i++)
	{
		p = (*this)[i];
		p1 = p;
		while (*p != '\0')
		{
			if (*p == '\"')bInString = !bInString;
			if ((!bInString) && (*p == ' ' || *p == '	'))
			{
				p++;
				continue;
			}
			*p1++ = *p++;
		}
		*p1 = 0;
	}
}
//------------------------------------------------------------------------
bool CStringFile::LoadFile(const char * pszTextFile)
{
	FILE* fp = fopen(pszTextFile, "rb");
	if (fp == nullptr)
		return false;
	fseek(fp, 0, SEEK_END);
	m_iDataSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	m_pData = new char[m_iDataSize + 2];
	if (m_pData == NULL)
		return false;
	m_bBuildInData = TRUE;
	fread(m_pData, m_iDataSize, 1, fp);
	fclose(fp);
	m_pData[m_iDataSize] = 0;
	m_pData[m_iDataSize + 1] = 0;
	m_iLineCount = ProcData();
	return BuildLines();
}
//------------------------------------------------------------------------
bool CStringFile::SetData(char * pData, int iSize)
{
	m_bBuildInData = false;
	m_iDataSize = iSize;
	m_pData = pData;
	m_iLineCount = ProcData();
	return BuildLines();
}
//------------------------------------------------------------------------
char* CStringFile::operator[](int line)
{
	if (line < 0 || line >= m_iLineCount)
		return nullptr;
	return m_pLines[line];
}
//------------------------------------------------------------------------
int	CStringFile::GetLineCount()
{ 
	return m_iLineCount;
}
//------------------------------------------------------------------------
bool CStringFile::BuildLines()
{
	if (m_iLineCount == 0)
		return false;
	char * p = m_pData;
	m_pLines = new char*[m_iLineCount];
	int len = 0;
	int ptr = 0;
	for (int i = 0; i < m_iLineCount; i++)
	{
		len = (int)strlen(p);
		if (len > 0)
			m_pLines[ptr++] = p;
		else
			break;
		p = p + len + 1;
	}
	return true;
}
//------------------------------------------------------------------------
int CStringFile::ProcData()
{
	int i = 0;
	char * p = nullptr;
	int linecount = 0;
	int rptr = 0;
	bool	binstring = false;
	bool	newlinestart = false;
	for (i = 0; i < m_iDataSize; i++)
	{
		p = m_pData + i;
		switch (*p)
		{
		case	'\n':
		case	'\r':
		{
			if (newlinestart)
			{
				*(m_pData + rptr++) = 0;
				newlinestart = false;
				linecount++;
			}
		}
		break;
		default:
		{
			*(m_pData + rptr++) = *p;
			if (!newlinestart)newlinestart = true;
		}
		break;
		}
	}
	if (newlinestart)
		linecount++;
	assert(rptr <= m_iDataSize);
	m_pData[rptr++] = 0;
	m_pData[rptr++] = 0;
	m_iDataSize = rptr;
	return linecount;
}
//------------------------------------------------------------------------