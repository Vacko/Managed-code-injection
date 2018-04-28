#include <Windows.h>
#include <mscoree.h>
#include <metahost.h>
#include <wchar.h>
#include <fstream>

#pragma comment(lib, "mscoree.lib")

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

static const LPCTSTR Log      = "LogFile.log";
static const LPCWSTR Assembly = L"\\CodeInject.dll";
static const LPCWSTR Class    = L"CodeInject.Startup";
static const LPCWSTR Method   = L"EntryPoint";
static const LPCWSTR Param    = L"";


DWORD WINAPI CreateDotNetRunTime(LPVOID lpParam)
{
	ICLRRuntimeHost* lpRuntimeHost = NULL;
	ICLRRuntimeInfo* lpRuntimeInfo = NULL;
	ICLRMetaHost* lpMetaHost = NULL;
	FILE* file;

	LPWSTR AppPath = new WCHAR[_MAX_PATH];
	::GetModuleFileNameW((HINSTANCE)&__ImageBase, AppPath, _MAX_PATH);

	std::wstring tempPath = AppPath;
	int index = tempPath.rfind('\\');
	tempPath.erase(index, tempPath.length() - index);
	tempPath += Assembly;

	fopen_s(&file, Log, "a+");

	HRESULT hr = CLRCreateInstance(
		CLSID_CLRMetaHost, 
		IID_ICLRMetaHost, 
		(LPVOID*)&lpMetaHost
	);

	if (FAILED(hr))
	{
		fprintf(file, "Failed to create CLR instance.\n");
		fflush(file);
	}

	hr = lpMetaHost->GetRuntime(
		L"v4.0.30319", 
		IID_PPV_ARGS(&lpRuntimeInfo)
	);

	if (FAILED(hr))
	{
		fprintf(file, "Getting runtime failed.\n");
		fflush(file);

		lpMetaHost->Release();
	}

	BOOL fLoadable;
	hr = lpRuntimeInfo->IsLoadable(&fLoadable);

	if (FAILED(hr) || !fLoadable)
	{
		fprintf(file, "Runtime can't be loaded into the process.\n");
		fflush(file);

		lpRuntimeInfo->Release();
		lpMetaHost->Release();
	}

	hr = lpRuntimeInfo->GetInterface(
		CLSID_CLRRuntimeHost, 
		IID_PPV_ARGS(&lpRuntimeHost)
	);

	if (FAILED(hr))
	{
		fprintf(file, "Failed to acquire CLR runtime.\n");
		fflush(file);

		lpRuntimeInfo->Release();
		lpMetaHost->Release();
	}

	hr = lpRuntimeHost->Start();

	if (FAILED(hr))
	{
		fprintf(file, "Failed to start CLR runtime.\n");
		fflush(file);

		lpRuntimeHost->Release();
		lpRuntimeInfo->Release();
		lpMetaHost->Release();
	}

	DWORD dwRetCode = 0;

	hr = lpRuntimeHost->ExecuteInDefaultAppDomain(
		(LPWSTR)tempPath.c_str(), 
		Class,
		Method, 
		Param, 
		&dwRetCode
	);

	if (FAILED(hr))
	{
		fprintf(file, "Unable to execute assembly.\n");
		fflush(file);

		lpRuntimeHost->Stop();
		lpRuntimeHost->Release();
		lpRuntimeInfo->Release();
		lpMetaHost->Release();
	}

	fclose(file);

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