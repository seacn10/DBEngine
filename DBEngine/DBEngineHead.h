#ifndef KERNELENGINEHEAD_HEAD_FILE
#define KERNELENGINEHEAD_HEAD_FILE

#include <ICrsint.h>

#include "Module.h"

//ADO ����
#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;

typedef _com_error						CComError;						//COM ����
typedef _variant_t						CDBVarValue;					//���ݿ���ֵ

//��������
#ifndef DB_ENGINE_CLASS
#ifdef  DB_ENGINE_DLL
#define DB_ENGINE_CLASS _declspec(dllexport)
#else
#define DB_ENGINE_CLASS _declspec(dllimport)
#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
#define KERNEL_ENGINE_DLL_NAME	TEXT("DBEngine.dll")			//��� DLL ����
#else
#define KERNEL_ENGINE_DLL_NAME	TEXT("DBEngineD.dll")			//��� DLL ����
#endif

//SQL �쳣����
enum enSQLException
{
	SQLException_None = 0,										//û���쳣
	SQLException_Connect = 1,									//���Ӵ���
	SQLException_Syntax = 2,									//�﷨����
};

#endif