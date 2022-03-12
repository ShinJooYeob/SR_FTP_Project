#include "stdafx.h"
#include "..\public\Object_OrbitButton.h"
#include "Object_OrbitCube.h"


CObject_OrbitButton::CObject_OrbitButton(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_OrbitButton::CObject_OrbitButton(const CObject_OrbitButton & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_OrbitButton::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_OrbitButton::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (pArg == nullptr)
		return E_FAIL;

	memcpy(&m_tOrbitDesc, pArg, sizeof(ORBITDESC));


	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(Clone_OrbitCube()))
		return E_FAIL;

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, m_tOrbitDesc.vButtonPos);


	m_ComTexture->Change_TextureLayer(TEXT("DefaultCube"));
	m_Layer_Tag = TEXT("Layer_OrbitButton");

	return S_OK;
}

_int CObject_OrbitButton::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);



	if (m_bSwitch)
	{
		m_fPassedTime += fTimeDelta;
		if (m_fPassedTime > 2.f)
			m_bSwitch = false;
	}

	return _int();
}

_int CObject_OrbitButton::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS)))
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CObject_OrbitButton::Render()
{

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(10)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_ComVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

_int CObject_OrbitButton::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_OrbitButton::Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TAG_LAY(Layer_Player)))
	{
		if (!m_bSwitch && GetSingle(CGameInstance)->Get_DIKeyState(DIK_LSHIFT) & DIS_Down)
		{
			m_bSwitch = true;
			m_fPassedTime = 0;
		}

	}

	return _int();
}

HRESULT CObject_OrbitButton::SetUp_Components()
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

HRESULT CObject_OrbitButton::SetUp_RenderState()
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

HRESULT CObject_OrbitButton::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

HRESULT CObject_OrbitButton::Clone_OrbitCube()
{
	CObject_OrbitCube::ORBITCUBEDESC tDesc;
	tDesc.fRotAxis = m_tOrbitDesc.vOrbitRotAxis;


	for (_uint i = 0; i < m_tOrbitDesc.vOribitTotalXYZ.x;i++)
	{
		for (_uint j = 0; j < m_tOrbitDesc.vOribitTotalXYZ.y; j++)
		{
			for (_uint k = 0; k < m_tOrbitDesc.vOribitTotalXYZ.z; k++)
			{
				tDesc.fTransform = m_tOrbitDesc.vOribitCubeStartPos + _float3(_float(i), _float(j), _float(k) );

				if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Terrain), TEXT("Prototype_GameObject_Object_OrbitCube"), &tDesc))
					return E_FAIL;

				CObject_OrbitCube* pTargetCube = (CObject_OrbitCube*)(GetSingle(CGameInstance)->Get_ObjectList_from_Layer(m_eNowSceneNum, TAG_LAY(Layer_Terrain))->back());

				if (pTargetCube == nullptr)
					return E_FAIL;

				pTargetCube->Set_ButtonCube(this);

			}
		}
	}

	return S_OK;
}

CObject_OrbitButton * CObject_OrbitButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_OrbitButton* pInstance = new CObject_OrbitButton(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_OrbitButton");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_OrbitButton::Clone(void * pArg)
{
	CObject_OrbitButton* pInstance = new CObject_OrbitButton((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_OrbitButton");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_OrbitButton::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
	Safe_Release(m_ComTexture);
}
