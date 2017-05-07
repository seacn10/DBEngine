#include "stdafx.h"
#include "DataBaseException.h"

//构造函数
CDataBaseException::CDataBaseException()
{
	//错误类型
	m_hResult = S_OK;
	m_SQLException = SQLException_None;

	return;
}

//析构函数
CDataBaseException::~CDataBaseException()
{
}

//接口查询
VOID * CDataBaseException::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseException, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseException, Guid, dwQueryVer);
	return NULL;
}

//设置错误
VOID CDataBaseException::SetExceptionInfo(enSQLException SQLException, CComError * pComError)
{
	//效验参数
	//assert(pComError != NULL);
	if (pComError == NULL) return;

	//设置信息
	m_SQLException = SQLException;
	m_hResult = pComError->Error();
	m_strDescribe.Format(TEXT("数据库异常：%s [ 0x%8x ]"), (LPCTSTR)(pComError->Description()), pComError->Error());

	return;
}