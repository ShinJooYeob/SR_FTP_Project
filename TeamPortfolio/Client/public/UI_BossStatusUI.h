#pragma once


#include "UI.h"


BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END

BEGIN(Client)

class CUI_BossStatusUI final :public CUI
{

private:
	explicit CUI_BossStatusUI(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_BossStatusUI(const CUI_BossStatusUI& rhs);
	virtual ~CUI_BossStatusUI() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

	HRESULT Set_Player(CGameObject* pPlayer);
	HRESULT Set_ResultUI(CGameObject* pResult);

	_bool	Get_bCanHit() { return (m_bIsStageEnd)? (!m_bIsStageEnd ):(!m_bVersusPointChange); }

	void	Change_VersusPoint(_float vChangePoint);
	_float  Get_TargetPotint() const { return m_fTargetPoint; }


private:
	CTexture*				m_ComTexture = nullptr;
	CTexture*				m_ComTexture_Player = nullptr;
	CTexture*				m_ComTexture_Boss = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;


	

private:
	_float4			m_vUIDesc[4];
	RECT			m_vBarRect[3];


	_float			m_fTotalVersusPoint = 10;
	_float			m_fNowVersusPoint;

	_bool			m_bIsStageEnd = false;

	_bool			m_PlayerPictureClicked = false;
	_bool			m_BossPictureClicked = false;


	_bool			m_bVersusPointChange = false;

	_float			m_fStartPoint = 0;
	_float			m_fTargetPoint = 0;
	_float			m_fPassedTime = 0;
	_float			m_fEasingPoint = 0;



	_float3			m_StartColor = _float3(255, 255, 255);
	_float3			m_TargetColor = _float3(255, 255, 255);
	_float3			m_NowColor = _float3(255, 255, 255);
	_float			m_fTimeBarPassedTime = 0;



	class CUI_Result*		m_pResult = nullptr;

	class CPlayer*	m_pPlayer = nullptr;
	class CBossMonster*	m_pBoss = nullptr;

private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_UIDesc();
	HRESULT			Update_MouseButton(_float fTimeDelta);
	HRESULT			Update_Animation(_float fTimeDelta);


	HRESULT			Zero_SetUp_RenderState();
	HRESULT			First_SetUp_RenderState();
	HRESULT			Second_SetUp_RenderState();
	HRESULT			Release_RenderState();

	RECT			TransUIDesc_to_Rect(_float4 UIDesc);
	_float4			TransRect_to_UIDesc(RECT UIDesc);

	_float3			EaseingFloat3(EasingTypeID id, _float3 StartPos, _float3 EndPos, _float curTime, _float maxTime);

public:
	static CUI_BossStatusUI* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

END