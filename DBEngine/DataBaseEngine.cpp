#include "stdafx.h"
#include "DataBaseEngine.h"

#include <time.h>
#include <afxcom_.h>

//�궨��
_COM_SMARTPTR_TYPEDEF(IADORecordBinding,__uuidof(IADORecordBinding));

#define EfficacyResult(hResult) { if (FAILED(hResult)) _com_issue_error(hResult); }

//////////////////////////////////////////////////////////////////////////

//���캯��
CDataBase::CDataBase() : m_dwResumeConnectCount(10L), m_dwResumeConnectTime(30L)
{
	//״̬����
	m_dwConnectCount = 0L;
	m_dwConnectErrorTime = 0L;

	//��������
	m_DBCommand.CreateInstance(__uuidof(Command));
	m_DBRecordset.CreateInstance(__uuidof(Recordset));
	m_DBConnection.CreateInstance(__uuidof(Connection));

	//Ч������
	if (m_DBCommand == NULL) throw TEXT("���� m_DBCommand ����ʧ��");
	if (m_DBRecordset == NULL) throw TEXT("���� m_DBRecordset ����ʧ��");
	if (m_DBConnection == NULL) throw TEXT("���� m_DBConnection ����ʧ��");

	//���ñ���
	m_DBCommand->CommandType = adCmdStoredProc;

	return;
}

//��������
CDataBase::~CDataBase()
{
	//�ر�����
	CloseConnection();

	//�ͷŶ���
	m_DBCommand.Release();
	m_DBRecordset.Release();
	m_DBConnection.Release();

	return;
}

//�ӿڲ�ѯ
VOID * CDataBase::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBase, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBase, Guid, dwQueryVer);
	return NULL;
}

//������
VOID CDataBase::OpenConnection()
{
	//�������ݿ�
	try
	{
		//�ر�����
		CloseConnection();

		//������
		EfficacyResult(m_DBConnection->Open(_bstr_t(m_strConnect), L"", L"", adConnectUnspecified));

		//���ö���
		m_DBConnection->CursorLocation = adUseClient;
		m_DBCommand->ActiveConnection = m_DBConnection;
	}
	catch (CComError & ComError)
	{
		//���ñ���
		m_dwConnectCount = 0L;
		m_dwConnectErrorTime = (DWORD)time(NULL);

		//�׳��쳣
		OnSQLException(SQLException_Connect, &ComError);
	}

	return;
}

//�ر�����
VOID CDataBase::CloseConnection()
{
	try
	{
		//���ñ���
		m_dwConnectCount = 0L;
		m_dwConnectErrorTime = 0L;

		//�ر�����
		CloseRecordset();
		if ((m_DBConnection != NULL) && (m_DBConnection->GetState() != adStateClosed))
		{
			EfficacyResult(m_DBConnection->Close());
		}
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, &ComError);
	}

	return;
}

//������Ϣ
bool CDataBase::SetConnectionInfo(DWORD dwDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)
{
	//Ч����� 
	ASSERT(szUser != NULL);
	ASSERT(szDBName != NULL);
	ASSERT(szPassword != NULL);

	//��������
	CString strUser;
	CString strDBName;
	CString strPassword;

	//�ַ�ת��
	ConvertToSQLSyntax(szUser, strUser);
	ConvertToSQLSyntax(szDBName, strDBName);
	ConvertToSQLSyntax(szPassword, strPassword);

	//��������
	BYTE * pcbDBAddr = (BYTE *)&dwDBAddr;
	m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%d.%d.%d.%d,%ld;"),
		strPassword, strUser, strDBName, pcbDBAddr[0], pcbDBAddr[1], pcbDBAddr[2], pcbDBAddr[3], wPort);

	return true;
}

//������Ϣ
bool CDataBase::SetConnectionInfo(LPCTSTR szDBAddr, WORD wPort, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPassword)
{
	//Ч����� 
	ASSERT(szUser != NULL);
	ASSERT(szDBName != NULL);
	ASSERT(szDBAddr != NULL);
	ASSERT(szPassword != NULL);

	//��������
	CString strUser;
	CString strDBName;
	CString strPassword;

	//�ַ�ת��
	ConvertToSQLSyntax(szUser, strUser);
	ConvertToSQLSyntax(szDBName, strDBName);
	ConvertToSQLSyntax(szPassword, strPassword);

	//��������
	m_strConnect.Format(TEXT("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s,%ld;"),
		strPassword, strUser, strDBName, szDBAddr, wPort);

	return true;
}

