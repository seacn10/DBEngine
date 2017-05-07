// KernelEngine_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "..\DBEngine\DataBaseEngine.h"
#include "..\DBEngine\DataBaseAide.h"

#pragma comment(lib, "DBEngine.lib")

#define SCORE						LONGLONG							//积分类型
//分销积分
struct DBR_GR_WriteGameDistriScore
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	SCORE							lParentScore;						//上级积分
	SCORE							lPParentScore;						//上上级积分
};

struct tagDataBaseParameter
{
	WORD							wDataBasePort;						//数据库端口
	TCHAR							szDataBaseAddr[32];					//数据库地址
	TCHAR							szDataBaseUser[32];					//数据库用户
	TCHAR							szDataBasePass[32];					//数据库密码
	TCHAR							szDataBaseName[32];					//数据库名字
};

//配置信息
struct DBO_GR_GameParameter
{
	//汇率信息
	DWORD							dwMedalRate;						//奖牌汇率
	DWORD							dwRevenueRate;						//银行税收
	DWORD							dwExchangeRate;						//兑换比率
	DWORD							dwPresentExchangeRate;				//魅力游戏币兑换率
	DWORD							dwRateGold;							//游戏豆游戏币兑换率

	//经验奖励
	DWORD							dwWinExperience;					//赢局经验

	//版本信息
	DWORD							dwClientVersion;					//客户版本
	DWORD							dwServerVersion;					//服务版本

	DWORD							dwDistributionRate1;				// 上级分销比率
	DWORD							dwDistributionRate2;				// 上上级分销比率

	SCORE							lEducateGrantScore;					//练习赠送
};

CDataBaseAide					m_GameDBAide;					//数据库
CDataBaseHelper					m_GameDBModule;					//数据库

bool OnRequestWriteGameDistriScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//请求处理
	DBR_GR_WriteGameDistriScore * pWriteGameDistriScore = (DBR_GR_WriteGameDistriScore *)pData;
	if (pWriteGameDistriScore == NULL)
		return false;

	dwUserID = pWriteGameDistriScore->dwUserID;

	try
	{
		//效验参数
		ASSERT(wDataSize == sizeof(DBR_GR_WriteGameDistriScore));
		if (wDataSize != sizeof(DBR_GR_WriteGameDistriScore)) return false;

		//构造参数
		m_GameDBAide.ResetParameter();

		//用户信息
		m_GameDBAide.AddParameter(TEXT("@dwUserID"), pWriteGameDistriScore->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@lPDistributionScore"), pWriteGameDistriScore->lParentScore);
		m_GameDBAide.AddParameter(TEXT("@lPPDistributionScore"), pWriteGameDistriScore->lPParentScore);

		//执行查询
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
		//变量定义
		DBO_GR_GameParameter GameParameter;
		ZeroMemory(&GameParameter, sizeof(GameParameter));

		//执行查询
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"), 300);
		m_GameDBAide.AddParameter(TEXT("@wServerID"), 300);


		//执行查询
		LONG lResultCode = m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadParameter"), true);

		//读取信息
		if (lResultCode == 0)
		{
			//汇率信息
			GameParameter.dwMedalRate = m_GameDBAide.GetValue_DWORD(TEXT("MedalRate"));
			GameParameter.dwRevenueRate = m_GameDBAide.GetValue_DWORD(TEXT("RevenueRate"));
			GameParameter.dwExchangeRate = m_GameDBAide.GetValue_DWORD(TEXT("ExchangeRate"));
			GameParameter.dwPresentExchangeRate = m_GameDBAide.GetValue_DWORD(TEXT("PresentExchangeRate"));
			GameParameter.dwRateGold = m_GameDBAide.GetValue_DWORD(TEXT("RateGold"));

			//经验奖励
			GameParameter.dwWinExperience = m_GameDBAide.GetValue_DWORD(TEXT("WinExperience"));
			//版本信息
			GameParameter.dwClientVersion = m_GameDBAide.GetValue_DWORD(TEXT("ClientVersion"));
			GameParameter.dwServerVersion = m_GameDBAide.GetValue_DWORD(TEXT("ServerVersion"));

			//分销比例
			GameParameter.dwDistributionRate1 = m_GameDBAide.GetValue_DWORD(TEXT("DistributionRate1"));
			GameParameter.dwDistributionRate2 = m_GameDBAide.GetValue_DWORD(TEXT("DistributionRate2"));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
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

	tagDataBaseParameter *m_DBParameter = new tagDataBaseParameter;		//连接地址
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

	// 调用写数据
	DWORD dwUserID = 0;
	OnRequestWriteGameDistriScore(0, &WriteGameDistriScore, sizeof(DBR_GR_WriteGameDistriScore), dwUserID);

	// 调用读数据
	OnRequestLoadParameter(0, NULL, 0, dwUserID);

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestDatabase();

	return 0;
}

