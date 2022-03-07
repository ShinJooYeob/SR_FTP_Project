#include "stdafx.h"
#include "..\public\Npc_izaac.h"

CNpc_izaac::CNpc_izaac(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CNpc(pGraphicDevice)
{

}

CNpc_izaac::CNpc_izaac(const CNpc_izaac& rhs)
	: CNpc(rhs)
{

}


HRESULT CNpc_izaac::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CNpc_izaac::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	if (pArg != nullptr)
	{
		NPCDESC* temp = (NPCDESC*)pArg;
		m_tNpcDesc.pNpcTextureName = temp->pNpcTextureName;
		m_tNpcDesc.pText = temp->pText;
		m_tNpcDesc.vDir = temp->vDir;
		m_tNpcDesc.vPos = temp->vPos;

		m_ComTransform->Scaled(_float3(1.5f, 1.5f, 1.5f));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, m_tNpcDesc.vPos);

		m_iRand = rand()%5+1;
		m_ComTexture->Change_TextureLayer(m_tNpcDesc.pNpcTextureName);
		/*	m_tNpcDesc.temp = wstring(m_tNpcDesc.temp.c_str());*/
	}



	return S_OK;
}

_int CNpc_izaac::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	m_fFrame = fDeltaTime;

	Move(fDeltaTime);

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);

	return _int();
}

_int CNpc_izaac::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;


	if (nullptr == m_ComRenderer)
		return -1;
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CNpc_izaac::Render()
{

	if (FAILED(__super::Render()))
		return E_FAIL;
	if (FAILED(m_ComTransform->Bind_WorldMatrix_Look_Camera()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComRectVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;


	if (m_fPassedTime < 5.3)
	{
		if (m_bTextStart)
		{
			wstring temp;
			temp = L"Hi I'm izaac";

			GetSingle(CGameInstance)->Render_World_Font(temp, m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1, 0), _float2(0.4f, 0.6f), _float3(255, 0, 255), m_fText);

		}
	}
	else
	{
		m_bTextStart = false;
		m_bPause = false;
	}





	return S_OK;
}

_int CNpc_izaac::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;
	return _int();
}

HRESULT CNpc_izaac::ReInitialize(void * pArg)
{
	if (FAILED(__super::ReInitialize(pArg)))
		return E_FAIL;
	return S_OK;
}
HRESULT CNpc_izaac::Move(_float DeltaTime)
{

	if (m_bPause == true)
	{


		if (m_bisMoveRight == true)
			m_ComTexture->Change_TextureLayer(L"izaac_talk_reverse");
		
		else if (m_bisMoveRight == false)
			m_ComTexture->Change_TextureLayer(L"izaac_talk");

		m_fText += DeltaTime*12.f;
		m_fPassedTime += DeltaTime;
	}
	if (m_bPause == false)
	{

		m_fText = 0.f;
		m_fPassedTime = 0.f;
		if (m_bMove == false)
		{
			if (m_bisMoveRight == true)
				m_ComTexture->Change_TextureLayer(L"izaac_Idle_reverse");
			else if (m_bisMoveRight == false)
				m_ComTexture->Change_TextureLayer(L"izaac_Idle");

			m_fStartMoveCount += DeltaTime;
			if (m_fStartMoveCount > m_iRand)
			{
				m_iRand = rand() % 5 + 1;
				m_bMove = true;
				m_fStartMoveCount = 0;
			}
		}
		else if (m_bMove == true)
		{
			m_fMoveTime += DeltaTime;

			if (m_fMoveTime < 1)
			{
				if (m_bisMoveRight == true)
				{
					m_ComTexture->Change_TextureLayer(L"izaac_walk");

					m_ComTransform->MovetoDir(m_tNpcDesc.vDir, DeltaTime);

				}
				else
				{
					m_ComTexture->Change_TextureLayer(L"izaac_walk_reverse");
					m_ComTransform->MovetoDir(m_tNpcDesc.vDir*(-1), DeltaTime);
				}
			}
			else
			{
				m_bisMoveRight = !m_bisMoveRight;
				m_fMoveTime = 0;
				m_bMove = false;
			}
		}
	}
	return S_OK;
}

_int CNpc_izaac::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	const _tchar* test = pDestObjects->Get_Layer_Tag();

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TAG_LAY(Layer_Player)))
	{
		if ((GetSingle(CGameInstance)->Get_DIKeyState(DIK_LSHIFT) & DIS_Down))
		{
			if (m_bPause == false)
				GetSingle(CGameInstance)->PlaySound(L"JW_talkloopzuboy.wav", CHANNEL_OBJECT);
			m_bPause = !m_bPause;
			m_bTextStart = !m_bTextStart;
		}

	}



	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CNpc_izaac::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fMovePerSec = 3.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.vPivot = _float3(0, -0.25f, 0);

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComRectVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, (void*)&TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_NPC"), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Collision), TAG_COM(Com_Collision), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	return S_OK;
}

HRESULT CNpc_izaac::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;
	/*
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);*/

	/*if (m_bTextureReverse)
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);*/

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 130);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



	return S_OK;
}

HRESULT CNpc_izaac::Release_RenderState()
{
	/*if (m_bTextureReverse)
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);*/

	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);






	return S_OK;
}

CNpc_izaac * CNpc_izaac::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CNpc_izaac* pInstance = new CNpc_izaac(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CNpc_izaacProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CNpc_izaac::Clone(void * pArg)
{
	CNpc_izaac* pInstance = new CNpc_izaac((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CNpc_izaacClone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CNpc_izaac::Free()
{
	__super::Free();
	Safe_Release(m_ComRenderer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComRectVIBuffer);
	Safe_Release(m_ComTransform);
	Safe_Release(m_pCollisionCom);
}
