#ifndef DATABASE_AIDE_HEAD_FILE
#define DATABASE_AIDE_HEAD_FILE

#include "DataBaseEngine.h"

//存储长度
#define CountStringBufferA(String) ((UINT)((lstrlenA(String)+1)*sizeof(CHAR)))
#define CountStringBufferW(String) ((UINT)((lstrlenW(String)+1)*sizeof(WCHAR)))

//存储长度
#ifdef _UNICODE
#define CountStringBuffer CountStringBufferW
#else
#define CountStringBuffer CountStringBufferA
#endif

//数据库助手
class DB_ENGINE_CLASS CDataBaseAide
{
	//变量定义
protected:
	IDataBase *						m_pIDataBase;						//数据对象

	//函数定义
public:
	//构造函数
	CDataBaseAide(IUnknownEx * pIUnknownEx = NULL);
	//析构函数
	virtual ~CDataBaseAide();

	//管理函数
public:
	//设置对象
	bool SetDataBase(IUnknownEx * pIUnknownEx);
	//获取对象
	VOID * GetDataBase(REFGUID Guid, DWORD dwQueryVer);

	//获取数据
public:
	//获取数据
	INT GetValue_INT(LPCTSTR pszItem);
	//获取数据
	UINT GetValue_UINT(LPCTSTR pszItem);
	//获取数据
	LONG GetValue_LONG(LPCTSTR pszItem);
	//获取数据
	BYTE GetValue_BYTE(LPCTSTR pszItem);
	//获取数据
	WORD GetValue_WORD(LPCTSTR pszItem);
	//获取数据
	DWORD GetValue_DWORD(LPCTSTR pszItem);
	//获取数据
	FLOAT GetValue_FLOAT(LPCTSTR pszItem);
	//获取数据
	DOUBLE GetValue_DOUBLE(LPCTSTR pszItem);
	//获取数据
	LONGLONG GetValue_LONGLONG(LPCTSTR pszItem);
	//获取数据
	VOID GetValue_VarValue(LPCTSTR pszItem, CDBVarValue & DBVarValue);
	//获取数据
	VOID GetValue_SystemTime(LPCTSTR pszItem, SYSTEMTIME & SystemTime);
	//获取字符
	VOID GetValue_String(LPCTSTR pszItem, LPSTR pszString, UINT uMaxCount);
	//获取字符
	VOID GetValue_String(LPCTSTR pszItem, LPWSTR pszString, UINT uMaxCount);

	//参数函数
public:
	//重置参数
	VOID ResetParameter();
	//获取参数
	VOID GetParameter(LPCTSTR pszItem, CDBVarValue & DBVarValue);
	//获取参数
	VOID GetParameter(LPCTSTR pszItem, LPSTR pszString, UINT uSize);
	//获取参数
	VOID GetParameter(LPCTSTR pszItem, LPWSTR pszString, UINT uSize);

	//插入参数
public:
	//插入参数
	VOID AddParameter(LPCTSTR pszItem, INT nValue, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInput);
	//插入参数
	VOID AddParameter(LPCTSTR pszItem, UINT uValue, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInput);
	//插入参数
	VOID AddParameter(LPCTSTR pszItem, LONG lValue, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInput);
	//插入参数
	VOID AddParameter(LPCTSTR pszItem, LONGLONG lValue, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInput);
	//插入参数
	VOID AddParameter(LPCTSTR pszItem, BYTE cbValue, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInput);
	//插入参数
	VOID AddParameter(LPCTSTR pszItem, WORD wValue, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInput);
	//插入参数
	VOID AddParameter(LPCTSTR pszItem, DWORD dwValue, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInput);
	//插入参数
	VOID AddParameter(LPCTSTR pszItem, FLOAT fValue, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInput);
	//插入参数
	VOID AddParameter(LPCTSTR pszItem, DOUBLE dValue, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInput);
	//插入参数
	VOID AddParameter(LPCTSTR pszItem, LPCSTR pszString, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInput);
	//插入参数
	VOID AddParameter(LPCTSTR pszItem, LPCWSTR pszString, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInput);
	//插入参数
	VOID AddParameter(LPCTSTR pszItem, SYSTEMTIME & SystemTime, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInput);
	//插入参数
	VOID AddParameterOutput(LPCTSTR pszItem, LPSTR pszString, UINT uSize, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInputOutput);
	//插入参数
	VOID AddParameterOutput(LPCTSTR pszItem, LPWSTR pszString, UINT uSize, ADOCG::ParameterDirectionEnum ParameterDirection=adParamInputOutput);

	//执行辅助
public:
	//返回数值
	LONG GetReturnValue();
	//存储过程
	LONG ExecuteProcess(LPCTSTR pszSPName, bool bRecordset);
};

//////////////////////////////////////////////////////////////////////////////////

#endif