#pragma once


#include "UI_Result.h"


BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CUI_Status final :public CUI
{

private:
	explicit CUI_Status(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_Status(const CUI_Status& rhs);
	virtual ~CUI_Status() = default;

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

	HRESULT Set_TotalTimerSec(_float iTotalSec);
	HRESULT Set_Player(CGameObject* pPlayer);
	HRESULT Set_ResultUI(CGameObject* pResult);

public:
	void			Set_UI_TransformRect(_float4 vRect);

private:
	CTexture*				m_ComTexture = nullptr;
	CTexture*				m_ComTexture2 = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;

	_float					m_fTotalTimerTime = 0;

private:
	_float4			m_vUIDesc[5];
	_float			m_fWalkFrame = 0;
	_float			m_fHurtedTime = 0;


	_bool			m_bIsStatusChage = false;
	CUI_Result*		m_pResult = nullptr;

	class CPlayer*	m_pPlayer = nullptr;

private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_UIDesc();
	HRESULT			Update_MouseButton(_float fTimeDelta);
	HRESULT			Update_Animation(_float fTimeDelta);
	RECT			TransUIDesc_to_Rect(_float4 UIDesc);

public:
	static CUI_Status* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};