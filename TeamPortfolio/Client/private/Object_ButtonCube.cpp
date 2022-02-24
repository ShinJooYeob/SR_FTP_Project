#include "stdafx.h"
#include "..\public\Object_ButtonCube.h"


CObject_ButtonCube::CObject_ButtonCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_ButtonCube::CObject_ButtonCube(const CObject_ButtonCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_ButtonCube::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_ButtonCube::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(7.f, 0.f, 7.f));

	m_ComTexture->Change_TextureLayer(TEXT("DefaultCube"));

	return S_OK;
}

_int CObject_ButtonCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CObject_ButtonCube::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;

	//if (FAILED(SetUp_OnTerrain(fTimeDelta)))
	//	return -1;

	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CObject_ButtonCube::Render()
{

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture(3)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_ComVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

_int CObject_ButtonCube::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_ButtonCube::Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	const _tchar* test = pDestObjects->Get_Layer_Tag();

	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_Cube")))
	{
		Collision_Interactive(fDeltaTime);
	}

	return _int();
}

_int CObject_ButtonCube::Collision_Interactive(_float fDeltaTime)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//객체에게 중력을 적용하기 위한 값
	CTransform* TransformCube = (CTransform*)pGameInstance->Get_Commponent_By_LayerIndex(SCENE_STAGE2, TEXT("Layer_InteractiveCube"), TAG_COM(Com_Transform));

	_float3& CubePos = TransformCube->Get_MatrixState(CTransform::STATE_POS);

	CGameInstance* pInstance = GetSingle(CGameInstance);


	seconds += fDeltaTime;
	_float TempAngle;

	if (!m_bSwitch)
	{
		if (!m_bSwitch && pInstance->Get_DIKeyState(DIK_Q) & DIS_Down)
		{
			Angle += 90;
			m_bSwitch = true;
			seconds = 0;
		}

		if (!m_bSwitch && pInstance->Get_DIKeyState(DIK_E) & DIS_Down)
		{
			Angle -= 90;
			m_bSwitch = true;
			seconds = 0;
		}

	}
	else
	{
		TempAngle = GetSingle(CGameInstance)->Easing(TYPE_ElasticInOut, m_RotAngle, m_RotAngle + Angle, seconds);

		if (seconds > 1.f)
		{
			m_RotAngle = m_RotAngle + Angle;

			TempAngle = m_RotAngle;

			seconds = 0;
			Angle = 0;
			m_bSwitch = false;
		}

		TransformCube->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(TempAngle));
	}

	//if ( pInstance->Get_DIKeyState(DIK_Q) & DIS_Down)
	//{
	//	Switch_Q = true;
	//}

	//if (Switch_Q)
	//{
	//	_float TempAngle = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, m_RotAngle, m_RotAngle + 90, seconds, 2.0f);

	//	if (seconds > 2.f)
	//	{
	//		seconds = 0;

	//		m_RotAngle = m_RotAngle + 90;
	//		TempAngle = m_RotAngle;

	//		Switch_Q = false;
	//	}
	//	TransformCube->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(TempAngle));
	//}

	//if ( pInstance->Get_DIKeyState(DIK_E) & DIS_Down)
	//{
	//	Switch_E = true;
	//}
	//if(Switch_E)
	//{
	//	_float TempAngle = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, m_RotAngle, m_RotAngle - 90, seconds, 2.0f);

	//	if (seconds > 2.f)
	//	{
	//		seconds = 0;

	//		m_RotAngle = m_RotAngle - 90;
	//		TempAngle = m_RotAngle;

	//		Switch_E = false;
	//	}
	//	TransformCube->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(TempAngle));
	//}


	RELEASE_INSTANCE(CGameInstance);


	return _int();
}

HRESULT CObject_ButtonCube::SetUp_Components()
{
	/* For.트랜스폼 */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	/* For.트랜스폼 */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Cube_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	/* For.렌더러 */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.버텍스 큐브 */

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	///////////////////////////////////////////////////////
	/* For.콜리전 */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////

	return S_OK;
}

HRESULT CObject_ButtonCube::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	/*
	_float4		vSourColor, vDestColor;

	(vSourColor.rgb) * vSourColor.a + (vDestColor.rgb) * (1.f - vSourColor.a);*/

	return S_OK;
}

HRESULT CObject_ButtonCube::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

CObject_ButtonCube * CObject_ButtonCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_ButtonCube* pInstance = new CObject_ButtonCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_ButtonCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_ButtonCube::Clone(void * pArg)
{
	CObject_ButtonCube* pInstance = new CObject_ButtonCube((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_ButtonCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_ButtonCube::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
	Safe_Release(m_ComTexture);
}
