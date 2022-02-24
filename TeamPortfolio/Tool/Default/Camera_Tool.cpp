#include "stdafx.h"
#include "Camera_Tool.h"



CCamera_Tool::CCamera_Tool(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CCamera(pGraphicDevice)
{
}

CCamera_Tool::CCamera_Tool(const CCamera_Tool & rhs)
	:CCamera(rhs)
{
}

HRESULT CCamera_Tool::Initialize_Prototype(void * pArg)
{
	
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCamera_Tool::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	m_StartMat = m_pTransform->Get_WorldMatrix();

	return S_OK;
}

_int CCamera_Tool::Update(_float fDeltaTime)
{

	bool isKey = (_bool)GetSingle(CSuperToolSIngleton)->GetMyButtonView()->m_CheckCameraEnable.GetCheck();

	if (isKey)
	{
		if (GetKeyState(VK_UP) & 0x8000)
		{
			m_pTransform->Move_Up(fDeltaTime);
		}
		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			m_pTransform->Move_Down(fDeltaTime);
		}

		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			m_pTransform->Move_Left(fDeltaTime);
		}
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			m_pTransform->Move_Right(fDeltaTime);
		}


		if (GetKeyState('W') & 0x8000)
		{
			m_pTransform->Move_Forward(fDeltaTime);
		}
		if (GetKeyState('S') & 0x8000)
		{
			m_pTransform->Move_Backward(fDeltaTime);
		}

		if (GetKeyState('E') & 0x8000)
		{
			m_pTransform->Turn_CW(_float3(0,1,0),fDeltaTime);
		}
		if (GetKeyState('Q') & 0x8000)
		{
			m_pTransform->Turn_CCW(_float3(0, 1, 0), fDeltaTime);
		}

		if (GetKeyState('T') & 0x8000)
		{
			// 직교 투영	
			Change_Camera_Demension();
		}

		if (GetKeyState('R') & 0x8000)
		{
			m_pTransform->Set_Matrix(m_StartMat);
		}
	}
	


	if(FAILED(__super::Update(fDeltaTime)))
		return -1;


	return 0;
}

_int CCamera_Tool::LateUpdate(_float fDeltaTime)
{

	return _int();
}

_int CCamera_Tool::Render()
{
	return _int();
}

_int CCamera_Tool::LateRender()
{
	return _int();
}

CCamera_Tool * CCamera_Tool::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CCamera_Tool* pInstance = new CCamera_Tool(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg))) {
		MSGBOX("Failed to Create CCamera_Tool");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject * CCamera_Tool::Clone(void * pArg)
{
	CCamera_Tool* pInstance = new CCamera_Tool(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg))) {
		MSGBOX("Failed to Create CCamera_Tool");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCamera_Tool::Free()
{
	__super::Free();
}
