#include "stdafx.h"
#include "Player.h"
#include "..\public\LoginMgr.h"


IMPLEMENT_SINGLETON(CLoginMgr);

CLoginMgr::CLoginMgr()
{
}

HRESULT CLoginMgr::Initialize_LoginMgr()
{
	return S_OK;
}

HRESULT CLoginMgr::Create_PlayerData(wstring ID, wstring Passward, _uint * IsbOutLoginSuccess)
{
	*IsbOutLoginSuccess = 0;



	wstring TempString = L"";

	TempString += ID + L"|";
	TempString += Passward + L"|";


	for (_uint i = 0; i < SCENE_END; i++)
	{

		TempString += L"0|";
	}

	for (_uint i = 0; i < QUEST_END; i++)
	{
		TempString += L"0|";
	}

	for (_uint i = 0; i < SKILL_END; i++)
	{
		TempString += L"0|";
	}


	TempString += L"3000";
	m_LoginDesc.iGold = 3000;

	//////////////////////////////////////////////////////////////////////////
	const _tchar*		pGetPath = L"../bin/Resources/LoginData/Player.txt";


	wofstream fout;

	fout.open(pGetPath, ios::app);

	if (!fout.fail())
	{
		fout << TempString << endl;


		m_LoginDesc.szID = ID;
		m_LoginDesc.szPassword = Passward;

		*IsbOutLoginSuccess = 1;
		fout.close();
	}

	//////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CLoginMgr::Load_PlayerData(wstring ID, wstring Passward, _uint * IsbOutLoginSuccess)
{
	wifstream		fin;
	*IsbOutLoginSuccess = 0;

	_tchar	szID[MAX_PATH] = L"";
	_tchar szPadding[MAX_PATH] = L"";

	_tchar	szPassward[64] = L"";

	_tchar	szArrStageBestClear[SCENE_END][64] = {};
	_tchar	szArrQuestProgress[QUEST_END][64] = {};
	_tchar szArrSkillLevel[SKILL_END][64] = {};
	_tchar szGold[64] = L"";

	_tchar FullFilePath[MAX_PATH] = L"../bin/Resources/LoginData/Player.txt";

	fin.open(FullFilePath);

	if (!fin.fail())
	{


		while (true)
		{

			// '|' 단위로 끊어서 문자열 입력 처리

			fin.getline(szID, MAX_PATH, '|');

			if (fin.eof())
				break;

			if (!lstrcmp(ID.c_str(), szID))
			{
				fin.getline(szPassward, MAX_PATH, '|');

				for (_uint i = 0; i < SCENE_END; i++)
				{
					fin.getline((szArrStageBestClear[i]), 64, '|');
				}
				for (_uint i = 0; i < QUEST_END; i++)
				{
					fin.getline((szArrQuestProgress[i]), 64, '|');
				}
				for (_uint i = 0; i < SKILL_END; i++)
				{
					fin.getline((szArrSkillLevel[i]), 64, '|');
				}

				fin.getline(szGold, 64);

				*IsbOutLoginSuccess = 2;
				if (!lstrcmp(Passward.c_str(), szPassward))
				{
					m_LoginDesc.szID = ID;
					m_LoginDesc.szPassword = Passward;

					(*IsbOutLoginSuccess) = 1;
				}

				break;

			}
			else
			{
				fin.getline(szPadding, MAX_PATH);
			}



			if (fin.eof())
				break;
		}

		fin.close();		// close 함수는 생략 가능(객체 타입이어서 소멸 시점에 알아서 개방한 파일 또한 소멸 가능)
	}

	if (*IsbOutLoginSuccess == 1)
	{
		m_LoginDesc.szID = ID;
		m_LoginDesc.szPassword = Passward;


		for (_uint i = 0; i < SCENE_END; i++)
		{
			m_LoginDesc.iArrStageBestClear[i] = _wtoi((szArrStageBestClear[i]));
		}

		for (_uint i = 0; i < QUEST_END; i++)
		{
			m_LoginDesc.iArrQuestProgress[i] = _wtoi((szArrQuestProgress[i]));
		}

		for (_uint i = 0; i < SKILL_END; i++)
		{
			m_LoginDesc.iArrSkillLevel[i] = _wtoi((szArrSkillLevel[i]));
		}

		m_LoginDesc.iGold = _wtoi(szGold);

	}




	return S_OK;
}

HRESULT CLoginMgr::Save_PlayerData(CPlayer * pPlayer, _uint * IsbOutLoginSuccess)
{
	wifstream		fin;
	*IsbOutLoginSuccess = 0;
	LOGINDESC tLoginDesc = {};

	pPlayer->Get_PlayerLoginDesc(&tLoginDesc);

	list<wstring> DataBufferList;
	_bool bIsIDExist = false;


	//자신의 데이터를 제외한 모든 데이터를 buffer에 담아둠
	_tchar FullFilePath[MAX_PATH] = L"../bin/Resources/LoginData/Player.txt";

	fin.open(FullFilePath);

	if (!fin.fail())
	{

		_tchar	szID[MAX_PATH] = L"";
		_tchar szPadding[MAX_PATH] = L"";


		while (true)
		{

			// '|' 단위로 끊어서 문자열 입력 처리

			fin.getline(szID, MAX_PATH, '|');

			if (fin.eof())
				break;

			if (!lstrcmp(tLoginDesc.szID.c_str(), szID))
			{
				fin.getline(szPadding, MAX_PATH);
				bIsIDExist = true;
			}
			else
			{
				fin.getline(szPadding, MAX_PATH);
				DataBufferList.push_back(szID + wstring(L"|") + wstring(szPadding));
			}
			if (fin.eof())
				break;
		}

		fin.close();		// close 함수는 생략 가능(객체 타입이어서 소멸 시점에 알아서 개방한 파일 또한 소멸 가능)
	}





	wofstream fout;

	fout.open(FullFilePath);

	if (!fout.fail())
	{
		auto iter = DataBufferList.begin();
		for (; iter != DataBufferList.end(); iter++)
		{
			fout << (*iter) << endl;
		}

		if (bIsIDExist)//아이디가 존재한다면 저장해라
		{
			wstring Temp = LoginDescToSting(tLoginDesc);
			fout << Temp << endl;
			*IsbOutLoginSuccess = 1;
		}

		fout.close();
	}



	if (!bIsIDExist)////아이디가 없엇다면 새로 만들어라
	{
		_uint IsSuccess = 0;
		Create_PlayerData(tLoginDesc.szID, tLoginDesc.szPassword, &IsSuccess);

		*IsbOutLoginSuccess = 1;
	}


	*IsbOutLoginSuccess = 1;
	return S_OK;

}

HRESULT CLoginMgr::Get_LoginDesc(LOGINDESC * tLoginDesc)
{
	(*tLoginDesc) = m_LoginDesc;
	return S_OK;
}

wstring CLoginMgr::LoginDescToSting(LOGINDESC tLoginDesc)
{
	wstring TempString = L"";

	TempString += tLoginDesc.szID + L"|";
	TempString += tLoginDesc.szPassword + L"|";

	/*
	wstring szID;
	wstring szPassword;

	_int iArrStageBestClear[SCENE_END];
	_int iArrQuestProgress[QUEST_END];
	_int iArrSkillLevel[SKILL_END];
	_int iGold;
	*/
	for (_uint i = 0; i < SCENE_END; i++)
	{
		_tchar buffer[64] = L"";

		_itow_s(tLoginDesc.iArrStageBestClear[i], buffer, 10);

		TempString += wstring(buffer) + L"|";
	}

	for (_uint i = 0; i < QUEST_END; i++)
	{
		_tchar buffer[64] = L"";

		_itow_s(tLoginDesc.iArrQuestProgress[i], buffer, 10);

		TempString += wstring(buffer) + L"|";
	}

	for (_uint i = 0; i < SKILL_END; i++)
	{
		_tchar buffer[64] = L"";

		_itow_s(tLoginDesc.iArrSkillLevel[i], buffer, 10);

		TempString += wstring(buffer) + L"|";
	}


	_tchar buffer[64] = L"";

	_itow_s(tLoginDesc.iGold, buffer, 10);

	TempString += wstring(buffer);

	return TempString;
}




void CLoginMgr::Free()
{
}

