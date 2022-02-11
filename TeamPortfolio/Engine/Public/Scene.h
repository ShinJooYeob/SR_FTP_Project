#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CScene abstract :public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_float fDeltaTime);
	virtual _int LateUpdate(_float fDeltaTime);
	virtual _int Render();
	virtual _int LateRender();

protected:
	LPDIRECT3DDEVICE9 m_pGraphicDevice = nullptr;

public:
	virtual void Free()override;
};

END