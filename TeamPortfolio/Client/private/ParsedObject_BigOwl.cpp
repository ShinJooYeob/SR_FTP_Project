#include "stdafx.h"
#include "Camera_Main.h"
#include "..\public\ParsedObject_BigOwl.h"


CParsedObject_BigOwl::CParsedObject_BigOwl(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParsedObject(pGraphic_Device)
{
}

CParsedObject_BigOwl::CParsedObject_BigOwl(const CParsedObject_BigOwl & rhs)
	: CParsedObject(rhs)
{
}

HRESULT CParsedObject_BigOwl::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

static _uint s_iTotalOwlBodyCounter = 0;
HRESULT CParsedObject_BigOwl::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		m_Layer_Tag = (TEXT("Layer_BigOwl"));
		m_ComTransform->Scaled(_float3(0.5f, 0.5f, 0.5f));
		m_fDegreeAngle = 0.f;
		//m_ComTransform->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(m_fDegreeAngle));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSettingPoint );

		if (!s_iTotalOwlBodyCounter)
			m_ComVIBuffer->Fix_Vertex_By_Postion(_float3(0.5f, 2.75f, -1.5f));

		m_fTargetAngle = 5.f;
		m_eEasingType = TYPE_SinIn;

	}
	

	if (s_iTotalOwlBodyCounter % 2) {
		FAILED_CHECK(m_ComTexture->Change_TextureLayer(TEXT("BigOwl")));
		m_bIsUp = true;
	}
	else
	{
		FAILED_CHECK(m_ComTexture->Change_TextureLayer(TEXT("Penguin")));
		m_bIsUp = false;
	}
	s_iTotalOwlBodyCounter++;

	m_pMainCam = (CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	if (m_pMainCam == nullptr)
		return E_FAIL;

	FAILED_CHECK(SetUp_ParticleDesc());

	return S_OK;
}

_int CParsedObject_BigOwl::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	





	if (m_bIsUp)
	{

		if (m_fFrameTime < 0.25f)
		{
			m_fFrameTime += fTimeDelta;

			m_fDegreeAngle = GetSingle(CGameInstance)->Easing(m_eEasingType, 0, m_fTargetAngle, m_fFrameTime, 0.25f);

			if (m_fFrameTime > 0.25f)
			{
				m_fDegreeAngle = m_fTargetAngle;

			}

		}
		else if (m_fFrameTime < 0.75f)
		{
			m_fFrameTime += fTimeDelta;
			m_fDegreeAngle = GetSingle(CGameInstance)->Easing(m_eEasingType, m_fTargetAngle, -m_fTargetAngle, m_fFrameTime - 0.25f, 0.5f);

			if (m_fFrameTime > 0.75f)
			{
				m_fDegreeAngle = -m_fTargetAngle;
			}

		}
		else if (m_fFrameTime < 1.f)
		{
			m_fFrameTime += fTimeDelta;
			m_fDegreeAngle = GetSingle(CGameInstance)->Easing(m_eEasingType, -m_fTargetAngle, 0.f, m_fFrameTime - 0.75f, 0.25f);

			if (m_fFrameTime > 1.f)
			{
				m_fDegreeAngle = 0.001f;
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

			m_fDegreeAngle = GetSingle(CGameInstance)->Easing(m_eEasingType, 0, -m_fTargetAngle, m_fFrameTime, 0.25f);

			if (m_fFrameTime > 0.25f)
			{
				m_fDegreeAngle = -m_fTargetAngle;
			}

		}
		else if (m_fFrameTime < 0.75f)
		{
			m_fFrameTime += fTimeDelta;
			m_fDegreeAngle = GetSingle(CGameInstance)->Easing(m_eEasingType, -m_fTargetAngle, m_fTargetAngle, m_fFrameTime - 0.25f, 0.5f);

			if (m_fFrameTime > 0.75f)
			{
				m_fDegreeAngle = m_fTargetAngle;
			}

		}
		else if (m_fFrameTime < 1.f)
		{
			m_fFrameTime += fTimeDelta;
			m_fDegreeAngle = GetSingle(CGameInstance)->Easing(m_eEasingType, m_fTargetAngle, 0.f, m_fFrameTime - 0.75f, 0.25f);

			if (m_fFrameTime > 1.f)
			{
				m_fDegreeAngle = 0.001f;
				GetSingle(CParticleMgr)->Create_ParticleObject(SCENE_STAGE2, m_ParticleDesc);

				if (rand()%2)
				{
					m_ParticleDesc.szTextureLayerTag = TEXT("Meteo_B");
				}

				else
				{
					m_ParticleDesc.szTextureLayerTag = TEXT("Meteo_A");

				}
			}
		}
		else
		{

			m_fFrameTime = 0;
			m_bIsUp = !m_bIsUp;
		}

	}

	_float3 LookAtPos = m_pMainCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);
	LookAtPos.y = m_ComTransform->Get_MatrixState(CTransform::STATE_POS).y;

	m_ComTransform->LookAt(LookAtPos);


	m_ComTransform->Turn_CW(m_ComTransform->Get_MatrixState(CTransform::STATE_LOOK), m_fDegreeAngle);


	return _int();
}

