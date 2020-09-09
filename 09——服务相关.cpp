// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <map>
#include <tchar.h>
//#include <afxwin.h>

void StopService(TCHAR szServerName[])
{
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCM)
	{
		std::wstring strInfo;
		strInfo = L"OpenSCManager Failed!\n";
		strInfo += GetLastError();
		MessageBox(NULL, L"失败", strInfo.data(), MB_OK);
		return;
	}
	SC_HANDLE hSCService = OpenService(hSCM, szServerName, SERVICE_ALL_ACCESS);
	if (!hSCService)
	{
		//Return code					Description
		//ERROR_ACCESS_DENIED			The handle does not have access to the service.
		//ERROR_INVALID_HANDLE			The specified handle is invalid.
		//ERROR_INVALID_NAME			The specified service name is invalid.
		//ERROR_SERVICE_DOES_NOT_EXIST	The specified service does not exist.
		std::wstring strInfo;
		strInfo = L"OpenService Failed!\n";
		strInfo += GetLastError();
		MessageBox(NULL, L"失败", strInfo.data(), MB_OK);
		return;
	}
	SERVICE_STATUS ServiceStatus;
	//停止服务
	BOOL bRet = ControlService(hSCService, SERVICE_CONTROL_STOP, &ServiceStatus);
	if (!bRet)
	{
		std::wstring strInfo;
		strInfo = L"ControlService Failed!\n";
		strInfo += GetLastError();
		MessageBox(NULL, L"失败", strInfo.data(), MB_OK);
		return;
	}

	CloseServiceHandle(hSCService);
	CloseServiceHandle(hSCM);
}

void StartService(TCHAR szServerName[])
{
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCM)
	{
		std::wstring strInfo;
		strInfo = L"OpenSCManager Failed!\n";
		strInfo += GetLastError();
		MessageBox(NULL, L"失败", strInfo.data(), MB_OK);
		return;
	}
	SC_HANDLE hSCService = OpenService(hSCM, szServerName, SERVICE_ALL_ACCESS);
	if (!hSCService)
	{
		std::wstring strInfo;
		strInfo = L"OpenService Failed!\n";
		strInfo += GetLastError();
		MessageBox(NULL, L"失败", strInfo.data(), MB_OK);
		return;
	}
	BOOL bRet = StartService(hSCService, 0, NULL);
	if (!bRet)
	{
		std::wstring strInfo;
		strInfo = L"StartService Failed!\n";
		strInfo += GetLastError();
		MessageBox(NULL, L"失败", strInfo.data(), MB_OK);
		return;
	}
	CloseServiceHandle(hSCService);
	CloseServiceHandle(hSCM);
}

void GetService(std::map<int, std::wstring> mapRegKey, DWORD dwServiceType)
{
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCM)
	{
		std::wstring strInfo;
		strInfo = L"OpenSCManager Failed!\n";
		strInfo += GetLastError();
		MessageBox(NULL, L"失败", strInfo.data(), MB_OK);
		return;
	}

	DWORD dwBufferSize = 512 * sizeof(ENUM_SERVICE_STATUS);
	DWORD dwByteNeeded = 0, dwServiceReturned = 0, lpResumeHandle = 0;
	ENUM_SERVICE_STATUS SerStatus[512] = { 0 };
	BOOL bRet = EnumServicesStatus(hSCM, dwServiceType, SERVICE_STATE_ALL,
		SerStatus, dwBufferSize, &dwByteNeeded, &dwServiceReturned, &lpResumeHandle);
	if (!bRet)
	{
		std::wstring strInfo;
		strInfo = L"StartService Failed!\n";
		strInfo += GetLastError();
		MessageBox(NULL, L"失败", strInfo.data(), MB_OK);
		return;
	}
	for (size_t i = 0; i < dwServiceReturned; i++)
	{
		std::wstring strState;
		switch (SerStatus[i].ServiceStatus.dwCurrentState)
		{
//
// Service State -- for CurrentState
//
//#define SERVICE_STOPPED                        0x00000001
//#define SERVICE_START_PENDING                  0x00000002
//#define SERVICE_STOP_PENDING                   0x00000003
//#define SERVICE_RUNNING                        0x00000004
//#define SERVICE_CONTINUE_PENDING               0x00000005
//#define SERVICE_PAUSE_PENDING                  0x00000006
//#define SERVICE_PAUSED                         0x00000007

		case SERVICE_STOPPED:
			strState = L"停止";
			break;
		case SERVICE_START_PENDING:
			strState = L"开始挂起";
			break;
		case SERVICE_STOP_PENDING:
			strState = L"停止挂起";
			break;
		case SERVICE_RUNNING:
			strState = L"运行";
			break;
		case SERVICE_CONTINUE_PENDING:
			strState = L"继续挂起";
			break;
		case SERVICE_PAUSE_PENDING:
			strState = L"暂停挂起";
			break;
		case SERVICE_PAUSED:
			strState = L"暂停";
			break;
		default:
			break;
		}
		std::wcout << L"ID:" << i << L" ServiceName:" << SerStatus[i].lpServiceName
			<< L" ServiceDisplayName:" << SerStatus[i].lpDisplayName << L" 状态:"
			<< strState << std::endl;
	}
}

void ShowService()
{

}

int main()
{
//https://docs.microsoft.com/en-us/windows/win32/api/winsvc/
//winsvc.h
	//注意权限问题
	std::wcout.imbue(std::locale("chs"));
	std::map<int, std::wstring> mapRegKey;
//The type of services to be enumerated.This parameter can be one or more of the following values.
//	Value	Meaning
//	SERVICE_DRIVER
//	0x0000000B
//	Services of type SERVICE_KERNEL_DRIVER and SERVICE_FILE_SYSTEM_DRIVER.
//	SERVICE_FILE_SYSTEM_DRIVER
//	0x00000002
//	File system driver services.
//	SERVICE_KERNEL_DRIVER
//	0x00000001
//	Driver services.
//	SERVICE_WIN32
//	0x00000030
//	Services of type SERVICE_WIN32_OWN_PROCESS and SERVICE_WIN32_SHARE_PROCESS.
//	SERVICE_WIN32_OWN_PROCESS
//	0x00000010
//	Services that run in their own processes.
//	SERVICE_WIN32_SHARE_PROCESS
//	0x00000020
//	Services that share a process with one or more other services.For more information, see Service Programs.

	std::wcout << L"驱动:" << std::endl;
	DWORD dwServiceType = SERVICE_DRIVER;
	GetService(mapRegKey, dwServiceType);

	std::wcout << L"文件系统驱动:" << std::endl;
	dwServiceType = SERVICE_FILE_SYSTEM_DRIVER;
	GetService(mapRegKey, dwServiceType);

	std::wcout << L"内核驱动:" << std::endl;
	dwServiceType = SERVICE_KERNEL_DRIVER;
	GetService(mapRegKey, dwServiceType);

	std::wcout << L"Win32服务:" << std::endl;
	dwServiceType = SERVICE_WIN32;
	GetService(mapRegKey, dwServiceType);

	std::wcout << L"Win32在自己的进程中运行的服务。:" << std::endl;
	dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	GetService(mapRegKey, dwServiceType);

	std::wcout << L"Win32与一个或多个其他服务共享进程的服务。有关:" << std::endl;
	dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
	GetService(mapRegKey, dwServiceType);
	getchar();
	return 0;
}
