#pragma once

#include "UI.h"


BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
END

class CUI_Loding final :public CUI
{
private:
	explicit CUI_Loding(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_Loding(const CUI_Loding& rhs);
	virtual ~CUI_Loding() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;

	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;


public:
	HRESULT Ready_Layer_UI_Image(const _tchar * pLayerTag);

private:
	HRESULT			SetUp_Components();

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;

private:	_float			m_fFrame = 0.f;
			_float4			m_vUIDesc;

public:
	static CUI_Loding* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;
};

