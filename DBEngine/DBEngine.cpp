// KernelEngine.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DBEngine.h"

//静态变量
static AFX_EXTENSION_MODULE	KernelEngineDLL = { NULL, NULL };

//全局函数
extern "C" INT APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(KernelEngineDLL, hInstance)) return 0;
		new CDynLinkLibrary(KernelEngineDLL);

		//加载 COM
		CoInitialize(NULL);
	}
	else if (dwReason == DLL_THREAD_ATTACH)
	{
		//加载 COM
		CoInitialize(NULL);
	}
	else if (dwReason == DLL_THREAD_DETACH)
	{
		//释放 COM
		CoUninitialize();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		//释放 COM
		CoUninitialize();

		AfxTermExtensionModule(KernelEngineDLL);
	}

	return 1;
}