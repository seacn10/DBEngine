// KernelEngine.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DBEngine.h"

//��̬����
static AFX_EXTENSION_MODULE	KernelEngineDLL = { NULL, NULL };

//ȫ�ֺ���
extern "C" INT APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(KernelEngineDLL, hInstance)) return 0;
		new CDynLinkLibrary(KernelEngineDLL);

		//���� COM
		CoInitialize(NULL);
	}
	else if (dwReason == DLL_THREAD_ATTACH)
	{
		//���� COM
		CoInitialize(NULL);
	}
	else if (dwReason == DLL_THREAD_DETACH)
	{
		//�ͷ� COM
		CoUninitialize();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		//�ͷ� COM
		CoUninitialize();

		AfxTermExtensionModule(KernelEngineDLL);
	}

	return 1;
}