// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <map>
#include <tchar.h>
#include "tlhelp32.h"


void InjectDll(DWORD dwPid, TCHAR * szDllName)
{
	if (dwPid == 0 || _tcsclen(szDllName) == 0)
	{
		return;
	}

#ifdef _UNICODE
	TCHAR pFunName[] = _T("LoadLibraryW");
#else
	TCHAR pFunName[] = _T("LoadLibraryA");
#endif // _UNICODE

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL)
	{
		return;
	}

	int nDllLen = (_tcslen(szDllName) + 1) * sizeof(TCHAR);
	PVOID pDllAddr = VirtualAllocEx(hProcess, NULL, nDllLen, MEM_COMMIT, PAGE_READWRITE);
	if (pDllAddr == NULL)
	{
		CloseHandle(hProcess);
		return;
	}

	SIZE_T dwWriteNum = 0;
	WriteProcessMemory(hProcess, pDllAddr, szDllName, nDllLen, &dwWriteNum);
	FARPROC pFunAddr = GetProcAddress(GetModuleHandle("ketnel32.dll"), pFunName);
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunAddr, pDllAddr, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
}

void UnInjectDll(DWORD dwPid, TCHAR * szDllName)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	MODULEENTRY32 Me32 = { 0 };
	Me32.dwSize = sizeof(MODULEENTRY32);
	BOOL bRet = Module32First(hSnap, &Me32);
	while (bRet)
	{
		if (strcmp(Me32.szExePath, szDllName) == 0)
		{
			break;
		}
		bRet = Module32Next(hSnap, &Me32);
	}
	CloseHandle(hSnap);
	TCHAR pFunName[] = "FreeLibrary";
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	FARPROC pFunAddr = GetProcAddress(GetModuleHandle("kernel32.dll"), pFunName);
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunAddr, Me32.hModule, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
}

int main()
{
	std::cout.imbue(std::locale("chs"));

	getchar();
	return 0;
}