//�������
VOID CDataBase::ClearParameters()
{
	try
	{
		LONG lParameterCount = m_DBCommand->Parameters->Count;
		if (lParameterCount > 0L)
		{
			for (LONG i = lParameterCount; i > 0; i--)
			{
				m_DBCommand->Parameters->Delete(i - 1);
			}
		}
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, &ComError);
	}

	return;
}

//��ȡ����
VOID CDataBase::GetParameter(LPCTSTR pszParamName, CDBVarValue & DBVarValue)
{
	try
	{
		DBVarValue = m_DBCommand->Parameters->Item[pszParamName]->Value;
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, &ComError);
	}

	return;
}

//�������
VOID CDataBase::AddParameter(LPCTSTR pszName, ADOCG::DataTypeEnum Type, ADOCG::ParameterDirectionEnum Direction, LONG lSize, CDBVarValue & DBVarValue)
{
	try
	{
		_ParameterPtr Parameter = m_DBCommand->CreateParameter(pszName, Type, Direction, lSize, DBVarValue);
		m_DBCommand->Parameters->Append(Parameter);
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, &ComError);
	}

	return;
}

//�л���¼
VOID CDataBase::NextRecordset()
{
	try
	{
		CDBVarValue DBVarValue(0L);
		m_DBRecordset->NextRecordset(&DBVarValue);
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, &ComError);
	}

	return;
}

//�󶨶���
VOID CDataBase::BindToRecordset(CADORecordBinding * pBind)
{
	try
	{
		IADORecordBindingPtr RecordBindingPtr(m_DBRecordset);
		RecordBindingPtr->BindToRecordset(pBind);
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, &ComError);
	}

	return;
}

//�����ƶ�
VOID CDataBase::MoveToNext()
{
	try
	{
		m_DBRecordset->MoveNext();
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, &ComError);
	}

	return;
}

//�Ƶ���ͷ
VOID CDataBase::MoveToFirst()
{
	try
	{
		m_DBRecordset->MoveFirst();
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, &ComError);
	}

	return;
}

//�Ƿ����
bool CDataBase::IsRecordsetEnd()
{
	try
	{
		return (m_DBRecordset->EndOfFile == VARIANT_TRUE);
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, &ComError);
	}

	return true;
}

//��ȡ��Ŀ
LONG CDataBase::GetRecordCount()
{
	try
	{
		return m_DBRecordset->GetRecordCount();
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, &ComError);
	}

	return 0;
}

//������ֵ
LONG CDataBase::GetReturnValue()
{
	try
	{
		//��������
		_ParameterPtr Parameter;
		LONG lParameterCount = m_DBCommand->Parameters->Count;

		//��������
		for (LONG i = 0; i < lParameterCount; i++)
		{
			Parameter = m_DBCommand->Parameters->Item[i];
			if (Parameter->Direction == adParamReturnValue) return Parameter->Value.lVal;
		}

		//����Ч��
		ASSERT(FALSE);
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, &ComError);
	}

	return 0L;
}

//�رռ�¼
VOID CDataBase::CloseRecordset()
{
	try
	{
		if (IsRecordsetOpened() == true) EfficacyResult(m_DBRecordset->Close());
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Syntax, &ComError);
	}

	return;
}

//��ȡ����
VOID CDataBase::GetRecordsetValue(LPCTSTR pszItem, CDBVarValue & DBVarValue)
{
	try
	{
		FieldsPtr RecordFields = m_DBRecordset->GetFields();
		DBVarValue = RecordFields->GetItem(pszItem)->GetValue();
	}
	catch (CComError & ComError)
	{
		DBVarValue.Clear();
		OnSQLException(SQLException_Syntax, &ComError);
	}

	return;
}

//�洢����
VOID CDataBase::ExecuteProcess(LPCTSTR pszSPName, bool bRecordset)
{
	//ִ������
	do
	{
		try
		{
			//�رռ�¼
			CloseRecordset();

			//��������
			m_DBCommand->CommandText = pszSPName;
			m_DBRecordset->CursorLocation = adUseClient;

			//ִ������
			if (bRecordset == true)
			{
				m_DBRecordset->PutRefSource(m_DBCommand);
				EfficacyResult(m_DBRecordset->Open((IDispatch *)m_DBCommand, vtMissing, adOpenForwardOnly, adLockReadOnly, adOptionUnspecified));
			}
			else
			{
				EfficacyResult(m_DBCommand->Execute(NULL, NULL, adExecuteNoRecords));
			}

			return;
		}
		catch (CComError & ComError)
		{
			//�����ж�
			if (IsConnectError() == true)
			{
				//��������
				if (TryConnectAgain(&ComError) == false)
				{
					OnSQLException(SQLException_Connect, &ComError);
				}
			}
			else
			{
				//�׳��쳣
				OnSQLException(SQLException_Syntax, &ComError);
			}
		}
	} while (TRUE);

	return;
}

