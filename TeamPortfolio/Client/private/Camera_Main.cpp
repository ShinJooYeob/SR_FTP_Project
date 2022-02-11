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

	if (pInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	{
		m_pTransform->Move_Right(fDeltaTime);
	}


	if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_DoubleDown)
	{
		Change_Camera_Demension();
	}

	_long MoveDist;

	if (MoveDist = pInstance->Get_DIMouseMoveState(CInput_Device::MMS_X))
	{
		m_pTransform->Turn_CW({0,1,0}, fDeltaTime * MoveDist * 0.1f);

	}
	if (MoveDist = pInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y))
	{
		m_pTransform->Turn_CW(m_pTransform->Get_MatrixState(CTransform::STATE_RIGHT), fDeltaTime * MoveDist * 0.1f);
	}


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
