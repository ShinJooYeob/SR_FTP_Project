#include "..\Public\Camera.h"
#include "GameInstance.h"



_uint CALLBACK ShakeThread(void* _Prameter)
{

	THREADARG tThreadArg{};
	memcpy(&tThreadArg, _Prameter, sizeof(THREADARG));
	delete _Prameter;


	CCamera* pCamera = (CCamera*)tThreadArg.pArg;
	
	pCamera->ShakeFunction(tThreadArg.IsClientQuit, tThreadArg.CriSec);

	return 0;
}



CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

CCamera::CCamera(const CCamera & rhs)
	:CGameObject(rhs)
{
	m_CameraDesc = rhs.m_CameraDesc;
}

HRESULT CCamera::Initialize_Prototype(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	return S_OK;
}

HRESULT CCamera::Initialize_Clone(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	if (m_pGraphicDevice == nullptr)
		return E_FAIL;

	m_pTransform = CTransform::Create(m_pGraphicDevice);

	if (nullptr == m_pTransform)
		return E_FAIL;

	m_pTransform->Set_TransformDesc(m_CameraDesc.TransformDesc);

	_float3 vRight, vUp, vLook;

	vLook = (m_CameraDesc.vAt - m_CameraDesc.vEye).Get_Nomalize();

	vRight = m_CameraDesc.vAxisY.Get_Cross(vLook).Get_Nomalize();

	vUp = vLook.Get_Cross(vRight).Get_Nomalize();

	m_pTransform->Set_MatrixState(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_MatrixState(CTransform::STATE_UP, vUp);
	m_pTransform->Set_MatrixState(CTransform::STATE_LOOK, vLook);
	m_pTransform->Set_MatrixState(CTransform::STATE_POS, m_CameraDesc.vEye);


	return S_OK;
}

_int CCamera::Update(_float fDeltaTime)
{
	if (m_pGraphicDevice == nullptr)
		return -1;

	if (FAILED(m_pGraphicDevice->SetTransform(D3DTS_VIEW, &(m_pTransform->Get_InverseWorldMatrix()))))
		return -1;

	_Matrix matProj;

	if (bIsOrtho)
		D3DXMatrixOrthoLH(&matProj, 1.f, 1.f / m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	else
		D3DXMatrixPerspectiveFovLH(&matProj, m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	
	if (FAILED(m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &matProj)))
		return -1;




	return _int();
}

_int CCamera::LateUpdate(_float fDeltaTime)
{
	return _int();
}

_int CCamera::Render()
{
	return _int();
}

_int CCamera::LateRender()
{


	return _int();
}

HRESULT CCamera::Camera_Shaking(_float fDeltaTime)
{
	if (!IsShaking) {

		IsShaking = true;

		m_fTempDeltaTime = fDeltaTime;
		if (FAILED(GetSingle(CGameInstance)->PlayThread(ShakeThread, this)))
			return E_FAIL;
	}
	return S_OK;
}

void CCamera::ShakeFunction(_bool * IsClientQuit, CRITICAL_SECTION * _CriSec)
{

	_uint RandInt = rand() % 2;

	for (_uint i = RandInt; i < RandInt+6;i++)
	{
		EnterCriticalSection(_CriSec);

		if (i % 2)
			m_pTransform->Move_Left(m_fTempDeltaTime);
		else
			m_pTransform->Move_Right(m_fTempDeltaTime);

		LeaveCriticalSection(_CriSec);

		Sleep(15);
	}

	EnterCriticalSection(_CriSec);
	IsShaking = false;
	LeaveCriticalSection(_CriSec);
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
}
