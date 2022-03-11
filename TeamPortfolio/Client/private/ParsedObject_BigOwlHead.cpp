#include "stdafx.h"
#include "Camera_Main.h"
#include "..\public\ParsedObject_BigOwlHead.h"

CParsedObject_BigOwlHead::CParsedObject_BigOwlHead(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParsedObject(pGraphic_Device)
{
}

CParsedObject_BigOwlHead::CParsedObject_BigOwlHead(const CParsedObject_BigOwlHead & rhs)
	: CParsedObject(rhs)
{
}

HRESULT CParsedObject_BigOwlHead::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

static _uint s_iTotalOwlCounter = 0;
HRESULT CParsedObject_BigOwlHead::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		m_Layer_Tag = (TEXT("Layer_BigOwlHead"));
		m_ComTransform->Scaled(_float3(0.5f, 0.5f, 0.5f));
		//m_fDegreeAngle = 180.f;
		//m_ComTransform->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(m_fDegreeAngle));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSettingPoint );

		if (!s_iTotalOwlCounter)
			m_ComVIBuffer->Fix_Vertex_By_Postion(_float3(0.5f, 2.75f, -1.5f));
		//m_ComVIBuffer->Fix_Vertex_By_Postion(_float3(0.5f, 3.f, -1.5f));
	}


	if (s_iTotalOwlCounter % 2) {
		FAILED_CHECK(m_ComTexture->Change_TextureLayer(TEXT("BigOwlHead")));
		m_bIsUp = true;
	}
	else
	{
		FAILED_CHECK(m_ComTexture->Change_TextureLayer(TEXT("Penguin")));
		m_bIsUp = false;
	}
	s_iTotalOwlCounter++;

	m_pMainCam = (CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	if (m_pMainCam == nullptr)
		return E_FAIL;

	return S_OK;
}

_int CParsedObject_BigOwlHead::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	if (m_bIsUp)
	{

		if (m_fFrameTime < 0.25f)
		{
			m_fFrameTime += fTimeDelta;

			m_fLookAtYPoint = GetSingle(CGameInstance)->Easing(TYPE_Linear, 0, 2.f, m_fFrameTime, 0.25f);

			if (m_fFrameTime > 0.25f)
			{
				m_fLookAtYPoint = 2.f;
			}

		}
		else if (m_fFrameTime < 0.75f)
		{
			m_fFrameTime += fTimeDelta;
			m_fLookAtYPoint = GetSingle(CGameInstance)->Easing(TYPE_Linear, 2.f, -2.f, m_fFrameTime - 0.25f, 0.5f);

			if (m_fFrameTime > 0.75f)
			{
				m_fLookAtYPoint = -2.f;
			}

		}
		else if (m_fFrameTime < 1.f)
		{
			m_fFrameTime += fTimeDelta;
			m_fLookAtYPoint = GetSingle(CGameInstance)->Easing(TYPE_Linear, -2.f, 0.f, m_fFrameTime - 0.75f, 0.25f);

			if (m_fFrameTime > 1.f)
			{
				m_fLookAtYPoint = 0.f;
			}
		}
		else
		{

			m_fFrameTime = 0;
			m_bIsUp = !m_bIsUp;
		}
	}
	else {


		if (m_fFrameTime < 0.25f)
		{
			m_fFrameTime += fTimeDelta;

			m_fLookAtYPoint = GetSingle(CGameInstance)->Easing(TYPE_Linear, 0, -2.f, m_fFrameTime, 0.25f);

			if (m_fFrameTime > 0.25f)
			{
				m_fLookAtYPoint = -2.f;
			}

		}
		else if (m_fFrameTime < 0.75f)
		{
			m_fFrameTime += fTimeDelta;
			m_fLookAtYPoint = GetSingle(CGameInstance)->Easing(TYPE_Linear, -2.f, 2.f, m_fFrameTime - 0.25f, 0.5f);

			if (m_fFrameTime > 0.75f)
			{
				m_fLookAtYPoint = 2.f;
			}

		}
		else if (m_fFrameTime < 1.f)
		{
			m_fFrameTime += fTimeDelta;
			m_fLookAtYPoint = GetSingle(CGameInstance)->Easing(TYPE_Linear, 2.f, 0.f, m_fFrameTime - 0.75f, 0.25f);

			if (m_fFrameTime > 1.f)
			{
				m_fLookAtYPoint = 0.f;
			}
		}
		else
		{

			m_fFrameTime = 0;
			m_bIsUp = !m_bIsUp;
		}









	}

	_float3 vCamPos = m_pMainCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);
	_float3 vObjPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

	vCamPos.y = vObjPos.y;

	_float3 LookAtPos = vObjPos + (vCamPos - vObjPos).Get_Nomalize() * 10.f ;

	LookAtPos.y += m_fLookAtYPoint;

	m_ComTransform->LookAt(LookAtPos);

	return _int();
}

_int CParsedObject_BigOwlHead::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS), 7.f))
		m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CParsedObject_BigOwlHead::Render()
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

_int CParsedObject_BigOwlHead::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CParsedObject_BigOwlHead::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}


HRESULT CParsedObject_BigOwlHead::SetUp_Components()
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
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_BigOwlHead"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
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

HRESULT CParsedObject_BigOwlHead::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CParsedObject_BigOwlHead::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	///////////////////////////////

	return S_OK;
}

CParsedObject_BigOwlHead * CParsedObject_BigOwlHead::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParsedObject_BigOwlHead* pInstance = new CParsedObject_BigOwlHead(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_BigOwlHead");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CParsedObject_BigOwlHead::Clone(void * pArg)
{
	CParsedObject_BigOwlHead* pInstance = new CParsedObject_BigOwlHead((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_BigOwlHead");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CParsedObject_BigOwlHead::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
