#include "stdafx.h"
#include "..\public\Object_FixCube.h"


CObject_FixCube::CObject_FixCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_FixCube::CObject_FixCube(const CObject_FixCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_FixCube::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_FixCube::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;



	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSettingPoint);
		m_Layer_Tag = TEXT("Layer_FixCube");
		m_ComTexture->Change_TextureLayer(L"FixedCube");
	}

	return S_OK;
}

_int CObject_FixCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FLEXIBLE, this);

	return _int();
}

_int CObject_FixCube::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;

	//if (FAILED(SetUp_OnTerrain(fTimeDelta)))
	//	return -1;
	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS)))
		m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CObject_FixCube::Render()
{

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture(2)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return S_OK;
}

_int CObject_FixCube::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_FixCube::Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TAG_LAY(Layer_Player)) || !lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_FixCube")))
	{
	}

	return _int();
}

HRESULT CObject_FixCube::SetUp_Components()
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

HRESULT CObject_FixCube::SetUp_RenderState()
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

HRESULT CObject_FixCube::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

CObject_FixCube * CObject_FixCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_FixCube* pInstance = new CObject_FixCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_FixCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_FixCube::Clone(void * pArg)
{
	CObject_FixCube* pInstance = new CObject_FixCube((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_FixCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_FixCube::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
	Safe_Release(m_ComTexture);
}
