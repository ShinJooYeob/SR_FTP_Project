#pragma once


#include "UI.h"


BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CUI_Complete final :public CUI
{

private:
	explicit CUI_Complete(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_Complete(const CUI_Complete& rhs);
	virtual ~CUI_Complete() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;


	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

	HRESULT First_SetUp_RenderState();


	HRESULT Release_RenderState();


private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;



private:
	_float4			m_vUIDesc;
	_uint			m_iAlpha = 0;
	_float			m_fCountFrame = 0;

	_bool			m_bStarton = false;
	_bool			m_bStartTime = false;

	_float			m_fStartPoint = 0;
	_float			m_fTargetPoint = 0;
	_float			m_fPassedTime = 0;

private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_UIDesc();
	HRESULT			Update_Animation(_float fTimeDelta);

public:
	static CUI_Complete* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};