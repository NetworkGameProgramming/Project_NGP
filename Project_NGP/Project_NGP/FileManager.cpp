#include "stdafx.h"
#include "FileManager.h"

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

void FileManager::GetFileListFromFolder(bstr_t folderPath, vector<bstr_t>& vecStr)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(folderPath, &fd);

	if (INVALID_HANDLE_VALUE == hFind)
		return;

	FindNextFile(hFind, &fd);

	while (TRUE == FindNextFile(hFind, &fd))
	{
		vecStr.push_back(bstr_t(fd.cFileName));
	}

	FindClose(hFind);
}
