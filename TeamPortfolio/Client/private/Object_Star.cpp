#include "stdafx.h"
#include "..\public\Object_Star.h"
#include "UI_Result.h"

CObject_Star::CObject_Star(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_Star::CObject_Star(const CObject_Star & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_Star::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_Star::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		memcpy(&m_StarDesc, pArg, sizeof(STARDESC));

		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, m_StarDesc.fTransform);
	}

	m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));

	m_ComTexture->Change_TextureLayer(TEXT("Object_Star"));

	return S_OK;
}

_int CObject_Star::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FLEXIBLE, this);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CObject_Star::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;

	//if (FAILED(SetUp_OnTerrain(fTimeDelta)))
	//	return -1;

	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CObject_Star::Render()
{

	//if (FAILED(m_ComTransform->Bind_WorldMatrix()))
	//	return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix_Look_Camera()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_ComRectVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CObject_Star::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_Star::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	const _tchar* test = pDestObjects->Get_Layer_Tag();

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TAG_LAY(Layer_Player)))
	{
		CUI_Result* UI_Result =(CUI_Result*)pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum,TEXT("Layer_UI_Result"));
		UI_Result->Set_RankStar();
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, NOT_EXIST_BLOCK);
		DIED();
		//m_pCollisionCom->Collision_Pushed(m_ComTransform, fCollision_Distance, fDeltaTime);
	}



	RELEASE_INSTANCE(CGameInstance);

	return _int();
}
HRESULT CObject_Star::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_UI_Result_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer_Rect */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComRectVIBuffer)))
		return E_FAIL;

	///////////////////////////////////////////////////////
	/* For.Com_Collision */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////
	return S_OK;
}

HRESULT CObject_Star::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	/*
	_float4		vSourColor, vDestColor;

	(vSourColor.rgb) * vSourColor.a + (vDestColor.rgb) * (1.f - vSourColor.a);*/

	return S_OK;
}

HRESULT CObject_Star::Release_RenderState()
{

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	return S_OK;
}

CObject_Star * CObject_Star::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_Star* pInstance = new CObject_Star(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_Star");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_Star::Clone(void * pArg)
{
	CObject_Star* pInstance = new CObject_Star((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_Star");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_Star::Free()
{
	__super::Free();

	Safe_Release(m_ComRectVIBuffer);
	Safe_Release(m_ComCubeVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
