#pragma once


#include "UI.h"


BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CUI_BossStageEntryUI final :public CUI
{

private:
	explicit CUI_BossStageEntryUI(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_BossStageEntryUI(const CUI_BossStageEntryUI& rhs);
	virtual ~CUI_BossStageEntryUI() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;


	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

	HRESULT First_SetUp_RenderState();
	HRESULT Second_SetUp_RenderState();
	HRESULT Third_SetUp_RenderState();
	HRESULT Release_RenderState();


public:
	void			Set_UI_TransformRect(_float4 vRect);

private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;


private:
	_float4			m_vUIDesc[5];

	_float			m_fFrameTime = 0;


	_float4			m_vBackFadeColor = _float4(255.f, 219.f, 0, 20.f);
	_float3			m_vMarkFadeTimes = _float3(0, 0, 0);


	_bool			m_bIsEndEntry = false;
	_float			m_fFadeOutFrame = 240.f;

private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_UIDesc();

	HRESULT			Update_Animation(_float fTimeDelta);
	RECT			TransUIDesc_to_Rect(_float4 UIDesc);

public:
	static CUI_BossStageEntryUI* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};