// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <map>
#include <tchar.h>
#include "tlhelp32.h"



void CloseNotepad()
{
	HWND hNoteWnd = FindWindow(NULL, L"无标题 - 记事本");
	if (!hNoteWnd)
	{
		return;
	}
	DWORD dwNotePid = 0;
	GetWindowThreadProcessId(hNoteWnd, &dwNotePid);
	if (!dwNotePid)
	{
		return;
	}
	HANDLE hNoteHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwNotePid);
	if (!hNoteHandle)
	{
		return;
	}
	TerminateProcess(hNoteHandle, 0);
	CloseHandle(hNoteHandle);
	//CloseHandle(hNoteWnd);
}

void ShowProcess()
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32W);
	BOOL bRet = Process32FirstW(hSnap, &pe32);
	while (bRet)
	{
		
		std::wcout << L"ID:" << pe32.th32ProcessID << L" 文件:" << pe32.szExeFile << std::endl;
		bRet = Process32NextW(hSnap, &pe32);
	}
}

//权限 securitybaseapi.h
//https://docs.microsoft.com/en-us/windows/win32/api/securitybaseapi/
void DebugPrivilege()
{
	HANDLE hToken = NULL;
	BOOL bRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);
	if (bRet)
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		BOOL bRet = LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		if (!bRet)
		{
			int nErr = GetLastError();
			CloseHandle(hToken);
			return;
		}
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		bRet = AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		if (bRet != ERROR_SUCCESS)
		{
			int nErr = GetLastError();
			CloseHandle(hToken);
			return;
		}
		CloseHandle(hToken);
	}
}

void Stop()
{
	int nPid;
	std::wcin >> nPid;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, nPid);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		int nErr = GetLastError();
		return;
	}
	THREADENTRY32 Te32 = { 0 };
	Te32.dwSize = sizeof(THREADENTRY32);
	BOOL bRet = Thread32First(hSnap, &Te32);
	while (bRet)
	{
		if (Te32.th32OwnerProcessID == nPid)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Te32.th32ThreadID);
			SuspendThread(hThread);
			CloseHandle(hThread);
		}
		bRet = Thread32Next(hSnap, &Te32);
	}
}

int main()
{
	std::wcout.imbue(std::locale("chs"));
	DebugPrivilege();
	ShowProcess();
	getchar();
	return 0;
}
