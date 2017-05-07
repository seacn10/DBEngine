#ifndef DATABASEEXCEPTION_HEAD_FILE
#define DATABASEEXCEPTION_HEAD_FILE

#include <atlstr.h>
#include "DBEngineHead.h"

#ifdef _UNICODE
#define VER_IDataBaseException INTERFACE_VERSION(1,1)
static const GUID IID_IDataBaseException = { 0x008be9d3, 0x2305, 0x40da, 0x00ae, 0xd1, 0x61, 0x7a, 0xd2, 0x2a, 0x47, 0xfc };
#else
#define VER_IDataBaseException INTERFACE_VERSION(1,1)
static const GUID IID_IDataBaseException = { 0x428361ed, 0x9dfa, 0x43d7, 0x008f, 0x26, 0x17, 0x06, 0x47, 0x6b, 0x2a, 0x51 };
#endif

//数据库异常
interface IDataBaseException : public IUnknownEx
{
	//异常代码
	virtual HRESULT GetExceptionResult() = NULL;
	//异常描述
	virtual LPCTSTR GetExceptionDescribe() = NULL;
	//异常类型
	virtual enSQLException GetExceptionType() = NULL;
};

//ADO 错误类
class CDataBaseException : public IDataBaseException
{
	//错误类型
protected:
	HRESULT							m_hResult;							//异常代码
	enSQLException					m_SQLException;						//异常类型

	//辅助变量
protected:
	CString						m_strDescribe;						//异常信息

	//函数定义
public:
	//构造函数
	CDataBaseException();
	//析构函数
	virtual ~CDataBaseException();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//功能接口
public:
	//错误代号
	virtual HRESULT GetExceptionResult() { return m_hResult; }
	//错误描述
	virtual LPCTSTR GetExceptionDescribe() { return m_strDescribe; }
	//错误类型
	virtual enSQLException GetExceptionType() { return m_SQLException; }

	//功能函数
public:
	//设置错误
	VOID SetExceptionInfo(enSQLException SQLException, CComError * pComError);
};

#endif