#pragma once

#include "UI.h"


BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
END

class CUI_Loading final :public CUI
{
private:
	explicit CUI_Loading(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_Loading(const CUI_Loading& rhs);
	virtual ~CUI_Loading() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;

	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;


private:
	HRESULT			SetUp_Components();

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	CUI * Find_UI(const _tchar * tagUI);
	CUI * Find_Image(const _tchar * tagUIList);
	HRESULT Ready_Layer_UI_Image(const _tchar * pLayerTag);

private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;

private:
	_float			m_fFrame = 0.f;
	_float4			m_vUIDesc;

	map<const _tchar*, CUI*>			m_UIList;
	map<const _tchar*, CUI*>			m_UIPrototypes;

public:
	static CUI_Loading* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;
};

