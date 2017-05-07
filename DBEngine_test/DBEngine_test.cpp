// KernelEngine_test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "..\DBEngine\DataBaseEngine.h"
#include "..\DBEngine\DataBaseAide.h"

#pragma comment(lib, "DBEngine.lib")

#define SCORE						LONGLONG							//��������
//��������
struct DBR_GR_WriteGameDistriScore
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lParentScore;						//�ϼ�����
	SCORE							lPParentScore;						//���ϼ�����
};

struct tagDataBaseParameter
{
	WORD							wDataBasePort;						//���ݿ�˿�
	TCHAR							szDataBaseAddr[32];					//���ݿ��ַ
	TCHAR							szDataBaseUser[32];					//���ݿ��û�
	TCHAR							szDataBasePass[32];					//���ݿ�����
	TCHAR							szDataBaseName[32];					//���ݿ�����
};

//������Ϣ
struct DBO_GR_GameParameter
{
	//������Ϣ
	DWORD							dwMedalRate;						//���ƻ���
	DWORD							dwRevenueRate;						//����˰��
	DWORD							dwExchangeRate;						//�һ�����
	DWORD							dwPresentExchangeRate;				//������Ϸ�Ҷһ���
	DWORD							dwRateGold;							//��Ϸ����Ϸ�Ҷһ���

	//���齱��
	DWORD							dwWinExperience;					//Ӯ�־���

	//�汾��Ϣ
	DWORD							dwClientVersion;					//�ͻ��汾
	DWORD							dwServerVersion;					//����汾

	DWORD							dwDistributionRate1;				// �ϼ���������
	DWORD							dwDistributionRate2;				// ���ϼ���������

	SCORE							lEducateGrantScore;					//��ϰ����
};

CDataBaseAide					m_GameDBAide;					//���ݿ�
CDataBaseHelper					m_GameDBModule;					//���ݿ�

bool OnRequestWriteGameDistriScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//������
	DBR_GR_WriteGameDistriScore * pWriteGameDistriScore = (DBR_GR_WriteGameDistriScore *)pData;
	if (pWriteGameDistriScore == NULL)
		return false;

	dwUserID = pWriteGameDistriScore->dwUserID;

	try
	{
		//Ч�����
		ASSERT(wDataSize == sizeof(DBR_GR_WriteGameDistriScore));
		if (wDataSize != sizeof(DBR_GR_WriteGameDistriScore)) return false;

		//�������
		m_GameDBAide.ResetParameter();

		//�û���Ϣ
		m_GameDBAide.AddParameter(TEXT("@dwUserID"), pWriteGameDistriScore->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@lPDistributionScore"), pWriteGameDistriScore->lParentScore);
		m_GameDBAide.AddParameter(TEXT("@lPPDistributionScore"), pWriteGameDistriScore->lPParentScore);

		//ִ�в�ѯ
		LONG lResultCode = m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_WriteGameDistributionScore"), false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		LPCTSTR pszDescribe = pIException->GetExceptionDescribe();

		return false;
	}

	return true;
}

bool OnRequestLoadParameter(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//��������
		DBO_GR_GameParameter GameParameter;
		ZeroMemory(&GameParameter, sizeof(GameParameter));

		//ִ�в�ѯ
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"), 300);
		m_GameDBAide.AddParameter(TEXT("@wServerID"), 300);


		//ִ�в�ѯ
		LONG lResultCode = m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadParameter"), true);

		//��ȡ��Ϣ
		if (lResultCode == 0)
		{
			//������Ϣ
			GameParameter.dwMedalRate = m_GameDBAide.GetValue_DWORD(TEXT("MedalRate"));
			GameParameter.dwRevenueRate = m_GameDBAide.GetValue_DWORD(TEXT("RevenueRate"));
			GameParameter.dwExchangeRate = m_GameDBAide.GetValue_DWORD(TEXT("ExchangeRate"));
			GameParameter.dwPresentExchangeRate = m_GameDBAide.GetValue_DWORD(TEXT("PresentExchangeRate"));
			GameParameter.dwRateGold = m_GameDBAide.GetValue_DWORD(TEXT("RateGold"));

			//���齱��
			GameParameter.dwWinExperience = m_GameDBAide.GetValue_DWORD(TEXT("WinExperience"));
			//�汾��Ϣ
			GameParameter.dwClientVersion = m_GameDBAide.GetValue_DWORD(TEXT("ClientVersion"));
			GameParameter.dwServerVersion = m_GameDBAide.GetValue_DWORD(TEXT("ServerVersion"));

			//��������
			GameParameter.dwDistributionRate1 = m_GameDBAide.GetValue_DWORD(TEXT("DistributionRate1"));
			GameParameter.dwDistributionRate2 = m_GameDBAide.GetValue_DWORD(TEXT("DistributionRate2"));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe = pIException->GetExceptionDescribe();

		return false;
	}

	return true;
}

bool TestDatabase()
{
	if ((m_GameDBModule.GetInterface() == NULL) && (m_GameDBModule.CreateInstance() == false))
	{
		return false;
	}

	tagDataBaseParameter *m_DBParameter = new tagDataBaseParameter;		//���ӵ�ַ
	if (m_DBParameter == NULL)
	{
		return false;
	}

	ZeroMemory(m_DBParameter, sizeof(m_DBParameter));
	m_DBParameter->wDataBasePort = 1433;
	_tcscpy_s(m_DBParameter->szDataBaseAddr, CountArray(m_DBParameter->szDataBaseAddr), L"127.0.0.1");
	_tcscpy_s(m_DBParameter->szDataBaseUser, CountArray(m_DBParameter->szDataBaseUser), L"sa");
	_tcscpy_s(m_DBParameter->szDataBasePass, CountArray(m_DBParameter->szDataBasePass), L"password1");
	_tcscpy_s(m_DBParameter->szDataBaseName, CountArray(m_DBParameter->szDataBaseName), L"THTreasureDB");

	m_GameDBModule->SetConnectionInfo(m_DBParameter->szDataBaseAddr, m_DBParameter->wDataBasePort,
		m_DBParameter->szDataBaseName, m_DBParameter->szDataBaseUser, m_DBParameter->szDataBasePass);

	m_GameDBModule->OpenConnection();
	m_GameDBAide.SetDataBase(m_GameDBModule.GetInterface());

	DBR_GR_WriteGameDistriScore WriteGameDistriScore = { 0 };
	WriteGameDistriScore.dwUserID = 8;
	WriteGameDistriScore.lParentScore = 20000;
	WriteGameDistriScore.lPParentScore = 30000;

	// ����д����
	DWORD dwUserID = 0;
	OnRequestWriteGameDistriScore(0, &WriteGameDistriScore, sizeof(DBR_GR_WriteGameDistriScore), dwUserID);

	// ���ö�����
	OnRequestLoadParameter(0, NULL, 0, dwUserID);

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestDatabase();

	return 0;
}

