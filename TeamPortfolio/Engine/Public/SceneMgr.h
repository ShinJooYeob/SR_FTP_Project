#pragma once
#include "Base.h"

BEGIN(Engine)
class CScene;


class CSceneMgr :public CBase
{
	DECLARE_SINGLETON(CSceneMgr)

private:
	explicit CSceneMgr();
	virtual ~CSceneMgr() = default;


public:
	HRESULT Scene_Chage(CScene * _pScene, _int iNextSceneIndex);

public:
	_int Update(_float fDeltaTime);
	_int LateUpdate(_float fDeltaTime);
	_int Render();
	_int LateRender();

private:
	_int				m_iCurrentSceneIdx = 0;
	CScene*				m_pScene = nullptr;

public:
	virtual void Free()override;
};


END