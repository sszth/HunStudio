// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <map>
#include <tchar.h>
#include "tlhelp32.h"

#define STRLEN 20
typedef struct _DATA
{
	DWORD dwLoadLibrary;
	DWORD dwGetProcAddress;
	DWORD dwGetModuleHandle;
	DWORD dwGetModuleFileName;
	char szUser32Dll[STRLEN];
	char szMessageBox[STRLEN];
	char szStr[STRLEN];
}DATA,*PDATA;

void UnInjectDll(LPVOID lpParam)
{
	PDATA pData = (PDATA)lpParam;
	HMODULE(__stdcall *MyLoadLibrary)(LPCTSTR);
	FARPROC(__stdcall *MyGetProcAddress)(HMODULE, LPCTSTR);
	HMODULE(__stdcall *MyGetModuleHandle)(LPCTSTR);

	int(__stdcall *MyMessageBox)(HWND, LPCTSTR, LPCTSTR, UINT);
	DWORD(__stdcall *MyModuleFileName)(HMODULE, LPCTSTR, DWORD);
	MyLoadLibrary = (HMODULE(__stdcall*)(LPCTSTR))pData->dwLoadLibrary;
	MyGetProcAddress = (FARPROC(__stdcall *)(HMODULE, LPCTSTR))pData->dwGetProcAddress;
	MyGetModuleHandle = (HMODULE(__stdcall *)(LPCTSTR))pData->dwGetModuleHandle;
	MyModuleFileName = (DWORD(__stdcall *)(HMODULE, LPCTSTR, DWORD))pData->dwGetModuleFileName;
	HMODULE hModule = MyLoadLibrary(pData->szUser32Dll);
	MyMessageBox = (int(__stdcall *)(HWND, LPCTSTR, LPCTSTR, UINT))MyGetProcAddress(hModule, pData->szMessageBox);
	char szModuleName[MAX_PATH] = { 0 };
	MyModuleFileName(NULL, szModuleName, MAX_PATH);
	MyMessageBox(NULL, pData->szStr, szModuleName, MB_OK);
}
//
void InjectDll(DWORD dwPid)
{
	if (dwPid == 0)
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

	DATA Data = { 0 };
	Data.dwLoadLibrary = (DWORD)GetProcAddress(GetModuleHandle("ketnel32.dll"), pFunName);
	Data.dwGetProcAddress = (DWORD)GetProcAddress(GetModuleHandle("ketnel32.dll"), "GetProcAddress");
	Data.dwGetModuleHandle = (DWORD)GetProcAddress(GetModuleHandle("ketnel32.dll"), "GetModuleHandleA");
	Data.dwGetModuleFileName = (DWORD)GetProcAddress(GetModuleHandle("ketnel32.dll"), "GetModuleFileNameA");
	lstrcpy(Data.szUser32Dll, "user32.dll");
	lstrcpy(Data.szMessageBox, "MessageBoxA");
	lstrcpy(Data.szStr, "Inject Code!!");

	LPVOID lpData = VirtualAllocEx(hProcess, NULL, sizeof(DATA), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
	if (lpData == NULL)
	{
		CloseHandle(hProcess);
		return;
	}

	SIZE_T dwWriteNum = 0;
	WriteProcessMemory(hProcess, lpData, &Data, sizeof(DATA), &dwWriteNum);
	DWORD dwFunSize = 0x2000;
	LPVOID lpCode = VirtualAllocEx(hProcess, NULL, dwFunSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpCode, lpData, 0, NULL);
	//WaitForSingleObject(hThread, INFINITE);
	//CloseHandle(hThread);
	//CloseHandle(hProcess);
	UnInjectDll((LPVOID)&Data);
}


int main()
{
	std::cout.imbue(std::locale("chs"));
	InjectDll(6584);
	getchar();
	return 0;
}
