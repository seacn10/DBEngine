#pragma once
#include "DBEngineHead.h"
#include "DataBaseException.h"

#ifdef _UNICODE
#define VER_IDataBase INTERFACE_VERSION(1,1)
static const GUID IID_IDataBase = { 0x7fbbbffa, 0xedf4, 0x43d2, 0x008a, 0xb7, 0x28, 0x87, 0x3b, 0xd0, 0xf5, 0x3f };
#else
#define VER_IDataBase INTERFACE_VERSION(1,1)
static const GUID IID_IDataBase = { 0xa2e38a78, 0x1e4f, 0x4de4, 0x00a5, 0xd1, 0xb9, 0x19, 0x9b, 0xce, 0x41, 0xae };
#endif

//数据库接口
interface IDataBase : public IUnknownEx
{
	//连接接口
public:
	//打开连接
	virtual VOID OpenConnection() = NULL;
	//关闭连接
	virtual VOID CloseConnection() = NULL;
	//连接信息
	virtual bool SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword) = NULL;
	//连接信息
	virtual bool SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword) = NULL;

	//参数接口
public:
	//清除参数
	virtual VOID ClearParameters() = NULL;
	//获取参数
	virtual VOID GetParameter(LPCTSTR pszParamName, CDBVarValue & DBVarValue) = NULL;
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszName, ADOCG::DataTypeEnum Type, ADOCG::ParameterDirectionEnum Direction, LONG lSize, CDBVarValue & DBVarValue) = NULL;

	//控制接口
public:
	//切换记录
	virtual VOID NextRecordset() = NULL;
	//关闭记录
	virtual VOID CloseRecordset() = NULL;
	//绑定对象
	virtual VOID BindToRecordset(CADORecordBinding * pBind) = NULL;

	//记录接口
public:
	//往下移动
	virtual VOID MoveToNext() = NULL;
	//移到开头
	virtual VOID MoveToFirst() = NULL;
	//是否结束
	virtual bool IsRecordsetEnd() = NULL;
	//获取数目
	virtual LONG GetRecordCount() = NULL;
	//返回数值
	virtual LONG GetReturnValue() = NULL;
	//获取数据
	virtual VOID GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue) = NULL;

	//控制接口
public:
	//存储过程
	virtual VOID ExecuteProcess(LPCTSTR pszSPName, bool bRecordset) = NULL;
	//执行语句
	virtual VOID ExecuteSentence(LPCTSTR pszCommand, bool bRecordset) = NULL;
};

class CDataBase : public IDataBase
{
	//连接信息
protected:
	CString							m_strConnect;						//连接字符
	CDataBaseException				m_DataBaseException;				//错误对象

	//状态变量
protected:
	DWORD							m_dwConnectCount;					//重试次数
	DWORD							m_dwConnectErrorTime;				//错误时间
	const DWORD						m_dwResumeConnectTime;				//恢复时间
	const DWORD						m_dwResumeConnectCount;				//恢复次数

	//内核变量
protected:
	_CommandPtr						m_DBCommand;						//命令对象
	_RecordsetPtr					m_DBRecordset;						//记录对象
	_ConnectionPtr					m_DBConnection;						//数据对象

	//函数定义
public:
	//构造函数
	CDataBase();
	//析构函数
	virtual ~CDataBase();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//连接接口
public:
	//打开连接
	virtual VOID OpenConnection();
	//关闭连接
	virtual VOID CloseConnection();
	//连接信息
	virtual bool SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);
	//连接信息
	virtual bool SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);

	//参数接口
public:
	//清除参数
	virtual VOID ClearParameters();
	//获取参数
	virtual VOID GetParameter(LPCTSTR pszParamName, CDBVarValue & DBVarValue);
	//插入参数
	virtual VOID AddParameter(LPCTSTR pszName, ADOCG::DataTypeEnum Type, ADOCG::ParameterDirectionEnum Direction, LONG lSize, CDBVarValue & DBVarValue);

	//控制接口
public:
	//切换记录
	virtual VOID NextRecordset();
	//关闭记录
	virtual VOID CloseRecordset();
	//绑定对象
	virtual VOID BindToRecordset(CADORecordBinding * pBind);

	//记录接口
public:
	//往下移动
	virtual VOID MoveToNext();
	//移到开头
	virtual VOID MoveToFirst();
	//是否结束
	virtual bool IsRecordsetEnd();
	//获取数目
	virtual LONG GetRecordCount();
	//返回数值
	virtual LONG GetReturnValue();
	//获取数据
	virtual VOID GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue);

	//控制接口
public:
	//存储过程
	virtual VOID ExecuteProcess(LPCTSTR pszSPName, bool bRecordset);
	//执行语句
	virtual VOID ExecuteSentence(LPCTSTR pszCommand, bool bRecordset);

	//内部函数
protected:
	//连接错误
	bool IsConnectError();
	//是否打开
	bool IsRecordsetOpened();
	//重新连接
	bool TryConnectAgain(CComError * pComError);
	//转换字符
	VOID ConvertToSQLSyntax(LPCTSTR pszString, CString & strResult);
	//错误处理
	VOID OnSQLException(enSQLException SQLException, CComError * pComError);
};


DECLARE_MODULE_HELPER(DataBase, L"DBEngine.dll", "CreateDataBase")