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

	m_StartPos = m_pTransform->Get_MatrixState(CTransform::STATE_POS);

	return S_OK;
}

_int CCamera_Tool::Update(_float fDeltaTime)
{


	// 나중에 맵에서 사용
	//if (GetKeyState(VK_UP) & 0x8000)
	//{		
	//	m_pTransform->Move_Up(fDeltaTime);
	//}
	//if (GetKeyState(VK_DOWN) & 0x8000)
	//{
	//	m_pTransform->Move_Down(fDeltaTime);
	//}

	//if (GetKeyState(VK_LEFT) & 0x8000)
	//{
	//	m_pTransform->Move_Left(fDeltaTime);
	//}
	//if (GetKeyState(VK_RIGHT) & 0x8000)
	//{
	//	m_pTransform->Move_Right(fDeltaTime);
	//}

	//if (GetKeyState('R') & 0x8000)
	//{
	//	m_pTransform->Set_MatrixState(CTransform::STATE_POS, m_StartPos);
	//}

	if(FAILED(__super::Update(fDeltaTime)))
		return -1;

	
	 __super::Update(fDeltaTime);


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

HRESULT CCamera_Tool::Revolution_Turn_AxisY_CW(_float3 vRevPos, _float fTimeDelta)
{

	m_fPassedTime += fTimeDelta;
	_float3 vOriginCameraPos = m_pTransform->Get_MatrixState(CTransform::STATE_POS);
	vRevPos.y = 0;
	_float3 vCameraPos = vOriginCameraPos - vRevPos;
	
	vCameraPos.y = 0;

	_float fRadianAngle = GetSingle(CGameInstance)->TargetQuadIn(m_fStartAngle, m_fTargetAngle, m_fPassedTime);

	if (m_fPassedTime >= 1.f) 
	{
		m_IsTurning = false;
		m_fStartAngle = m_fTargetAngle;
		fRadianAngle = m_fTargetAngle;
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

HRESULT CCamera_Tool::Revolution_Turn_AxisY_CCW(_float3 vRevPos, _float fTimeDelta)
{
	if (FAILED(Revolution_Turn_AxisY_CW(vRevPos, -fTimeDelta)))
		return E_FAIL;
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
