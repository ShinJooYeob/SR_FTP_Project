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

HRESULT CLoginMgr::LogIn_to_AWS_DB(wstring szID, wstring szPassward, _bool * IsbOutLoginSuccess, wstring * pOutPlayerData)
{
	return S_OK;
}

HRESULT CLoginMgr::Save_PlayerData(CPlayer * pPlayer, _bool * IsbOutLoginSuccess)
{
	*IsbOutLoginSuccess = false;
	if (pPlayer == nullptr)
		return E_FAIL;

	LOGINDESC tLoginDesc = {};

	pPlayer->Get_PlayerLoginDesc(&tLoginDesc);

	wstring TempString = L"";

	TempString += tLoginDesc.szID + L"|";
	TempString += tLoginDesc.szPassword + L"|";


	for (_uint i = 0; i<SCENE_END; i++)
	{
		_tchar szbuf[MAX_PATH] = L"";

		_itow_s(tLoginDesc.iArrStageBestClear[i], szbuf, 10);

		TempString += wstring(szbuf) + L"|";
	}

	for (_uint i = 0; i < QUEST_END; i++)
	{
		_tchar szbuf[MAX_PATH] = L"";

		_itow_s(tLoginDesc.iArrQuestProgress[i], szbuf, 10);

		TempString += wstring(szbuf) + L"|";
	}

	for (_uint i = 0; i < SKILL_END; i++)
	{
		_tchar szbuf[MAX_PATH] = L"";

		_itow_s(tLoginDesc.iArrSkillLevel[i], szbuf, 10);

		TempString += wstring(szbuf) + L"|";
	}


	_tchar szbuf[MAX_PATH] = L"";

	_itow_s(tLoginDesc.iGold, szbuf, 10);

	TempString += wstring(szbuf) + L"|";


	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	*IsbOutLoginSuccess = true;
	return S_OK;
}

HRESULT CLoginMgr::Load_PlayerData(wstring szID, wstring szPassward, _bool * IsbOutLoginSuccess)
{



	return S_OK;
}

void CLoginMgr::Free()
{
}

