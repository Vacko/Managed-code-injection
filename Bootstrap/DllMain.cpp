#include <Windows.h>
#include <mscoree.h>
#include <metahost.h>
#include <wchar.h>
#include <fstream>

#pragma comment(lib, "mscoree.lib")

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

static const LPCWSTR assemblyPath = L"\\CodeInject.dll";
static const LPCWSTR classFqn = L"CodeInject.Startup";
static const LPCWSTR methodName = L"EntryPoint";
static const LPCWSTR parameter = L"";

DWORD WINAPI CreateDotNetRunTime(LPVOID lpParam)
{
	ICLRMetaHost * lpMetaHost = NULL;
	HRESULT hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost,
		(LPVOID *)&lpMetaHost);

	ICLRRuntimeInfo * lpRuntimeInfo = NULL;

	hr = lpMetaHost->GetRuntime(
		L"v4.0.30319", 
		IID_ICLRRuntimeInfo,
		(LPVOID *)&lpRuntimeInfo);

	ICLRRuntimeHost * lpRuntimeHost = NULL;
	hr = lpRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost,
		(LPVOID *)&lpRuntimeHost);

	hr = lpRuntimeHost->Start();

	LPWSTR strDLLPath1 = new WCHAR[_MAX_PATH];
	::GetModuleFileNameW((HINSTANCE)&__ImageBase, strDLLPath1, _MAX_PATH);

	std::wstring tempPath = strDLLPath1;
	int index = tempPath.rfind('\\');
	tempPath.erase(index, tempPath.length() - index);
	tempPath += assemblyPath;

	DWORD dwRetCode = 0;
	hr = lpRuntimeHost->ExecuteInDefaultAppDomain(
		(LPWSTR)tempPath.c_str(),
		classFqn,
		methodName,
		parameter,
		&dwRetCode);

	//hr = lpRuntimeHost->Stop();
	//lpRuntimeHost->Release();

	return 0;
}


DWORD APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, CreateDotNetRunTime, NULL, NULL, NULL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
	default:
		break;
	}
	return true;
}