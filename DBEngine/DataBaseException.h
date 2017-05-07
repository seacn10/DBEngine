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

//���ݿ��쳣
interface IDataBaseException : public IUnknownEx
{
	//�쳣����
	virtual HRESULT GetExceptionResult() = NULL;
	//�쳣����
	virtual LPCTSTR GetExceptionDescribe() = NULL;
	//�쳣����
	virtual enSQLException GetExceptionType() = NULL;
};

//ADO ������
class CDataBaseException : public IDataBaseException
{
	//��������
protected:
	HRESULT							m_hResult;							//�쳣����
	enSQLException					m_SQLException;						//�쳣����

	//��������
protected:
	CString						m_strDescribe;						//�쳣��Ϣ

	//��������
public:
	//���캯��
	CDataBaseException();
	//��������
	virtual ~CDataBaseException();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ܽӿ�
public:
	//�������
	virtual HRESULT GetExceptionResult() { return m_hResult; }
	//��������
	virtual LPCTSTR GetExceptionDescribe() { return m_strDescribe; }
	//��������
	virtual enSQLException GetExceptionType() { return m_SQLException; }

	//���ܺ���
public:
	//���ô���
	VOID SetExceptionInfo(enSQLException SQLException, CComError * pComError);
};

#endif