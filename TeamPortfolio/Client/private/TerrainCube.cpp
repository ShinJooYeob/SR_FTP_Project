#include "stdafx.h"
#include "..\public\TerrainCube.h"



CTerrainCube::CTerrainCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTerrainCube::CTerrainCube(const CTerrainCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrainCube::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CTerrainCube::Initialize_Clone(void * pArg)
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
	}


	return S_OK;
}

_int CTerrainCube::Update(_float fTimeDelta)
{
	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FLEXIBLE, this);

	if (0 > __super::Update(fTimeDelta))
		return -1;


	return _int();
}

_int CTerrainCube::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);


	return _int();
}

_int CTerrainCube::Render()
{
	if (nullptr == m_ComVIBuffer || m_ComTransform == nullptr)
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CTerrainCube::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CTerrainCube::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}

HRESULT CTerrainCube::SetUp_Components()
{	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube), TAG_COM(Com_CollisionBuffer), (CComponent**)&m_ComColiisionBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collision), TAG_COM(Com_Collision), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;


	return S_OK;
}



HRESULT CTerrainCube::SetUp_RenderState()
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

	////////////////////////
	m_pGraphicDevice->SetTexture(0, NULL);
	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	/////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CTerrainCube::Release_RenderState()
{
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	///////////////////////////
	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	///////////////////////////////

	return S_OK;
}

CTerrainCube * CTerrainCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CTerrainCube* pInstance = new CTerrainCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CPlayer_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CTerrainCube::Clone(void * pArg)
{
	CTerrainCube* pInstance = new CTerrainCube((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CTestCubeMove_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CTerrainCube::Free()
{
	__super::Free();

	Safe_Release(m_ComColiisionBuffer);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
