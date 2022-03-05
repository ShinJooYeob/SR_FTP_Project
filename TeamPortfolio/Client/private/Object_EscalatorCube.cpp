#include "stdafx.h"
#include "..\public\Object_EscalatorCube.h"


CObject_EscalatorCube::CObject_EscalatorCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_EscalatorCube::CObject_EscalatorCube(const CObject_EscalatorCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_EscalatorCube::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;



	return S_OK;
}

HRESULT CObject_EscalatorCube::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));

	if (pArg != nullptr) {


		memcpy(&m_EscalatorDesc, pArg, sizeof(ESCALATORDESC));

		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, m_EscalatorDesc.vStartPos);

		CTransform::TRANSFORMDESC		TransformDesc;
		TransformDesc.fMovePerSec = m_EscalatorDesc.vStartPos.Get_Distance(m_EscalatorDesc.vEndPos) *0.25f;
		TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

		m_ComTransform->Set_TransformDesc(TransformDesc);

		m_Layer_Tag = TEXT("Layer_EscalatorCube");

		m_ComTexture->Change_TextureLayer(TEXT("EscalatorCube"));
	}


	//m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(-1.f, 2.f, 2.f));

	return S_OK;
}

_int CObject_EscalatorCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float3 RisingPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
	if (m_bCollisionSwitch == true)
	{
		m_bCollisionSwitch = false;
	}
	else if (m_EscalatorDesc.vStartPos.Get_Distance(RisingPos) > 0.2f)
	{
		//속도조절 가능

		m_ComTransform->MovetoTarget(m_EscalatorDesc.vStartPos, fTimeDelta);

		//_float3 RisingCubePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
		//m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(RisingCubePos.x, RisingCubePos.y + m_fTimer, RisingCubePos.z));
	}

	if (m_bSoundCollison == true)
	{
		m_bSoundCollison = false;
	}
	else if (m_bSoundSwitch == true)
	{
		GetSingle(CGameInstance)->PlaySound(L"EH_EscalatorCube_Retun.wav", CHANNEL_OBJECT, 2.f);
		m_bSoundSwitch = false;
	}

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CObject_EscalatorCube::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;

	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS)))
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CObject_EscalatorCube::Render()
{
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_ComVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CObject_EscalatorCube::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_EscalatorCube::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TAG_LAY(Layer_Player))||!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_FixCube")))
	{
		Collision_Descent(pDestObjects, fCollision_Distance, fDeltaTime);

		if(m_bSoundCollison == false)
		{
			if (m_bSoundSwitch == false)
			{
				GetSingle(CGameInstance)->PlaySound(L"EH_EscalatorCube_advance.wav", CHANNEL_OBJECT, 2.f);

				m_bSoundSwitch = true;
			}

			m_bSoundCollison = true;
		}
	}

	return _int();
}

_int CObject_EscalatorCube::Collision_Descent(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	m_bCollisionSwitch = true;

	_float3 RisingCubePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

	if (m_EscalatorDesc.vEndPos.Get_Distance(RisingCubePos) > 0.2f ) 
	{
		CTransform* DestTransform = (CTransform*)pDestObjects->Get_Component(TAG_COM(Com_Transform));
		_float3 CubeToTargetDist = DestTransform->Get_MatrixState(CTransform::STATE_POS) - RisingCubePos;
		m_ComTransform->MovetoTarget(m_EscalatorDesc.vEndPos, fDeltaTime);

		RisingCubePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);



		DestTransform->Set_MatrixState(CTransform::STATE_POS, RisingCubePos + CubeToTargetDist);

	}
	return _int();
}


HRESULT CObject_EscalatorCube::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Cube_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	///////////////////////////////////////////////////////
	/* For.Com_Collision */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////
	return S_OK;
}

HRESULT CObject_EscalatorCube::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CObject_EscalatorCube::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	///////////////////////////////

	return S_OK;
}

CObject_EscalatorCube * CObject_EscalatorCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_EscalatorCube* pInstance = new CObject_EscalatorCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_EscalatorCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_EscalatorCube::Clone(void * pArg)
{
	CObject_EscalatorCube* pInstance = new CObject_EscalatorCube((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_EscalatorCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_EscalatorCube::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
