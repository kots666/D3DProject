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
