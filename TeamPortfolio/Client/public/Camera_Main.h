#pragma once

#include "Camera.h"

class CCamera_Main :public CCamera
{
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

public:
	HRESULT Revolution_Turn_AxisY_CW(_float3 vRevPos, _float fTimeDelta);
	HRESULT Revolution_Turn_AxisY_CCW(_float3 vRevPos, _float fTimeDelta);

private:
	_float			m_fPassedTime = 0;
	_float			m_fStartAngle = 0;
	_float			m_fTargetAngle = 0;
	_bool			m_IsTurning = false;

public:
	static CCamera_Main* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;
};

