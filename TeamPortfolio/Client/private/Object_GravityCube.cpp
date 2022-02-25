#include "stdafx.h"
#include "..\public\Object_GravityCube.h"


CObject_GravityCube::CObject_GravityCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_GravityCube::CObject_GravityCube(const CObject_GravityCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_GravityCube::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_GravityCube::Initialize_Clone(void * pArg)
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
		m_Layer_Tag = TEXT("Layer_GravityCube");

		m_ComTransform->Scaled({ 3.f,3.f,3.f });
		m_ComTexture->Change_TextureLayer(L"none", 15.f);
		m_ComTexture->Change_TextureLayer(L"Gravity",15.f);
	}

	return S_OK;
}

_int CObject_GravityCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);
	m_fFrame = fTimeDelta;


	return _int();
}

_int CObject_GravityCube::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;



	//객체에게 중력을 적용하기 위한 값

	if (FAILED(Collision_Gravity(fTimeDelta)))
		return -1;

	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS),3.f))
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CObject_GravityCube::Render()
{
	if (FAILED(m_ComTransform->Bind_WorldMatrix_Look_Camera()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;


	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CObject_GravityCube::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_GravityCube::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}

_int CObject_GravityCube::Collision_Gravity(_float fDeltaTime)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//객체에게 중력을 적용하기 위한 값
	CTransform* Player = (CTransform*)pGameInstance->Get_Commponent_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player), TAG_COM(Com_Transform));

	if (Player == nullptr)
		return E_FAIL;


	_float3& PlayerPos = Player->Get_MatrixState(CTransform::STATE_POS);

	_float3& GravityCubePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

	_float Distance = GravityCubePos.Get_Distance(PlayerPos);

	if (Distance < 2.5f) //거리 조절 가능
	{
		fDeltaTime *= 0.5f; // 속도 조절 가능
		Player->MovetoTarget(GravityCubePos, fDeltaTime);
	}


	RELEASE_INSTANCE(CGameInstance);

	return _int();
}


HRESULT CObject_GravityCube::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_GravityTexture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	///////////////////////////////////////////////////////
	/* For.Com_Collision */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////
	return S_OK;
}

HRESULT CObject_GravityCube::SetUp_OnTerrain(_float fDeltaTime)
{
	return S_OK;
}

HRESULT CObject_GravityCube::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 30);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(180, 255, 255, 255));

	return S_OK;
}

HRESULT CObject_GravityCube::Release_RenderState()
{
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	return S_OK;
}

CObject_GravityCube * CObject_GravityCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_GravityCube* pInstance = new CObject_GravityCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_GravityCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_GravityCube::Clone(void * pArg)
{
	CObject_GravityCube* pInstance = new CObject_GravityCube((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_GravityCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_GravityCube::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
