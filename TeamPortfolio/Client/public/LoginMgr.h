#pragma once

#include "Base.h"

BEGIN(Client)
class CPlayer;

class CLoginMgr :public CBase
{
	DECLARE_SINGLETON(CLoginMgr);


public:

private:
	explicit CLoginMgr();
	virtual ~CLoginMgr() = default;


public:
	HRESULT Initialize_LoginMgr();

public:
	HRESULT Create_PlayerData(wstring ID, wstring Passward, _uint* IsbOutLoginSuccess);
	HRESULT Load_PlayerData(wstring ID, wstring Passward, _uint* IsbOutLoginSuccess);
	HRESULT Save_PlayerData(CPlayer* pPlayer, _uint* IsbOutLoginSuccess);


	HRESULT Get_LoginDesc(LOGINDESC* tLoginDesc);

private:
	wstring LoginDescToSting(LOGINDESC tLoginDesc);

private:
	LOGINDESC m_LoginDesc;


public:
	virtual void Free() override;
};


END