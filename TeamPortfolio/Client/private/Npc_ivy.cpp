#include "stdafx.h"
#include "..\public\Npc_ivy.h"
#include "Shop.h"
CNpc_ivy::CNpc_ivy(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CNpc(pGraphicDevice)
{

}

CNpc_ivy::CNpc_ivy(const CNpc_ivy& rhs)
	: CNpc(rhs)
{

}


HRESULT CNpc_ivy::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CNpc_ivy::Initialize_Clone(void * pArg)
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

		m_iRand = rand() % 5 + 1;
		m_ComTexture->Change_TextureLayer(m_tNpcDesc.pNpcTextureName);
		/*	m_tNpcDesc.temp = wstring(m_tNpcDesc.temp.c_str());*/
	}



	return S_OK;
}

_int CNpc_ivy::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	m_fFrame = fDeltaTime;

	Move(fDeltaTime);

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);

	return _int();
}

_int CNpc_ivy::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;


	if (nullptr == m_ComRenderer)
		return -1;
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CNpc_ivy::Render()
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
			temp = L"Hi I'm ivy. \nDo you want buy a items?";

			GetSingle(CGameInstance)->Render_World_Font(temp, m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1, 0), _float2(0.4f, 0.6f), _float3(255, 0, 0), m_fText);

		}
	}
	else
	{
		if (m_bCollision)
		{
			CShop* temp = (CShop*)(GetSingle(CGameInstance)->Get_ObjectList_from_Layer(m_eNowSceneNum, TAG_LAY(Layer_Shop))->front());
			temp->Set_bIsPress();
		}
		m_bTextStart = false;
		m_bPause = false;
	}





	return S_OK;
}

_int CNpc_ivy::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;
	return _int();
}

HRESULT CNpc_ivy::ReInitialize(void * pArg)
{
	if (FAILED(__super::ReInitialize(pArg)))
		return E_FAIL;
	return S_OK;
}

/*CGameObject* player;
dynamic_cast <CPlayer*> (player=GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENEID::SCENE_STATIC, TAG_LAY(LAYERID::Layer_Player)));
(player->Get_Component(TAG_COM(COMPONENTID::Com_Transform)));*/
HRESULT CNpc_ivy::Move(_float DeltaTime)
{

	if (m_bPause == true)
	{
		

		if (m_bisMoveRight == true)
			m_ComTexture->Change_TextureLayer(L"ivy_talk_reverse");
		
		else if (m_bisMoveRight == false)
			m_ComTexture->Change_TextureLayer(L"ivy_talk");

		m_fText += DeltaTime*10.f;
		m_fPassedTime += DeltaTime;
	}
	if (m_bPause == false)
	{
		
		m_fText = 0.f;
		m_fPassedTime = 0.f;
		if (m_bMove == false)
		{
			if(m_bisMoveRight==true)
				m_ComTexture->Change_TextureLayer(L"ivy_Idle_reverse");
			else if (m_bisMoveRight == false)
				m_ComTexture->Change_TextureLayer(L"ivy_Idle");

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
					m_ComTexture->Change_TextureLayer(L"ivy_walk");
					
					m_ComTransform->MovetoDir(m_tNpcDesc.vDir, DeltaTime) ;

				}
				else
				{
					m_ComTexture->Change_TextureLayer(L"ivy_walk_reverse");
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

_int CNpc_ivy::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	const _tchar* test = pDestObjects->Get_Layer_Tag();

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TAG_LAY(Layer_Player)))
	{
		m_bCollision = true;
		if ((GetSingle(CGameInstance)->Get_DIKeyState(DIK_LSHIFT) & DIS_Down))
		{
			if (m_bPause == false)
				GetSingle(CGameInstance)->PlaySound(L"JW_talkloopgirl.wav", CHANNEL_OBJECT);

			m_bPause = !m_bPause;
			m_bTextStart = !m_bTextStart;

		}

	}

	else
		m_bCollision = false;

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CNpc_ivy::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fMovePerSec = 3.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.vPivot = _float3(0, -0.25f, 0);

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComRectVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, (void*)&TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_NPC"), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collision), TAG_COM(Com_Collision), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	return S_OK;
}

HRESULT CNpc_ivy::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;
	
	
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 130);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



	return S_OK;
}

HRESULT CNpc_ivy::Release_RenderState()
{
	
	
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	





	return S_OK;
}

CNpc_ivy * CNpc_ivy::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CNpc_ivy* pInstance = new CNpc_ivy(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CNpc_ivyProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CNpc_ivy::Clone(void * pArg)
{
	CNpc_ivy* pInstance = new CNpc_ivy((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CNpc_ivyClone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CNpc_ivy::Free()
{
	__super::Free();
	Safe_Release(m_ComRenderer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComRectVIBuffer);
	Safe_Release(m_ComTransform);
	Safe_Release(m_pCollisionCom);
}
