#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertToRelativePath(const CString & absPath)
{
	TCHAR relativePath[MAX_PATH] = L"";
	TCHAR curDir[MAX_PATH] = L"";
	
	GetCurrentDirectory(MAX_PATH, curDir);
	PathRelativePathTo(relativePath, curDir, FILE_ATTRIBUTE_DIRECTORY, absPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);
	
	return CString(relativePath);
}

CString CFileInfo::ConverToRelativePathOnly(const CString & absPath)
{
	TCHAR relativePath[MAX_PATH] = L"";
	TCHAR curDir[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, curDir);
	PathRelativePathTo(relativePath, curDir, FILE_ATTRIBUTE_DIRECTORY, absPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);

	int end = 0;
	int size = lstrlen(relativePath);

	for (int i = 0; i < size; ++i)
	{
		if (relativePath[i] == '\\')
		{
			end = i;
		}
	}

	TCHAR returnPath[MAX_PATH] = L"";
	CString tmp = relativePath;

	memcpy(returnPath, relativePath, sizeof(TCHAR) * (end + 1));

	return CString(returnPath);
}

CString CFileInfo::DeleteFileName(const CString & inputPath)
{
	TCHAR relativePath[MAX_PATH] = L"";

	lstrcpy(relativePath, inputPath);

	int end = 0;
	int size = lstrlen(relativePath);

	for (int i = 0; i < size; ++i)
	{
		if (relativePath[i] == '/')
		{
			end = i;
		}
	}

	TCHAR returnPath[MAX_PATH] = L"";
	CString tmp = relativePath;

	memcpy(returnPath, relativePath, sizeof(TCHAR) * (end + 1));

	return CString(returnPath);
}
