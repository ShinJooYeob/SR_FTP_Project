#include "stdafx.h"
#include "Camera_Tool.h"


const int RotSpeed = 3;

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
	Change_Camera_Demension();
	m_OffSet = ZEROVEC;

	return S_OK;
}

_int CCamera_Tool::Update(_float fDeltaTime)
{

	bool isKey = (_bool)GetSingle(CSuperToolSIngleton)->GetMyButtonView()->m_CheckCameraEnable.GetCheck();

	if (isKey)
	{
	//	_float3 newCenter = GetSingle(CSuperToolSIngleton)->Get_Center_MapPosition();
		
		if (GetSingle(CKeyMgr)->Key_Pressing(VK_UP))
		{
			m_pTransform->Move_Up(fDeltaTime);

		}
		if (GetSingle(CKeyMgr)->Key_Pressing(VK_DOWN))
		{
			m_pTransform->Move_Down(fDeltaTime);

		}
		_float3 newPos = m_pTransform->Get_MatrixState(CTransform::STATE_POS);
		_float3 newLook = m_pTransform->Get_MatrixState(CTransform::STATE_LOOK);
		_float3 newTarget = (newPos + newLook * 5);

		if (GetSingle(CKeyMgr)->Key_Pressing(VK_LEFT))
		{
			m_pTransform->Move_Right(fDeltaTime*RotSpeed);
			Set_NewLookPoint(newTarget);

		}
		if (GetSingle(CKeyMgr)->Key_Pressing(VK_RIGHT))
		{
			m_pTransform->Move_Left(fDeltaTime*RotSpeed);
			Set_NewLookPoint(newTarget);

		}

		

		if (GetSingle(CKeyMgr)->Key_Pressing('W'))
		{
			m_pTransform->Move_Forward(fDeltaTime);


		}
		if (GetSingle(CKeyMgr)->Key_Pressing('S'))
		{
			m_pTransform->Move_Backward(fDeltaTime);

		}
		



		if (GetSingle(CKeyMgr)->Key_Pressing('A'))
		{
			m_pTransform->Move_Left(fDeltaTime);
		}

		if (GetSingle(CKeyMgr)->Key_Pressing('D'))
		{

			m_pTransform->Move_Right(fDeltaTime);
		}	


		if (GetSingle(CKeyMgr)->Key_Down('T'))
		{
			// 직교 투영	
			Change_Camera_Demension();
		}

		if (GetSingle(CKeyMgr)->Key_Pressing('R'))
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

HRESULT CCamera_Tool::Set_StartPosView()
{
	m_NowMat = m_pTransform->Get_WorldMatrix();
	m_pTransform->Set_Matrix(m_StartMat);
	__super::Update(0.03f);

	return S_OK;
}

HRESULT CCamera_Tool::Set_OriginPosView()
{
	m_pTransform->Set_Matrix(m_NowMat);

	return S_OK;
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
