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

//���ݿ�ӿ�
interface IDataBase : public IUnknownEx
{
	//���ӽӿ�
public:
	//������
	virtual VOID OpenConnection() = NULL;
	//�ر�����
	virtual VOID CloseConnection() = NULL;
	//������Ϣ
	virtual bool SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword) = NULL;
	//������Ϣ
	virtual bool SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword) = NULL;

	//�����ӿ�
public:
	//�������
	virtual VOID ClearParameters() = NULL;
	//��ȡ����
	virtual VOID GetParameter(LPCTSTR pszParamName, CDBVarValue & DBVarValue) = NULL;
	//�������
	virtual VOID AddParameter(LPCTSTR pszName, ADOCG::DataTypeEnum Type, ADOCG::ParameterDirectionEnum Direction, LONG lSize, CDBVarValue & DBVarValue) = NULL;

	//���ƽӿ�
public:
	//�л���¼
	virtual VOID NextRecordset() = NULL;
	//�رռ�¼
	virtual VOID CloseRecordset() = NULL;
	//�󶨶���
	virtual VOID BindToRecordset(CADORecordBinding * pBind) = NULL;

	//��¼�ӿ�
public:
	//�����ƶ�
	virtual VOID MoveToNext() = NULL;
	//�Ƶ���ͷ
	virtual VOID MoveToFirst() = NULL;
	//�Ƿ����
	virtual bool IsRecordsetEnd() = NULL;
	//��ȡ��Ŀ
	virtual LONG GetRecordCount() = NULL;
	//������ֵ
	virtual LONG GetReturnValue() = NULL;
	//��ȡ����
	virtual VOID GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue) = NULL;

	//���ƽӿ�
public:
	//�洢����
	virtual VOID ExecuteProcess(LPCTSTR pszSPName, bool bRecordset) = NULL;
	//ִ�����
	virtual VOID ExecuteSentence(LPCTSTR pszCommand, bool bRecordset) = NULL;
};

class CDataBase : public IDataBase
{
	//������Ϣ
protected:
	CString							m_strConnect;						//�����ַ�
	CDataBaseException				m_DataBaseException;				//�������

	//״̬����
protected:
	DWORD							m_dwConnectCount;					//���Դ���
	DWORD							m_dwConnectErrorTime;				//����ʱ��
	const DWORD						m_dwResumeConnectTime;				//�ָ�ʱ��
	const DWORD						m_dwResumeConnectCount;				//�ָ�����

	//�ں˱���
protected:
	_CommandPtr						m_DBCommand;						//�������
	_RecordsetPtr					m_DBRecordset;						//��¼����
	_ConnectionPtr					m_DBConnection;						//���ݶ���

	//��������
public:
	//���캯��
	CDataBase();
	//��������
	virtual ~CDataBase();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ӽӿ�
public:
	//������
	virtual VOID OpenConnection();
	//�ر�����
	virtual VOID CloseConnection();
	//������Ϣ
	virtual bool SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);
	//������Ϣ
	virtual bool SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword);

	//�����ӿ�
public:
	//�������
	virtual VOID ClearParameters();
	//��ȡ����
	virtual VOID GetParameter(LPCTSTR pszParamName, CDBVarValue & DBVarValue);
	//�������
	virtual VOID AddParameter(LPCTSTR pszName, ADOCG::DataTypeEnum Type, ADOCG::ParameterDirectionEnum Direction, LONG lSize, CDBVarValue & DBVarValue);

	//���ƽӿ�
public:
	//�л���¼
	virtual VOID NextRecordset();
	//�رռ�¼
	virtual VOID CloseRecordset();
	//�󶨶���
	virtual VOID BindToRecordset(CADORecordBinding * pBind);

	//��¼�ӿ�
public:
	//�����ƶ�
	virtual VOID MoveToNext();
	//�Ƶ���ͷ
	virtual VOID MoveToFirst();
	//�Ƿ����
	virtual bool IsRecordsetEnd();
	//��ȡ��Ŀ
	virtual LONG GetRecordCount();
	//������ֵ
	virtual LONG GetReturnValue();
	//��ȡ����
	virtual VOID GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue);

	//���ƽӿ�
public:
	//�洢����
	virtual VOID ExecuteProcess(LPCTSTR pszSPName, bool bRecordset);
	//ִ�����
	virtual VOID ExecuteSentence(LPCTSTR pszCommand, bool bRecordset);

	//�ڲ�����
protected:
	//���Ӵ���
	bool IsConnectError();
	//�Ƿ��
	bool IsRecordsetOpened();
	//��������
	bool TryConnectAgain(CComError * pComError);
	//ת���ַ�
	VOID ConvertToSQLSyntax(LPCTSTR pszString, CString & strResult);
	//������
	VOID OnSQLException(enSQLException SQLException, CComError * pComError);
};


DECLARE_MODULE_HELPER(DataBase, L"DBEngine.dll", "CreateDataBase")