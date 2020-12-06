#pragma once
class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString ConvertToRelativePath(const CString& absPath);
	static CString ConverToRelativePathOnly(const CString& absPath);
	static CString DeleteFileName(const CString& relativePath);
};

