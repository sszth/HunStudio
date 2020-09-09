// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <map>
#include <tchar.h>
//#include <afxwin.h>

//winreg.h Win REG 集合
//https://docs.microsoft.com/en-us/windows/win32/api/winreg/



//AbortSystemShutdownA	Stops a system shutdown that has been initiated.
//AbortSystemShutdownW	Stops a system shutdown that has been initiated.
//Stops a system shutdown that has been initiated.
//停止已启动的系统关闭。
//BOOL AbortSystemShutdownW(
//LPWSTR lpMachineName	//计算机网络名称 如果是NULL代表本地
//);
//成功返回非0  失败返回0,GetLastError
//若要阻止本地计算机关闭，调用进程必须具有SE_STOWINE_NAME特权。
//若要阻止远程计算机关闭，调用进程必须在远程计算机上具有SE_Remote_SUBUT_NAME特权。
//默认情况下，用户可以在登录到的计算机上启用SE_STOWINE_NAME特权，管理员可以在远程计算机上启用SE_Remote_STOWINE_NAME特权。

//InitiateShutdownA	Initiates a shutdown and restart of the specified computer, and restarts any applications that have been registered for restart.
//InitiateShutdownW	Initiates a shutdown and restart of the specified computer, and restarts any applications that have been registered for restart.
//启动指定计算机的关闭和重新启动，并重新启动已注册以重新启动的任何应用程序。
//DWORD InitiateShutdownW(
//	LPWSTR lpMachineName,	//要关闭的计算机的名称。如果此参数的值为零本地计算机被关闭。
//	LPWSTR lpMessage,		//要在“交互式关机”对话框中显示的消息。
//	DWORD  dwGracePeriod,	//关机时间 0立即关闭  最大不超过MAX_SHUTDOWN_TIMEOUT（10年） 
//	DWORD  dwShutdownFlags,	//见下列选项
//	DWORD  dwReason
//);
//如果dwShutdownFlags为SHUTDOWN_GRACE_OVERRIDE并且dwGracePeriod>0 函数失败并返回ERROR_BAD_ARGUMENTS
//dwShutdownFlags:
//SHUTDOWN_FORCE_OTHERS			0x00000001 (0x1)
//所有会话都被强制注销。如果未设置此标志，并且当前用户以外的其他用户登录到由lpMachineName参数时，此函数将失败，返回值为ERROR_SHUTDOWN_USERS_LOGGED_ON.
//SHUTDOWN_FORCE_SELF			0x00000002 (0x2)
//指定强制注销原始会话。如果未设置此标志，则会以交互方式关闭起始会话，因此即使函数成功返回，也不能保证关闭。
//SHUTDOWN_GRACE_OVERRIDE		0x00000020 (0x20)
//Overrides the grace period so that the computer is shut down immediately.
//SHUTDOWN_HYBRID				0x00000200 (0x200)
//Beginning with InitiateShutdown running on Windows 8, you must include the SHUTDOWN_HYBRID flag with one or more of the flags in this table to specify options for the shutdown.
//Beginning with Windows 8, InitiateShutdown always initiate a full system shutdown if the SHUTDOWN_HYBRID flag is absent.
//
//SHUTDOWN_INSTALL_UPDATES		0x00000040 (0x40)
//The computer installs any updates before starting the shutdown.
//SHUTDOWN_NOREBOOT				0x00000010 (0x10)
//The computer is shut down but is not powered down or rebooted.
//SHUTDOWN_POWEROFF				0x00000008 (0x8)
//The computer is shut down and powered down.
//SHUTDOWN_RESTART				0x00000004 (0x4)
//The computer is shut down and rebooted.
//SHUTDOWN_RESTARTAPPS			0x00000080 (0x80)
//The system is rebooted using the ExitWindowsEx function with the EWX_RESTARTAPPS flag.This restarts any applications that have been registered for restart using the RegisterApplicationRestart function.

