#include "stdafx.h"
#include "DataBaseException.h"

//���캯��
CDataBaseException::CDataBaseException()
{
	//��������
	m_hResult = S_OK;
	m_SQLException = SQLException_None;

	return;
}

//��������
CDataBaseException::~CDataBaseException()
{
}

//�ӿڲ�ѯ
VOID * CDataBaseException::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseException, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseException, Guid, dwQueryVer);
	return NULL;
}

//���ô���
VOID CDataBaseException::SetExceptionInfo(enSQLException SQLException, CComError * pComError)
{
	//Ч�����
	//assert(pComError != NULL);
	if (pComError == NULL) return;

	//������Ϣ
	m_SQLException = SQLException;
	m_hResult = pComError->Error();
	m_strDescribe.Format(TEXT("���ݿ��쳣��%s [ 0x%8x ]"), (LPCTSTR)(pComError->Description()), pComError->Error());

	return;
}