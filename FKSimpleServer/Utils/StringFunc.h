#pragma once
//------------------------------------------------------------------------
#include "CharSet.h"
#define _GLIBCXX_USE_WSTRING
#include <string>
using std::wstring;
//------------------------------------------------------------------------
inline TCHAR * o_strncpy(TCHAR * pdest, const TCHAR * psrc, int length)
{
	_tcsncpy(pdest, psrc, length);
	pdest[length] = 0;
	return pdest;
}
//------------------------------------------------------------------------
inline char * Trim(char * pString)
{
	int len = (int)strlen(pString);
	char * p = pString;

	while (*p == ' ' || *p == '	')p++, len--;
	while (*(p + len - 1) == ' ' || *(p + len - 1) == '	')len--, *(p + len) = 0;
	return p;
}
//------------------------------------------------------------------------
inline int SearchParam(char * buffer, char ** Params, int maxparam, int spliter)
{
	char * pbuffer = Trim(buffer);
	int len = (int)strlen(buffer);
	if (len == 0)return 0;
	char * p = strchr(pbuffer, spliter);
	int count = 0;
	Params[0] = pbuffer;
	while (p)
	{
		*p++ = 0;
		Params[count++] = Trim(Params[count]);
		if (count >= maxparam)return count;
		Params[count] = p;
		p = strchr(p, spliter);
	}
	Params[count++] = Trim(Params[count]);
	return count;
}
//------------------------------------------------------------------------
static char * TrimEx(char * pszString, CCharSet & whiteset = CharSetWhite)
{
	while (*pszString && whiteset.charIn(*pszString))
		pszString++;
	if (pszString[0] == 0)return pszString;
	char * pWhite = NULL;
	int ptr = 0;
	while (*(pszString + ptr))
	{
		if (whiteset.charIn(*(pszString + ptr)))
		{
			if (pWhite == NULL)pWhite = pszString + ptr;
		}
		else
			pWhite = NULL;
		ptr++;
	}
	if (pWhite)*pWhite = 0;
	return pszString;
}
//------------------------------------------------------------------------
inline int ExtractStrings(char * pszString, CCharSet & whiteset, CCharSet & delimset, 
	char ** Params, int iArraySize, BOOL bKeepEmpty = TRUE, BOOL bKeepString = TRUE)
{
	int ptr = 0;
	int nStringCount = 0;
	char * pStart = pszString;
	char	cTemp = 0;
	BOOL	bKeep = FALSE;

	while (*(pszString + ptr))
	{
		if (bKeepString && *(pszString + ptr) == '\"')
			bKeep = !bKeep;

		if (!bKeep && delimset.charIn(*(pszString + ptr)))
		{
			*(pszString + ptr) = 0;
			pStart = TrimEx(pStart, whiteset);
			if (pStart[0] != 0 || bKeepEmpty)
			{
				Params[nStringCount++] = pStart;
				if (nStringCount >= iArraySize)
					return nStringCount;
			}
			pStart = pszString + ptr + 1;
		}
		ptr++;
	}
	if (*pStart != 0 || nStringCount > 0)
	{
		pStart = TrimEx(pStart, whiteset);
		if (pStart[0] != 0 || bKeepEmpty)
		{
			Params[nStringCount++] = pStart;
		}
	}
	return nStringCount;
}
//------------------------------------------------------------------------
inline std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}
//------------------------------------------------------------------------
inline std::string WStringToMBytes(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen;
	// wide char to multi char
	iTextLen = ::WideCharToMultiByte(CP_ACP, 0, lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_ACP, 0, lpwcszWString, 0, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}
//------------------------------------------------------------------------