//InitiateSystemShutdownA	Initiates a shutdown and optional restart of the specified computer.
//InitiateSystemShutdownExA	Initiates a shutdown and optional restart of the specified computer, and optionally records the reason for the shutdown.
//InitiateSystemShutdownExW	Initiates a shutdown and optional restart of the specified computer, and optionally records the reason for the shutdown.
//InitiateSystemShutdownW	Initiates a shutdown and optional restart of the specified computer.
//RegCloseKey	Closes a handle to the specified registry key.
//RegConnectRegistryA	Establishes a connection to a predefined registry key on another computer.
//RegConnectRegistryW	Establishes a connection to a predefined registry key on another computer.
//RegCopyTreeA	Copies the specified registry key, along with its values and subkeys, to the specified destination key.
//RegCopyTreeW	Copies the specified registry key, along with its values and subkeys, to the specified destination key.
//RegCreateKeyA	Creates the specified registry key.If the key already exists in the registry, the function opens it.
//RegCreateKeyExA	Creates the specified registry key.If the key already exists, the function opens it.Note that key names are not case sensitive.
//RegCreateKeyExW	Creates the specified registry key.If the key already exists, the function opens it.Note that key names are not case sensitive.
//RegCreateKeyTransactedA	Creates the specified registry key and associates it with a transaction.
//RegCreateKeyTransactedW	Creates the specified registry key and associates it with a transaction.
//RegCreateKeyW	Creates the specified registry key.If the key already exists in the registry, the function opens it.
//RegDeleteKeyA	Deletes a subkey and its values.
//RegDeleteKeyExA	Deletes a subkey and its values from the specified platform - specific view of the registry.
//RegDeleteKeyExW	Deletes a subkey and its values from the specified platform - specific view of the registry.
//RegDeleteKeyTransactedA	Deletes a subkey and its values from the specified platform - specific view of the registry as a transacted operation.
//RegDeleteKeyTransactedW	Deletes a subkey and its values from the specified platform - specific view of the registry as a transacted operation.
//RegDeleteKeyValueA	Removes the specified value from the specified registry key and subkey.
//RegDeleteKeyValueW	Removes the specified value from the specified registry key and subkey.
//RegDeleteKeyW	Deletes a subkey and its values.
//RegDeleteTreeA	Deletes the subkeys and values of the specified key recursively.
//RegDeleteTreeW	Deletes the subkeys and values of the specified key recursively.
//RegDeleteValueA	Removes a named value from the specified registry key.
//RegDeleteValueW	Removes a named value from the specified registry key.
//RegDisablePredefinedCache	Disables handle caching of the predefined registry handle for HKEY_CURRENT_USER for the current process.
//RegDisablePredefinedCacheEx	Disables handle caching for all predefined registry handles for the current process.
//RegDisableReflectionKey	Disables registry reflection for the specified key.Disabling reflection for a key does not affect reflection of any subkeys.
//RegEnableReflectionKey	Restores registry reflection for the specified disabled key.Restoring reflection for a key does not affect reflection of any subkeys.
//RegEnumKeyA	Enumerates the subkeys of the specified open registry key.
//RegEnumKeyExA	Enumerates the subkeys of the specified open registry key.The function retrieves information about one subkey each time it is called.
//RegEnumKeyExW	Enumerates the subkeys of the specified open registry key.The function retrieves information about one subkey each time it is called.
//RegEnumKeyW	Enumerates the subkeys of the specified open registry key.
//RegEnumValueA	Enumerates the values for the specified open registry key.The function copies one indexed value name and data block for the key each time it is called.
//RegEnumValueW	Enumerates the values for the specified open registry key.The function copies one indexed value name and data block for the key each time it is called.
//RegFlushKey	Writes all the attributes of the specified open registry key into the registry.
//RegGetKeySecurity	Retrieves a copy of the security descriptor protecting the specified open registry key.
//RegGetValueA	Retrieves the type and data for the specified registry value.
//RegGetValueW	Retrieves the type and data for the specified registry value.
//RegLoadAppKeyA	Loads the specified registry hive as an application hive.
//RegLoadAppKeyW	Loads the specified registry hive as an application hive.
//RegLoadKeyA	Creates a subkey under HKEY_USERS or HKEY_LOCAL_MACHINE and loads the data from the specified registry hive into that subkey.
//RegLoadKeyW	Creates a subkey under HKEY_USERS or HKEY_LOCAL_MACHINE and loads the data from the specified registry hive into that subkey.
//RegLoadMUIStringA	Loads the specified string from the specified key and subkey.
//RegLoadMUIStringW	Loads the specified string from the specified key and subkey.
//RegNotifyChangeKeyValue	Notifies the caller about changes to the attributes or contents of a specified registry key.
//RegOpenCurrentUser	Retrieves a handle to the HKEY_CURRENT_USER key for the user the current thread is impersonating.
//RegOpenKeyA	Opens the specified registry key.
//RegOpenKeyExA	Opens the specified registry key.Note that key names are not case sensitive.
//RegOpenKeyExW	Opens the specified registry key.Note that key names are not case sensitive.
//RegOpenKeyTransactedA	Opens the specified registry key and associates it with a transaction.
//RegOpenKeyTransactedW	Opens the specified registry key and associates it with a transaction.
//RegOpenKeyW	Opens the specified registry key.
//RegOpenUserClassesRoot	Retrieves a handle to the HKEY_CLASSES_ROOT key for a specified user.The user is identified by an access token.
//RegOverridePredefKey	Maps a predefined registry key to the specified registry key.
//RegQueryInfoKeyA	Retrieves information about the specified registry key.
//RegQueryInfoKeyW	Retrieves information about the specified registry key.
//RegQueryMultipleValuesA	Retrieves the type and data for a list of value names associated with an open registry key.
//RegQueryMultipleValuesW	Retrieves the type and data for a list of value names associated with an open registry key.
//RegQueryReflectionKey	Determines whether reflection has been disabled or enabled for the specified key.
//RegQueryValueA	Retrieves the data associated with the default or unnamed value of a specified registry key.The data must be a null - terminated string.
//RegQueryValueExA	Retrieves the type and data for the specified value name associated with an open registry key.
//RegQueryValueExW	Retrieves the type and data for the specified value name associated with an open registry key.
//RegQueryValueW	Retrieves the data associated with the default or unnamed value of a specified registry key.The data must be a null - terminated string.
//RegReplaceKeyA	Replaces the file backing a registry key and all its subkeys with another file, so that when the system is next started, the key and subkeys will have the values stored in the new file.
//RegReplaceKeyW	Replaces the file backing a registry key and all its subkeys with another file, so that when the system is next started, the key and subkeys will have the values stored in the new file.
//RegRestoreKeyA	Reads the registry information in a specified file and copies it over the specified key.This registry information may be in the form of a key and multiple levels of subkeys.
//RegRestoreKeyW	Reads the registry information in a specified file and copies it over the specified key.This registry information may be in the form of a key and multiple levels of subkeys.
//RegSaveKeyA	Saves the specified key and all of its subkeys and values to a new file, in the standard format.
//RegSaveKeyExA	Saves the specified key and all of its subkeys and values to a registry file, in the specified format.
//RegSaveKeyExW	Saves the specified key and all of its subkeys and values to a registry file, in the specified format.
//RegSaveKeyW	Saves the specified key and all of its subkeys and values to a new file, in the standard format.
//RegSetKeySecurity	Sets the security of an open registry key.
//RegSetKeyValueA	Sets the data for the specified value in the specified registry key and subkey.
//RegSetKeyValueW	Sets the data for the specified value in the specified registry key and subkey.
//RegSetValueA	Sets the data for the default or unnamed value of a specified registry key.The data must be a text string.
//RegSetValueExA	Sets the data and type of a specified value under a registry key.
//RegSetValueExW	Sets the data and type of a specified value under a registry key.
//RegSetValueW	Sets the data for the default or unnamed value of a specified registry key.The data must be a text string.
//RegUnLoadKeyA	Unloads the specified registry key and its subkeys from the registry.
//RegUnLoadKeyW	Unloads the specified registry key and its subkeys from the registry.

