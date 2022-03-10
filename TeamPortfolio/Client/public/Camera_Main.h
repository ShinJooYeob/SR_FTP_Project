#pragma once

#include "Camera.h"
#include "MiniMapUI.h"

BEGIN(Client)

class CCamera_Main :public CCamera
{
public:

	enum CameraLookStateID
	{
		Look_Left_Axis,
		Look_Back_Axis,
		Look_Right_Axis,
		Look_Front_Axis
	};

	enum CameraEffectID
	{
		CAM_EFT_FADE_IN,
		CAM_EFT_FADE_OUT,
		CAM_EFT_HIT,

		CAM_EFT_SHAKE,
		CAM_EFT_ACTION,
		CAM_EFT_VICTORY,
		CAM_EFT_END
	};

private:
	explicit CCamera_Main(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CCamera_Main(const CCamera_Main&  rhs);
	virtual ~CCamera_Main() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;
	virtual HRESULT ReInitialize(_float3* ActionPointArr, _uint iArrSize);
	void Set_VictoryTurnAxis(_float3 vVictoryTurnAxis, _float3 vCamPivot) { m_VictoryTurnAxis = vVictoryTurnAxis; m_vCamPivot = vCamPivot; };

	void		CameraEffect(CameraEffectID eEffect,_float fTimeDelta, _float fTotalFrame = 1.f);




public:
	HRESULT Revolution_Turn_AxisY_CW(_float3 vRevPos, _float fTimeDelta);
	HRESULT Revolution_Turn_AxisY_CCW(_float3 vRevPos, _float fTimeDelta);

	HRESULT			Reset_LookAtAxis(void* pArg, CameraLookStateID LookDir = Look_Front_Axis);
public:
	_bool	Get_bIsTuring() { return m_IsTurning; };
	CameraLookStateID	Get_CameraLookState() { return (CameraLookStateID)m_eLoookState; };

	

public:
	CameraEffectID Get_EffectID() {	return m_eEffectID;}
	void FadeIn(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec);
	void FadeOut(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec);
	void CamShake(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec);
	void HitEft(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec);
	void CamAction(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec);
	void CamViectoryEft(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec);

private:
	PARTICLEDESC				m_ParticleDesc;

	_int						m_eLoookState = Look_Front_Axis;
	_float						m_fPassedTime = 0;
	_float						m_fStartAngle = 0;
	_float						m_fTargetAngle = 0;
	_bool						m_IsTurning = false;
	_bool						m_bMinimapStart = false;

	CTexture*				m_ComTexture = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;

	CMiniMapUI*				m_MiniMapUI = nullptr;
	_float					m_MapOpenTime= 0;
	

	CameraEffectID			m_eEffectID = CAM_EFT_END;
	_float					m_fTotalEftFrame = 0;
	_uint					m_ARGB[4] = { 0 };
	_float					m_fTimeDelta = 0;
	_Matrix					m_CamEffectMatricx;
	_float					m_fZoomInTime = 0;
	_float3*				m_ActionTargetPos = nullptr;

	_float3					m_VictoryTurnAxis = NOT_EXIST_BLOCK;
	_float3					m_vCamPivot = NOT_EXIST_BLOCK;
	_uint					m_iActionArrSize = 0;
private:
	HRESULT			SetUp_DefaultLookAtAxis(void* pArg);
	HRESULT			SetUp_Components();

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	HRESULT Input_Keyboard(_float fDeltaTime);

public:
	static CCamera_Main* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;
};

END