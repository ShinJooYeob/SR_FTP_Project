#include "stdafx.h"
#include "..\public\ParsedObject_Alien.h"


CParsedObject_Alien::CParsedObject_Alien(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParsedObject(pGraphic_Device)
{
}

CParsedObject_Alien::CParsedObject_Alien(const CParsedObject_Alien & rhs)
	: CParsedObject(rhs)
{
}

HRESULT CParsedObject_Alien::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}


static _uint s_iTotalAlienCounter = 0;
HRESULT CParsedObject_Alien::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		m_Layer_Tag = (TEXT("Layer_Alien"));
		m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSettingPoint);

		m_DefaultPos = vSettingPoint;
		if (!s_iTotalAlienCounter)
			m_ComVIBuffer->Fix_Vertex_By_Postion(_float3(0.0f, 2.5f, 0.0f));

	}

	FAILED_CHECK(m_ComTexture->Change_TextureLayer(TEXT("Alien")));


	s_iTotalAlienCounter++;
	return S_OK;
}

_int CParsedObject_Alien::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;


	if (m_fFrameTime < 0.5f)
	{
		m_fFrameTime += fTimeDelta;
		m_ChangingY = GetSingle(CGameInstance)->Easing(TYPE_Linear, 0, 2.f, m_fFrameTime, 0.5f);
		m_fAngle = GetSingle(CGameInstance)->Easing(TYPE_Linear, 0, 45, m_fFrameTime, 0.5f);

		if (m_fFrameTime > 0.5f)
		{
			m_ChangingY = 2.f;
			m_fAngle = 45.f;
		}

	}
	else if (m_fFrameTime < 0.75f)
	{
		m_fFrameTime += fTimeDelta;
		m_ChangingY = GetSingle(CGameInstance)->Easing(TYPE_Linear, 2.f, 0, m_fFrameTime - 0.5f, 0.25f);

		if (m_fFrameTime > 0.75f)
		{
			m_ChangingY = 0.f;
		}
	}
	else if(m_fFrameTime < 1.50f)
	{
		m_fFrameTime += fTimeDelta;
		m_fAngle = GetSingle(CGameInstance)->Easing(TYPE_Linear, 45, 90, m_fFrameTime - 0.75f, 0.75f);

		if (m_fFrameTime > 1.5f)
		{
			m_fAngle = 90;
		}
	}
	else
	{
		m_fFrameTime = 0;
	}


	m_ComTransform->Rotation_CW(_float3(0,1,0), D3DXToRadian( m_fAngle));
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, m_DefaultPos +_float3(0, m_ChangingY,0));

	return _int();
}

_int CParsedObject_Alien::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS)))
		m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CParsedObject_Alien::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	FAILED_CHECK(m_ComVIBuffer->Render());

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CParsedObject_Alien::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CParsedObject_Alien::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}


HRESULT CParsedObject_Alien::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_Parsed"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Alien"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;



	///////////////////////////////////////////////////////
	/* For.Com_Collision */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////
	return S_OK;
}

HRESULT CParsedObject_Alien::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CParsedObject_Alien::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	///////////////////////////////

	return S_OK;
}

CParsedObject_Alien * CParsedObject_Alien::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParsedObject_Alien* pInstance = new CParsedObject_Alien(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_Alien");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CParsedObject_Alien::Clone(void * pArg)
{
	CParsedObject_Alien* pInstance = new CParsedObject_Alien((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_Alien");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CParsedObject_Alien::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