//#define REG_RUN 
struct HunKeyInfo
{
	std::wstring m_strValueName;
	std::wstring m_strValueKey;
	HunKeyInfo(const std::wstring& strValueName, const std::wstring& strValueKey)
		: m_strValueName(strValueName), m_strValueKey(strValueKey)
	{

	}
	HunKeyInfo()
	{}
	HunKeyInfo& operator = (const HunKeyInfo& src)
	{
		if (this == &src)
		{
			return *this;
		}
		m_strValueKey = src.m_strValueKey;
		m_strValueName = src.m_strValueName;
		return *this;
	}
};

TCHAR	g_szwRun[] = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run\\";

void GetRegKey(std::map<int, HunKeyInfo>& mapRegKey)
{
	DWORD dwType = 0;
	DWORD dwBufferSize = MAXBYTE;
	DWORD dwKeySize = MAXBYTE;
	//std::wstring strValueName, strValueKey;
	TCHAR  szValueName[MAXBYTE] = { 0 };
	TCHAR  szValueKey[MAXBYTE] = { 0 };
	HKEY hKey = NULL;
	LONG lRet = RegOpenKey(HKEY_LOCAL_MACHINE, (LPCWSTR)g_szwRun, &hKey);
	if (lRet != ERROR_SUCCESS)
	{
		return;
	}

	int nIndex = 0;
	std::wstring strTmp;
	while (true)
	{
		//lRet = RegQueryValueEx(hKey, szValueName, 0, 0, (BYTE*)&szValueKey, &dwKeySize);
		lRet = RegEnumValue(hKey, nIndex, szValueName, &dwBufferSize, NULL, &dwType, (LPBYTE)szValueKey, &dwKeySize);
		if (lRet == ERROR_NO_MORE_ITEMS)
		{
			break;
		}
		mapRegKey[nIndex] = HunKeyInfo(szValueName, szValueKey);
		ZeroMemory(szValueName, MAXBYTE);
		ZeroMemory(szValueKey, MAXBYTE);
		nIndex++;
	}
	RegCloseKey(hKey);
}

