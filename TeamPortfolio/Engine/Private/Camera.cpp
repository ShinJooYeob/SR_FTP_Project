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

	m_bIsOrtho = true;

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
	

	if (FAILED(Set_ProjectMatrix(m_bIsOrtho)))
		return E_FAIL;


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

HRESULT CCamera::Camera_Shaking(_float fDeltaTime, _float fTotalEftFrame)
{
	if (!IsShaking) {

		IsShaking = true;

		m_fTotalEftFrame = fTotalEftFrame;
		m_fTempDeltaTime = fDeltaTime;
		if (FAILED(GetSingle(CGameInstance)->PlayThread(ShakeThread, this)))
			return E_FAIL;
	}
	return S_OK;
}

void CCamera::ShakeFunction(_bool * IsClientQuit, CRITICAL_SECTION * _CriSec)
{

	_float3 vShakeDir;
	DWORD dwSleepTime = DWORD(m_fTempDeltaTime * 1000);
	_uint TotalCnt = (_uint)((m_fTotalEftFrame / m_fTempDeltaTime) * 0.25f);

	_float3 TotalMovement = _float3(0, 0, 0);
	TotalCnt *= 2;

	for (_uint i = 0; i < TotalCnt; i++)
	{
		EnterCriticalSection(_CriSec);
		switch (i % 2)
		{
		case 0:
			vShakeDir = _float3(0, 0, 0);
			vShakeDir += m_pTransform->Get_MatrixState(CTransform::STATE_RIGHT)* ((rand() % 100) - 50.f);
			vShakeDir += m_pTransform->Get_MatrixState(CTransform::STATE_UP)* ((rand() % 100) - 50.f);
			m_pTransform->MovetoDir(vShakeDir, m_fTempDeltaTime);
			TotalMovement += vShakeDir;
			break;

		case 1:
			m_pTransform->MovetoDir(vShakeDir*-1.f, m_fTempDeltaTime);
			TotalMovement += vShakeDir*-1.f;
			break;

		default:
			break;
		}

		LeaveCriticalSection(_CriSec);

		Sleep(dwSleepTime);
	}



	EnterCriticalSection(_CriSec);
	if (TotalMovement != _float3(0, 0, 0))
		m_pTransform->MovetoDir(TotalMovement, m_fTempDeltaTime);
	IsShaking = false;
	LeaveCriticalSection(_CriSec);
}

HRESULT CCamera::Set_ProjectMatrix(_bool bIsOrtho)
{
	_Matrix matProj;

	if (bIsOrtho)
		D3DXMatrixOrthoLH(&matProj, m_fOrthoZoomInOut * m_CameraDesc.fAspect, m_fOrthoZoomInOut, -m_CameraDesc.fFar, m_CameraDesc.fFar);

	else
		D3DXMatrixPerspectiveFovLH(&matProj, m_CameraDesc.fFovy*m_fOrthoZoomInOut/16.f, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);


	if (FAILED(m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &matProj)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CCamera::Set_NewLookPoint(_float3 lookat)
{

	if (m_pGraphicDevice == nullptr)
		return E_FAIL;

	if (nullptr == m_pTransform)
		return E_FAIL;

	m_CameraDesc.vAt = lookat;
	m_CameraDesc.vEye = m_pTransform->Get_MatrixState(CTransform::STATE_POS);

	_float3 vRight, vUp, vLook;

	vLook = (m_CameraDesc.vAt - m_CameraDesc.vEye).Get_Nomalize();

	vRight = m_CameraDesc.vAxisY.Get_Cross(vLook).Get_Nomalize();

	vUp = vLook.Get_Cross(vRight).Get_Nomalize();

	m_pTransform->Set_MatrixState(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_MatrixState(CTransform::STATE_UP, vUp);
	m_pTransform->Set_MatrixState(CTransform::STATE_LOOK, vLook);
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
}
