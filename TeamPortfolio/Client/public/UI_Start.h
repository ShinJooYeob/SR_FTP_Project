#pragma once


#include "UI.h"


BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CUI_Start final :public CUI
{

private:
	explicit CUI_Start(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_Start(const CUI_Start& rhs);
	virtual ~CUI_Start() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;


	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

	HRESULT First_SetUp_RenderState();
	HRESULT Second_SetUp_RenderState();


	HRESULT Release_RenderState();

public:
	void			Set_UI_TransformRect(_float4 vRect);

private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;



private:
	_float4			m_vUIDesc[3];
	_float			m_fCountFrame = 0;

	_bool			m_bStarton = false;

	_float			m_fStartPoint = 0;
	_float			m_fTargetPoint = 0;
	_float			m_fPassedTime = 0;
	_float			m_fOldframe = -1.f;
	_bool			m_bSound = false;

private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_UIDesc();
	HRESULT			Update_MouseButton(_float fTimeDelta);
	HRESULT			Update_Animation(_float fTimeDelta);
	RECT			TransUIDesc_to_Rect(_float4 UIDesc);

public:
	static CUI_Start* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};