#ifndef KERNELENGINEHEAD_HEAD_FILE
#define KERNELENGINEHEAD_HEAD_FILE

#include <ICrsint.h>

#include "Module.h"

//ADO 定义
#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;

typedef _com_error						CComError;						//COM 错误
typedef _variant_t						CDBVarValue;					//数据库数值

//导出定义
#ifndef DB_ENGINE_CLASS
#ifdef  DB_ENGINE_DLL
#define DB_ENGINE_CLASS _declspec(dllexport)
#else
#define DB_ENGINE_CLASS _declspec(dllimport)
#endif
#endif

//模块定义
#ifndef _DEBUG
#define KERNEL_ENGINE_DLL_NAME	TEXT("DBEngine.dll")			//组件 DLL 名字
#else
#define KERNEL_ENGINE_DLL_NAME	TEXT("DBEngineD.dll")			//组件 DLL 名字
#endif

//SQL 异常类型
enum enSQLException
{
	SQLException_None = 0,										//没有异常
	SQLException_Connect = 1,									//连接错误
	SQLException_Syntax = 2,									//语法错误
};

#endif