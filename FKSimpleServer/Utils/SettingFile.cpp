#include "SettingFile.h"
#include "../Base/BaseDepend.h"
//------------------------------------------------------------------------
bool CSettingFile::Open(const char * pszFile)
{
	if (!m_sfSetting.LoadFile((char*)pszFile))
		return false;
	m_sfSetting.MakeDeflate();
	return true;
}
//------------------------------------------------------------------------
void CSettingFile::Close()
{
	m_sfSetting.Destroy();
}
//------------------------------------------------------------------------
const char* CSettingFile::GetString(const char * pszSection, const char * pszItemName, const char * pszDefValue )
{
	char * p = GetSettingString(pszSection, pszItemName);
	if (p == nullptr)
		return pszDefValue;
	return p;
}
//------------------------------------------------------------------------
int	CSettingFile::GetInteger(const char * pszSection, const char * pszItemName, int DefValue )
{
	char* p = GetSettingString(pszSection, pszItemName);
	if (p == nullptr)
		return DefValue;
	return atoi(p);
}
//------------------------------------------------------------------------
int	CSettingFile::FindSectionLine(const char * pszSection)
{
	if (pszSection == nullptr)
		return 0;
	int linecount = m_sfSetting.GetLineCount();
	int sectionlength = (int)strlen(pszSection);
	for (int i = 0; i < linecount; i++)
	{
		char * p = m_sfSetting[i];

		if (*p == '[' && *(p + sectionlength + 1) == ']')
		{
			if (strnicmp(p + 1, pszSection, sectionlength) == 0)
				return (i + 1);
		}
	}
	return -1;
}
//------------------------------------------------------------------------
char* CSettingFile::GetSettingString(const char * pszSection, const char * pszItemName)
{
	int startindex = 0;
	if (pszSection != nullptr)
	{
		startindex = FindSectionLine(pszSection);
		if (startindex == -1)
			return nullptr;
	}

	int itemnamelength = (int)strlen(pszItemName);
	if (itemnamelength == 0)
		return nullptr;

	int linecount = m_sfSetting.GetLineCount();
	for (int i = startindex; i < linecount; i++)
	{
		char * p = m_sfSetting[i];
		//	���������һ��section�� ���ش���
		if (*p == '[')
			return nullptr;
		//	�����ItemName=�����ľ�ʽ�������һ������
		if (*(p + itemnamelength) == '=')
		{
			if (strnicmp(p, pszItemName, itemnamelength) == 0)
			{
				if (*(p + itemnamelength + 1) == '\"')
				{
					char * pret = (p + itemnamelength + 2);
					int length = (int)strlen(pret);
					if (*(pret + length - 1) == '\"')
						*(pret + length - 1) = 0;
					return pret;
				}
				return (p + itemnamelength + 1);
			}
		}
	}
	return nullptr;
}
//------------------------------------------------------------------------
int	CSettingFile::FindSettingLine(const char * pszSection, const char * pszItemName)
{
	int startindex = 0;
	if (pszSection != nullptr)
	{
		startindex = FindSectionLine(pszSection);
		if (startindex == -1)
			return -1;
	}

	int itemnamelength = (int)strlen(pszItemName);
	if (itemnamelength == 0)
		return -1;

	int linecount = m_sfSetting.GetLineCount();
	for (int i = startindex; i < linecount; i++)
	{
		char * p = m_sfSetting[i];
		//	���������һ��section�� ���ش���
		if (*p == '[')
			return -1;
		//	�����ItemName=�����ľ�ʽ�������һ������
		if (*(p + itemnamelength) == '=')
		{
			if (strnicmp(p, pszItemName, itemnamelength) == 0)
				return i;
		}
	}
	return -1;
}
//------------------------------------------------------------------------