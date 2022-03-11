#include "stdafx.h"
#include "..\public\Npc_izaacTuto.h"

CNpc_izaacTuto::CNpc_izaacTuto(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CNpc(pGraphicDevice)
{

}

CNpc_izaacTuto::CNpc_izaacTuto(const CNpc_izaacTuto& rhs)
	: CNpc(rhs)
{

}


HRESULT CNpc_izaacTuto::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CNpc_izaacTuto::Initialize_Clone(void * pArg)
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
		m_tNpcDesc.eCubeInfo = temp->eCubeInfo;
		m_ComTransform->Scaled(_float3(1.5f, 1.5f, 1.5f));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, m_tNpcDesc.vPos);

		m_ComTexture->Change_TextureLayer(m_tNpcDesc.pNpcTextureName);
		/*	m_tNpcDesc.temp = wstring(m_tNpcDesc.temp.c_str());*/
	}



	return S_OK;
}

_int CNpc_izaacTuto::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	m_fFrame = fDeltaTime;

	Move(fDeltaTime);

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);

	return _int();
}

_int CNpc_izaacTuto::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;


	if (nullptr == m_ComRenderer)
		return -1;
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CNpc_izaacTuto::Render()
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
			if (m_tNpcDesc.eCubeInfo == E_CUBEID::CUBEID_FIXED)
			{
				temp = L"This is a fixCube \nPress L-Shift Key";
				GetSingle(CGameInstance)->Render_World_Font(temp, m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1, 0), _float2(0.4f, 0.6f), _float3(155, 70, 255), m_fText);
			}
			else if (m_tNpcDesc.eCubeInfo == E_CUBEID::CUBEID_APPEAR)
			{
				temp = L"this is a AppearCube\nIf you go closer,\nA Cube will appear";
				GetSingle(CGameInstance)->Render_World_Font(temp, m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1, 0), _float2(0.4f, 0.6f), _float3(205, 75, 35), m_fText);
			}
			else if (m_tNpcDesc.eCubeInfo == E_CUBEID::CUBEID_ELEVETOR)
			{
				temp = L"this is a ElevatorCube\nYou can board on the Cube";
				GetSingle(CGameInstance)->Render_World_Font(temp, m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1, 0), _float2(0.4f, 0.6f), _float3(216, 50, 110), m_fText);
			}
			else if (m_tNpcDesc.eCubeInfo == E_CUBEID::CUBEID_ORBIT)
			{
				temp = L"this is a OrbitCube\nPress L-Shift Key";
				GetSingle(CGameInstance)->Render_World_Font(temp, m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1, 0), _float2(0.4f, 0.6f), _float3(170, 30, 35), m_fText);
			}
			else if (m_tNpcDesc.eCubeInfo == E_CUBEID::CUBEID_GRAVITY)
			{
				temp = L"this is a GravityCube\nDon't go closer";
				GetSingle(CGameInstance)->Render_World_Font(temp, m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1, 0), _float2(0.4f, 0.6f), _float3(58, 142, 107), m_fText);
			}
			else if (m_tNpcDesc.eCubeInfo == E_CUBEID::CUBEID_POTAL)
			{
				temp = L"this is a PortalCube\nIt teleports you ";
				GetSingle(CGameInstance)->Render_World_Font(temp, m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1, 0), _float2(0.4f, 0.6f), _float3(85, 86, 210), m_fText);
			}
			else if (m_tNpcDesc.eCubeInfo == E_CUBEID::CUBEID_VANISH)
			{
				temp = L"this is a VanishCube\nIt will disappear \nwhen you get on it";
				GetSingle(CGameInstance)->Render_World_Font(temp, m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1, 0), _float2(0.4f, 0.6f), _float3(200, 75, 150), m_fText);
			}
		}
	}
	else
	{
		m_bTextStart = false;
		m_bPause = false;
	}





	return S_OK;
}

_int CNpc_izaacTuto::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;
	return _int();
}

HRESULT CNpc_izaacTuto::ReInitialize(void * pArg)
{
	if (FAILED(__super::ReInitialize(pArg)))
		return E_FAIL;
	return S_OK;
}
HRESULT CNpc_izaacTuto::Move(_float DeltaTime)
{

	if (m_bPause == true)
	{
			m_ComTexture->Change_TextureLayer(L"izaac_talk");

		m_fText += DeltaTime*12.f;
		m_fPassedTime += DeltaTime;
	}
	if (m_bPause == false)
	{

		m_fText = 0.f;
		m_fPassedTime = 0.f;
		m_ComTexture->Change_TextureLayer(L"izaac_Idle");

		
	}
	return S_OK;
}

_int CNpc_izaacTuto::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	const _tchar* test = pDestObjects->Get_Layer_Tag();

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TAG_LAY(Layer_Player)))
	{
		if ((GetSingle(CGameInstance)->Get_DIKeyState(DIK_LCONTROL) & DIS_Down))
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

HRESULT CNpc_izaacTuto::SetUp_Components()
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

HRESULT CNpc_izaacTuto::SetUp_RenderState()
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

HRESULT CNpc_izaacTuto::Release_RenderState()
{
	/*if (m_bTextureReverse)
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);*/

	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);






	return S_OK;
}

CNpc_izaacTuto * CNpc_izaacTuto::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CNpc_izaacTuto* pInstance = new CNpc_izaacTuto(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CNpc_izaacTutoProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CNpc_izaacTuto::Clone(void * pArg)
{
	CNpc_izaacTuto* pInstance = new CNpc_izaacTuto((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CNpc_izaacTutoClone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CNpc_izaacTuto::Free()
{
	__super::Free();
	Safe_Release(m_ComRenderer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComRectVIBuffer);
	Safe_Release(m_ComTransform);
	Safe_Release(m_pCollisionCom);
}
