#pragma once

#include "Camera.h"

// 툴용 카메라
class CCamera_Tool :public CCamera
{
private:
	explicit CCamera_Tool(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CCamera_Tool(const CCamera_Tool&  rhs);
	virtual ~CCamera_Tool() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	// 각각 시작위치 이동 위치의 뷰 투영 행렬 세잍ㅇ
	HRESULT Set_StartPosView();
	HRESULT Set_OriginPosView();

public:
//	HRESULT Revolution_Turn_AxisY_CW(_float3 vRevPos, _float fTimeDelta);
//	HRESULT Revolution_Turn_AxisY_CCW(_float3 vRevPos, _float fTimeDelta);

private:
	_float			m_fPassedTime = 0;
	_float			m_fStartAngle = 0;
	_float			m_fTargetAngle = 0;
	_bool			m_IsTurning = false;

	_float3			m_OffSet;
	_Matrix			m_StartMat;
	_Matrix			m_NowMat;
public:
	static CCamera_Tool* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;
};

