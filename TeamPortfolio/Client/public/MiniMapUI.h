#pragma once
#include "UI.h"


BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CMiniMapUI :public CUI
{

private:
	explicit CMiniMapUI(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual ~CMiniMapUI() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	HRESULT Reset_MiniMapSize(_float2 vUIDesc);

private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;


private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_RenderState();
	HRESULT			Release_RenderState();

public:
	static CMiniMapUI* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free()override;

	// CUI을(를) 통해 상속됨
};