void Show(const std::map<int, HunKeyInfo>& mapRegKey)
{
	for (auto iter : mapRegKey)
	{
		std::wcout << L"ID:" << iter.first 
			<< L" 名称：" << iter.second.m_strValueName 
			<< L" 值：" << iter.second.m_strValueKey << std::endl;
	}
}

void RegAdd()
{
	std::wstring strRegName, strRegValue;
	std::wcout << L"添加注册表名称:" << std::endl;
	std::wcin >> strRegName;
	std::wcout << L"添加注册表值:" << std::endl;
	std::wcin >> strRegValue;
	HKEY hKey = NULL;
	LONG lRet = RegOpenKey(HKEY_LOCAL_MACHINE, g_szwRun, &hKey);
	if (lRet != ERROR_SUCCESS)
	{
		return;
	}

	int nIndex = 0;
	lRet = RegSetValueEx(hKey, strRegName.data(),
		NULL, REG_EXPAND_SZ, (CONST BYTE*)strRegValue.data(), (strRegValue.length() + 1) * sizeof(TCHAR));
	if (lRet != ERROR_SUCCESS)
	{
		//容易拒绝访问
		RegCloseKey(hKey);
	}
	RegCloseKey(hKey);
}

void RegDelete(std::map<int, HunKeyInfo> mapRegKey, int nDelIndex)
{
	HKEY hKey = NULL;
	LONG lRet = RegOpenKey(HKEY_LOCAL_MACHINE, g_szwRun, &hKey);
	if (lRet != ERROR_SUCCESS)
	{
		return;
	}

	lRet = RegDeleteValue(hKey, mapRegKey[nDelIndex].m_strValueName.data());
	if (lRet != ERROR_SUCCESS)
	{
		return;
	}
	RegCloseKey(hKey);
}

int main()
{
	std::wcout.imbue(std::locale("chs"));
	std::map<int, HunKeyInfo> mapRegKey;
	GetRegKey(mapRegKey);
	Show(mapRegKey);
	RegAdd();
	GetRegKey(mapRegKey);
	Show(mapRegKey);
	RegDelete(mapRegKey, 2);
	GetRegKey(mapRegKey);
	Show(mapRegKey);

	return 0;
}
