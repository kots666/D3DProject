#pragma once
class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString ConvertToRelativePath(const CString& absPath);
};

