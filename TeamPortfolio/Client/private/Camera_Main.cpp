#include "stdafx.h"
#include "..\Public\Camera_Main.h"
#include "GameInstance.h"



CCamera_Main::CCamera_Main(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CCamera(pGraphicDevice)
{
}

CCamera_Main::CCamera_Main(const CCamera_Main & rhs)
	:CCamera(rhs)
{
}
//1
HRESULT CCamera_Main::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCamera_Main::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;
	m_vWorldRotAxis = _float3(2.5f, 0, 2.5f);

	return S_OK;
}

_int CCamera_Main::Update(_float fDeltaTime)
{
	CGameInstance* pInstance = GetSingle(CGameInstance);



	if (pInstance->Get_DIKeyState(DIK_W) & DIS_Press)
	{
		m_pTransform->Move_Forward(fDeltaTime);
	}

	if (pInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	{
		m_pTransform->Move_Backward(fDeltaTime);

	}

	if (pInstance->Get_DIKeyState(DIK_A) & DIS_Press)
	{
		m_pTransform->Move_Left(fDeltaTime);
	}

	if (pInstance->Get_DIKeyState(DIK_X) & DIS_Press)
	{
		m_pTransform->MovetoTarget(m_pTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1.f, 0), fDeltaTime);
	}
	if (pInstance->Get_DIKeyState(DIK_Z) & DIS_Press)
	{
		m_pTransform->MovetoTarget(m_pTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1.f, 0), -fDeltaTime);
	}




	if (pInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	{
		m_pTransform->Move_Right(fDeltaTime);
	}
	if (pInstance->Get_DIKeyState(DIK_E) & DIS_Down)
	{

		_float3 vOriginCameraPos = m_pTransform->Get_MatrixState(CTransform::STATE_POS);
		_float3 vCameraPos = vOriginCameraPos - m_vWorldRotAxis;
		vCameraPos.y = 0;

		_float fRadianAngle = acosf(vCameraPos.Get_Nomalize().Get_Dot(_float3(1, 0, 0)));

		if (0 > vCameraPos.z)
			fRadianAngle = 2 * D3DX_PI - fRadianAngle;

		m_fStartAngle = fRadianAngle;
		m_fTargetAngle = fRadianAngle + D3DXToRadian(90);
		m_fPassedTime = 0;
		m_IsTurning = true;

	}
	
	if (pInstance->Get_DIKeyState(DIK_Q) & DIS_Down)
	{
		_float3 vOriginCameraPos = m_pTransform->Get_MatrixState(CTransform::STATE_POS);
		_float3 vCameraPos = vOriginCameraPos - m_vWorldRotAxis;
		vCameraPos.y = 0;

		_float fRadianAngle = acosf(vCameraPos.Get_Nomalize().Get_Dot(_float3(1, 0, 0)));

		if (0 > vCameraPos.z)
			fRadianAngle = 2 * D3DX_PI - fRadianAngle;

		m_fStartAngle = fRadianAngle;
		m_fTargetAngle = fRadianAngle - D3DXToRadian(90);
		m_fPassedTime = 0;
		m_IsTurning = true;

	}
	if (m_IsTurning) {

		Revolution_Turn_AxisY_CW(m_vWorldRotAxis, fDeltaTime);

	}



	if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_DoubleDown)
	{
		Change_Camera_Demension();
	}

	//_long MoveDist;

	//if (MoveDist = pInstance->Get_DIMouseMoveState(CInput_Device::MMS_X))
	//{
	//	m_pTransform->Turn_CW({0,1,0}, fDeltaTime * MoveDist * 0.1f);

	//}
	//if (MoveDist = pInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
	//{
	//	m_pTransform->Turn_CW(m_pTransform->Get_MatrixState(CTransform::STATE_RIGHT), fDeltaTime * MoveDist * 0.1f);
	//}


	__super::Update(fDeltaTime);


	return _int();
}

_int CCamera_Main::LateUpdate(_float fDeltaTime)
{

	return _int();
}

_int CCamera_Main::Render()
{
	return _int();
}

_int CCamera_Main::LateRender()
{
	return _int();
}

HRESULT CCamera_Main::Revolution_Turn_AxisY_CW(_float3 vRevPos, _float fTimeDelta)
{

	m_fPassedTime += fTimeDelta;
	_float3 vOriginCameraPos = m_pTransform->Get_MatrixState(CTransform::STATE_POS);
	_float3 vCameraPos = vOriginCameraPos - vRevPos;
	
	vCameraPos.y = 0;

	_float fRadianAngle = GetSingle(CGameInstance)->TargetQuadIn(m_fStartAngle, m_fTargetAngle, m_fPassedTime);

	_float TempAngle = D3DXToDegree(m_fStartAngle);
	_float TempAngle2 = D3DXToDegree(m_fTargetAngle);
	_float TempAngle3 = D3DXToDegree(fRadianAngle);



	if (m_fPassedTime >= 1.f) 
	{
		m_IsTurning = false;
		m_fStartAngle = m_fTargetAngle;
	}

	_float fDist = vCameraPos.Get_Distance(_float3(0,0,0));

	vCameraPos.x = cosf(fRadianAngle) * fDist;
	vCameraPos.z = sinf(fRadianAngle) * fDist;

	vCameraPos.y = vOriginCameraPos.y;
	vCameraPos += vRevPos;

	m_pTransform->Set_MatrixState(CTransform::STATE_POS, vCameraPos);
	vRevPos.y = vCameraPos.y;
	m_pTransform->LookAt(vRevPos);


	return S_OK;


	//_float fRadianAngle = acosf(vCameraPos.Get_Nomalize().Get_Dot(_float3(1,0,0)));
	//

	//if (0 > vCameraPos.z)
	//	fRadianAngle = 2 * D3DX_PI - fRadianAngle;

	//fRadianAngle += D3DXToRadian(90) * fTimeDelta;

}

HRESULT CCamera_Main::Revolution_Turn_AxisY_CCW(_float3 vRevPos, _float fTimeDelta)
{
	if (FAILED(Revolution_Turn_AxisY_CW(vRevPos, -fTimeDelta)))
		return E_FAIL;
	return S_OK;
}

CCamera_Main * CCamera_Main::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CCamera_Main* pInstance = new CCamera_Main(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg))) {
		MSGBOX("Failed to Create CCamera_Main");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject * CCamera_Main::Clone(void * pArg)
{
	CCamera_Main* pInstance = new CCamera_Main(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg))) {
		MSGBOX("Failed to Create CCamera_Main");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCamera_Main::Free()
{
	__super::Free();
}
