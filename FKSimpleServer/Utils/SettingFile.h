#pragma once
//------------------------------------------------------------------------
#include "StringFile.h"
//------------------------------------------------------------------------
class CSettingFile
{
public:
	bool			Open(const char * pszFile);
	void			Close();

	const char*		GetString(const char * pszSection, const char * pszItemName, const char * pszDefValue = nullptr);
	int				GetInteger(const char * pszSection, const char * pszItemName, int DefValue = 0);
private:
	int				FindSectionLine(const char * pszSection);
	char*			GetSettingString(const char * pszSection, const char * pszItemName);
	int				FindSettingLine(const char * pszSection, const char * pszItemName);
private:
	CStringFile m_sfSetting;
};