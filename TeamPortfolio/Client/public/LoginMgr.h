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
	HRESULT LogIn_to_AWS_DB(wstring szID, wstring szPassward, _bool* IsbOutLoginSuccess, wstring* pOutPlayerData);


	HRESULT Save_PlayerData(CPlayer* pPlayer, _bool* IsbOutLoginSuccess);
	HRESULT Load_PlayerData(wstring szID, wstring szPassward, _bool* IsbOutLoginSuccess);


public:
	virtual void Free() override;
};


END