// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>

int main()
{
	std::vector<std::wstring> vecDriver;
	TCHAR szDriverString[MAXBYTE] = { 0 };
	TCHAR * pTmp = nullptr;
	DWORD dwError = 0;
	DWORD dwSize = GetLogicalDriveStrings(MAXBYTE, szDriverString);

	if ( !dwSize)
	{
		dwError = GetLastError();
	}
	//if (dwSize > dwError)
	//{
	//	pTmp = new TCHAR[dwSize];
	//}
	pTmp = szDriverString;
	while (*pTmp)
	{
		vecDriver.push_back(pTmp);
		pTmp += 4;
	}
	return 0;
}
