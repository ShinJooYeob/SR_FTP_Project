#include "stdafx.h"
#include "..\public\Npc.h"
#include "Camera_Main.h"

CNpc::CNpc(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{

}

CNpc::CNpc(const CNpc& rhs)
	: CGameObject(rhs)
{

}


HRESULT CNpc::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CNpc::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	if (pArg == nullptr)
	{
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(0, 1.f, 0));
		m_ComTransform->Scaled(_float3(1.5f, 1.5f, 1.5f));
	}
	else {
		FAILED_CHECK(ReInitialize(pArg));
	}
	m_pCamera_Main = ((CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main))));

	if (m_pCamera_Main == nullptr)
		return E_FAIL;
	Safe_AddRef(m_pCamera_Main);
	return S_OK;
}

_int CNpc::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	Move(fDeltaTime);
	return _int();
}

_int CNpc::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;


	if (nullptr == m_ComRenderer)
		return -1;
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CNpc::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComRectVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

_int CNpc::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;
	return _int();
}

HRESULT CNpc::ReInitialize(void * pArg)
{
	if (FAILED(__super::ReInitialize(pArg)))
		return E_FAIL;
	return S_OK;
}
HRESULT CNpc::Move(_float DeltaTime)
{
	if (m_bMove == false)
	{
		m_fStartMoveCount += DeltaTime;
		if (m_fStartMoveCount < rand() % 2)
		{
			m_bMove = true;
			m_bTextureReverse = !m_bTextureReverse;
			m_fStartMoveCount = 0;
			//npc180도 돌리는코드넣자
			_Matrix matVeiwSpace;
			m_pGraphicDevice->GetTransform(D3DTS_VIEW, &matVeiwSpace);

			_float3 vPlayerPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);


			//카메라 바라보도록 설정
			_float3 vCamLook;
			memcpy(&vCamLook, &(matVeiwSpace.InverseMatrix().m[2][0]), sizeof(_float3));

			if (m_bTextureReverse)
				m_ComTransform->LookAt(vPlayerPos - vCamLook);
			else
				m_ComTransform->LookAt(vPlayerPos + vCamLook);
		}
	}
	else if (m_bMove == true)
	{
		m_fMoveTime += DeltaTime;
		
		if (m_fMoveTime < rand() % 3)
			m_ComTransform->Move_Right(DeltaTime);
		else
		{
			m_fMoveTime = 0;
			m_bMove = false;
		}
	}
	return S_OK;
}


HRESULT CNpc::SetUp_Components()
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
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_NPC"), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collision), TAG_COM(Com_Collision), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	return S_OK;
}

HRESULT CNpc::SetUp_RenderState()
{
	if (m_bTextureReverse)
		m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 130);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_bIsShdow)
	{
		m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_XRGB(0, 0, 0));
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);

	}
	return S_OK;
}

HRESULT CNpc::Release_RenderState()
{

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	if (m_bTextureReverse)
		m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	if (m_bIsShdow)
	{
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	}
	return S_OK;
}

CNpc * CNpc::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CNpc* pInstance = new CNpc(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CNpc_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CNpc::Clone(void * pArg)
{
	CNpc* pInstance = new CNpc((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CNpc_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CNpc::Free()
{
	Safe_Release(m_ComRenderer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComRectVIBuffer);
	Safe_Release(m_ComTransform);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_pCamera_Main);
}
