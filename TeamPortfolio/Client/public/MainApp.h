#pragma once

#include "Base.h"

class CMainApp final :public CBase
{
public:
	explicit CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT		Initialize(); 
	_int		Update(_float fDeltaTime);
	HRESULT		Render();



private:
	LPDIRECT3DDEVICE9		m_pGraphicDevice = nullptr;
	CGameInstance*			m_pGameInstance = nullptr;
	CRenderer*				m_pComRenderer = nullptr;

private:
	HRESULT Scene_Change(SCENEID eSceneID);
	HRESULT Default_Setting();

	HRESULT Ready_Static_Component_Prototype();
	HRESULT Ready_Static_GameObject_Prototype();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