_int CParsedObject_BigOwl::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS), 7.f))
		m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CParsedObject_BigOwl::Render()
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

_int CParsedObject_BigOwl::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CParsedObject_BigOwl::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}


HRESULT CParsedObject_BigOwl::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(1.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_Parsed"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_BigOwl"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
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

HRESULT CParsedObject_BigOwl::SetUp_ParticleDesc()
{

	m_ParticleDesc.eParticleID = Particle_Fountain;
	m_ParticleDesc.TotalParticleTime = 0.f;
	m_ParticleDesc.EachParticleLifeTime = 2.0;
	m_ParticleDesc.ParticleSize = _float3(1.f, 1.2f, 1.f);
	m_ParticleDesc.Particle_Power = 7;
	m_ParticleDesc.PowerRandomRange = _float2(1.f, 1.5f);
	m_ParticleDesc.MaxParticleCount = 1;
	m_ParticleDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JY_Effect");
	m_ParticleDesc.m_bIsTextureAutoFrame = true;
	m_ParticleDesc.fAutoFrameMul = 3.f;
	m_ParticleDesc.FollowingTarget = nullptr;
	m_ParticleDesc.FixedTarget = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
	m_ParticleDesc.MaxBoundary = _float3(100, 20, 100);
	m_ParticleDesc.ParticleColorChage = false;
	m_ParticleDesc.m_bIsUI = false;
	m_ParticleDesc.ParticleStartRandomPosMin = _float3(-1.5f, 0.f, -1.5f);
	m_ParticleDesc.ParticleStartRandomPosMax = _float3(1.5f, 0.2f, 1.5f);
	m_ParticleDesc.vUp = _float3(0, 1, 0);
	m_ParticleDesc.MustDraw = false;
	m_ParticleDesc.IsParticleFameEndtoDie = false;
	m_ParticleDesc.AlphaBlendON = false;
	m_ParticleDesc.bSubPraticle = false;



	if (m_bIsUp)
	{
		m_ParticleDesc.szTextureLayerTag = TEXT("Meteo_B");
	}

	else
	{
		m_ParticleDesc.szTextureLayerTag = TEXT("Meteo_A");

	}



	return S_OK;
}

HRESULT CParsedObject_BigOwl::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CParsedObject_BigOwl::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	///////////////////////////////

	return S_OK;
}

CParsedObject_BigOwl * CParsedObject_BigOwl::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParsedObject_BigOwl* pInstance = new CParsedObject_BigOwl(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_BigOwl");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CParsedObject_BigOwl::Clone(void * pArg)
{
	CParsedObject_BigOwl* pInstance = new CParsedObject_BigOwl((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_BigOwl");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CParsedObject_BigOwl::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
