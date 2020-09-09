// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <map>

// 创建的目录
#define AUTORUN L"autorun.inf"

//创建的无法删除的目录
#define IMMUNITY L"\\Immunity...\\"

void GetDriver(std::map<int, std::wstring>& mapDriver)
{
	TCHAR szDriverString[MAXBYTE] = { 0 };
	TCHAR * pTmp = nullptr;
	DWORD dwError = 0;
	DWORD dwSize = GetLogicalDriveStrings(MAXBYTE, szDriverString);

	if (!dwSize)
	{
		dwError = GetLastError();
	}
	//if (dwSize > dwError)
	//{
	//	pTmp = new TCHAR[dwSize];
	//}
	pTmp = szDriverString;
	int nIndex = 0;
	while (*pTmp)
	{
		mapDriver[nIndex] = pTmp;
		pTmp += 4;
		nIndex++;
	}
}

void ShowDriver(std::map<int, std::wstring>& mapDriver)
{
	for (std::map<int, std::wstring>::iterator iter = mapDriver.begin(); 
		iter != mapDriver.end(); iter++)
	{
		std::wcout << L"ID:" << iter->first << " Name:" << iter->second << std::endl;
	}
}

void CreateImmunity(std::map<int, std::wstring>& mapDriver)
{
	std::wcout << L"创建免疫 ID:" << std::endl;
	std::wstring strNum;
	std::wcin >> strNum;
	std::string::size_type sz;
	long li_dec = std::stol(strNum, &sz);
	auto iter = mapDriver.find(li_dec);

	if (iter == mapDriver.end())
	{
		return;
	}

	std::wstring strPath;
	strPath = iter->second;
	strPath += AUTORUN;
	BOOL bRet = CreateDirectory(strPath.data(), NULL);
	if (!bRet)
	{
		//无法免疫该盘符！可能已经免疫，或者该磁盘为不可写状态！
		return;
	}
	//创建无法删除的用来进行免疫的文件夹
	strPath += IMMUNITY;
	bRet = CreateDirectory(strPath.data(), NULL);
	if (!bRet)
	{
		//无法免疫该盘符！可能已经免疫，或者该磁盘为不可写状态
	}
}

void RemoveImmunity(std::map<int, std::wstring>& mapDriver)
{
	std::wcout << L"删除免疫 ID:" << std::endl;
	std::wstring strNum;
	std::wcin >> strNum;
	std::string::size_type sz;
	long li_dec = std::stol(strNum, &sz);
	auto iter = mapDriver.find(li_dec);

	if (iter == mapDriver.end())
	{
		return;
	}

	std::wstring strPath;
	strPath = iter->second;
	strPath += AUTORUN;
	strPath += IMMUNITY;
	RemoveDirectory(strPath.data());
	strPath = iter->second;
	strPath += AUTORUN;
	RemoveDirectory(strPath.data());
}

int main()
{
	std::map<int, std::wstring> mapDriver;
	GetDriver(mapDriver);
	ShowDriver(mapDriver);
	CreateImmunity(mapDriver);
	ShowDriver(mapDriver);
	RemoveImmunity(mapDriver);

	return 0;
}
