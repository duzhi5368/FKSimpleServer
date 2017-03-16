#pragma once
//------------------------------------------------------------------------
// 获取一个文件大小
inline int	_GetFileSize(FILE * fp)
{
	if (fp == NULL)return 0;
	int oldfp = ftell(fp);
	fseek(fp, 0, SEEK_END);
	int ret = ftell(fp);
	fseek(fp, oldfp, SEEK_SET);
	return ret;
}
//------------------------------------------------------------------------
// 打开一个文件
inline BYTE * LoadFile(const TCHAR * pszFileName)
{
	FILE	*	fp = _wfopen(pszFileName, L"rb");
	if (fp == NULL)return NULL;
	BYTE	*	pBytes = NULL;
	int size = _GetFileSize(fp);
	if (size == 0)return NULL;
	pBytes = new BYTE[size + 16];
	fread(pBytes, size, 1, fp);
	fclose(fp);
	pBytes[size] = 0;
	return pBytes;
}
//------------------------------------------------------------------------
// 读取一个文件
inline BYTE * LoadFile(const TCHAR * pszFileName, int & size)
{
	FILE	*	fp = _wfopen(pszFileName, L"rb");
	if (fp == NULL)return NULL;
	BYTE	*	pBytes = NULL;
	size = _GetFileSize(fp);
	if (size == 0)return NULL;
	pBytes = new BYTE[size + 16];
	fread(pBytes, size, 1, fp);
	fclose(fp);
	pBytes[size] = 0;
	return pBytes;
}
//------------------------------------------------------------------------
// 检查一个路径是否是文件夹
inline BOOL		PathIsFolder(const TCHAR * pszPath)
{
	WIN32_FIND_DATA wfd;
	ZeroMemory(&wfd, sizeof(wfd));

	HANDLE	hFind = FindFirstFile(pszPath, &wfd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return ((wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
	}
	return FALSE;
}
//------------------------------------------------------------------------
// 检查一个文件是否存在
inline BOOL		FileExist(const TCHAR * pszPath)
{
	WIN32_FIND_DATA wfd;
	ZeroMemory(&wfd, sizeof(wfd));

	HANDLE	hFind = FindFirstFile(pszPath, &wfd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return TRUE;
	}
	return FALSE;
}
//------------------------------------------------------------------------