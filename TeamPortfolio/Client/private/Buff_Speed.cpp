#include "stdafx.h"
#include "..\public\Buff_Speed.h"
#include "Camera_Main.h"





CBuff_Speed::CBuff_Speed(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

CBuff_Speed::CBuff_Speed(const CBuff_Speed & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBuff_Speed::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBuff_Speed::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	return S_OK;
}

_int CBuff_Speed::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	m_fFrame += 12.0f * fDeltaTime;

	if (m_fFrame >= 12.0f)
		m_fFrame = 0.f;


	//static _bool IsRTurning = false;
	//static _bool IsLTurning = false;
	//static _float fPassedTime = false;
	//static _float fStarPoint = 0;
	//static _float fTargetPoint = 0;

	//if (pInstance->Get_DIKeyState(DIK_E) & DIS_Down)
	//{
	//	IsRTurning = true;
	//	fPassedTime = 0;
	//	fTargetPoint = fStarPoint+90;


	//}
	//else if(IsRTurning)
	//{

	//	fPassedTime += fDeltaTime;

	//	_float fNowPoint;
	//	fNowPoint = pInstance->TargetLinear(fStarPoint, fTargetPoint,  fPassedTime);

	//	if (fNowPoint >= fTargetPoint)
	//	{
	//		IsRTurning = false;
	//		fNowPoint = fTargetPoint;
	//	}

	//	m_ComTransform->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(fNowPoint));

	//}

	//if (pInstance->Get_DIKeyState(DIK_Q) & DIS_Down)
	//{

	//	IsLTurning = true;
	//	fPassedTime = 0;
	//	fTargetPoint = fStarPoint - 90;

	//}
	//else if(IsLTurning)
	//{
	//	fPassedTime += fDeltaTime;

	//	_float fNowPoint;
	//	fNowPoint = pInstance->TargetQuadIn(fStarPoint, fTargetPoint, fPassedTime);

	//	if (fNowPoint <= fTargetPoint)
	//	{
	//		IsLTurning = false;
	//		fNowPoint = fTargetPoint;
	//	}

	//	m_ComTransform->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(fNowPoint));

	//}
		 
	/*if (pInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
	{
		m_fNowJumpPower = m_fJumpPower;
		m_bIsJumped = true;

		m_ComTransform->MovetoTarget(m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1.f, 0), fDeltaTime);
	}
*/



	


	return _int();
}

_int CBuff_Speed::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;


	
	//렌더링 그룹에 넣어주는 역활
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return _int();
}

_int CBuff_Speed::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL; 

	m_ComTransform->Scaled(_float3(20.f, 20.f, 1));
	
	CCamera_Main* pCamera = (CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGESELECT, TEXT("Layer_Camera_Main")));

	if (FAILED(m_ComTransform->Bind_WorldMatrix_Look_Camera(pCamera->Get_Camera_Position())))
		return E_FAIL;
	
	 //if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		//return E_FAIL;
	
	if (FAILED(m_ComTexture->Bind_Texture((_uint)m_fFrame)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CBuff_Speed::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CBuff_Speed::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.vPivot = _float3(0, -0.5f, 0);

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STAGESELECT, TEXT("Prototype_Component_Texture_Buff_Speed"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;




	return S_OK;
}



HRESULT CBuff_Speed::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	return S_OK;
}

HRESULT CBuff_Speed::Release_RenderState()
{
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}


CBuff_Speed * CBuff_Speed::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CBuff_Speed* pInstance = new CBuff_Speed(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CBuff_Speed_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CBuff_Speed::Clone(void * pArg)
{
	CBuff_Speed* pInstance = new CBuff_Speed((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CBuff_Speed_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CBuff_Speed::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