//ִ�����
VOID CDataBase::ExecuteSentence(LPCTSTR pszCommand, bool bRecordset)
{
	//ִ������
	do
	{
		try
		{
			//ִ������
			m_DBCommand->CommandText = L"";
			m_DBConnection->CursorLocation = adUseClient;
			m_DBConnection->Execute(pszCommand, NULL, adExecuteNoRecords);

			return;
		}
		catch (CComError & ComError)
		{
			//�����ж�
			if (IsConnectError() == true)
			{
				//��������
				if (TryConnectAgain(&ComError) == false)
				{
					OnSQLException(SQLException_Connect, &ComError);
				}
			}
			else
			{
				//�׳��쳣
				OnSQLException(SQLException_Syntax, &ComError);
			}
		}
	} while (TRUE);

	return;
}

//���Ӵ���
bool CDataBase::IsConnectError()
{
	try
	{
		//״̬�ж�
		if (m_DBConnection == NULL) return true;
		if (m_DBConnection->GetState() == adStateClosed) return true;

		//�����ж�
		if (m_DBConnection->Errors->Count > 0L)
		{
			//��������
			ErrorPtr pError = NULL;
			LONG lErrorCount = m_DBConnection->Errors->Count;

			//�������
			for (LONG i = 0; i < lErrorCount; i++)
			{
				if (m_DBConnection->Errors->GetItem(i)->Number == E_FAIL)
				{
					//�ر�����
					CloseConnection();

					//���ò���
					m_dwConnectCount = 0L;
					m_dwConnectErrorTime = (DWORD)time(NULL);

					//��ʾ��Ϣ
					//g_TraceServiceManager.TraceString(TEXT("���ݿ������жϣ��Ժ󽫻᳢����������"), TraceLevel_Warning);

					return true;
				}
			}
		}

		return false;
	}
	catch (CComError & ComError)
	{
		OnSQLException(SQLException_Connect, &ComError);
	}

	return false;
}

//�Ƿ��
bool CDataBase::IsRecordsetOpened()
{
	//״̬�ж�
	if (m_DBRecordset == NULL) return false;
	if (m_DBRecordset->GetState() == adStateClosed) return false;

	return true;
}

//��������
bool CDataBase::TryConnectAgain(CComError * pComError)
{
	//���ñ���
	m_dwConnectCount++;

	//�����ж�
	DWORD dwNowTime = (DWORD)time(NULL);
	if ((m_dwConnectCount >= m_dwResumeConnectCount) || ((m_dwConnectErrorTime + m_dwResumeConnectTime) <= dwNowTime))
	{
		try
		{
			//������
			OpenConnection();

			//��ʾ��Ϣ
			//g_TraceServiceManager.TraceString(TEXT("�ɹ������������ݿ�"), TraceLevel_Warning);

			return true;
		}
		catch (IDataBaseException *)
		{
			OnSQLException(SQLException_Connect, pComError);
		}
	}
	else
	{
		OnSQLException(SQLException_Connect, pComError);
	}

	return false;
}

//ת���ַ�
VOID CDataBase::ConvertToSQLSyntax(LPCTSTR pszString, CString & strResult)
{
	//��ȡ����
	INT nSrcLength = lstrlen(pszString);
	LPTSTR pszResult = strResult.GetBufferSetLength(nSrcLength * 2 + 1);

	//�滻�ַ�
	while (*pszString != TEXT('\0'))
	{
		if (*pszString == TEXT('\''))
		{
			*pszResult++ = TEXT('\'');
			*pszResult++ = TEXT('\'');
		}
		else *pszResult++ = *pszString++;
	}

	//�����ַ�
	*pszResult = TEXT('\0');
	strResult.ReleaseBuffer();

	return;
}

//������
VOID CDataBase::OnSQLException(enSQLException SQLException, CComError * pComError)
{
	//�����쳣
	m_DataBaseException.SetExceptionInfo(SQLException, pComError);

	//�׳��쳣
	throw &m_DataBaseException;

	return;
}

//�����������
DECLARE_CREATE_MODULE(DataBase);