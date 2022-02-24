#pragma once

#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:	
	typedef struct tagCameraDesc
	{
		_bool		bIsOrtho =  false;
		_float3		vWorldRotAxis = _float3(0, 0, 0);
		_float3		vEye, vAt, vAxisY;
		_float		fFovy, fAspect, fNear, fFar;
		CTransform::TRANSFORMDESC TransformDesc;
	}CAMERADESC;

protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDevice);
		explicit CCamera(const CCamera&  rhs);
		virtual ~CCamera() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg);
	virtual HRESULT Initialize_Clone(void* pArg);

	virtual _int Update(_float fDeltaTime);
	virtual _int LateUpdate(_float fDeltaTime);
	virtual _int Render();
	virtual _int LateRender();

	_float3 Get_Camera_Position() { return m_pTransform->Get_MatrixState(CTransform::STATE_POS); };
	CTransform*	 Get_Camera_Transform() { return m_pTransform; };

public:
	HRESULT Camera_Shaking(_float fDeltaTime, _float fTotalEftFrame);
	void ShakeFunction(_bool* IsClientQuit, CRITICAL_SECTION* _CriSec);
	
	void Change_Camera_Demension() { m_bIsOrtho = !m_bIsOrtho; };
	HRESULT Set_ProjectMatrix(_bool bIsOrtho);

	HRESULT Set_NewLookPoint(_float3 lookat);

protected:
	_bool						m_bIsOrtho = true;
	CTransform*					m_pTransform = nullptr;
	CAMERADESC					m_CameraDesc;
	_Matrix						m_OrthoMatrix;

	_float						m_fOrthoZoomInOut = 16.f;

private:
	_float						m_fTempDeltaTime = 0;
	_float						m_fTotalEftFrame = 0;
	bool						IsShaking = false;
public:
	virtual CGameObject* Clone(void* pArg)PURE;
	virtual void Free()override;

};

END