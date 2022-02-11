#pragma once

#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:	
	typedef struct tagCameraDesc
	{
		_bool		bIsOrtho;
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

public:
	HRESULT Camera_Shaking(_float fDeltaTime);
	void ShakeFunction(_bool* IsClientQuit, CRITICAL_SECTION* _CriSec);
	
	void Change_Camera_Demension() { bIsOrtho = !bIsOrtho; };


protected:
	_bool						bIsOrtho = false;
	CTransform*					m_pTransform = nullptr;
	CAMERADESC					m_CameraDesc;

private:
	_float						m_fTempDeltaTime = 0;
	bool						IsShaking = false;
public:
	virtual CGameObject* Clone(void* pArg)PURE;
	virtual void Free()override;

};

END