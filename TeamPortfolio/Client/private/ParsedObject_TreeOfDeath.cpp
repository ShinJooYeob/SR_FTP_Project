#include "stdafx.h"
#include "..\public\ParsedObject_TreeOfDeath.h"


CParsedObject_TreeOfDeath::CParsedObject_TreeOfDeath(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParsedObject(pGraphic_Device)
{
}

CParsedObject_TreeOfDeath::CParsedObject_TreeOfDeath(const CParsedObject_TreeOfDeath & rhs)
	: CParsedObject(rhs)
{
}

HRESULT CParsedObject_TreeOfDeath::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}


static _uint iDeathTreeCount = 0;
HRESULT CParsedObject_TreeOfDeath::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		m_Layer_Tag = (TEXT("Layer_TreeOfDeath"));
		//m_ComTransform->Scaled(_float3(0.16f, 0.16f, 0.16f));
		m_ComTransform->Scaled(_float3(0.32f, 0.32f, 0.32f));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSettingPoint + _float3(0,0.5f,0));
	}

	FAILED_CHECK(m_ComTexture->Change_TextureLayer(TEXT("TreeOfDeath")));

	if (iDeathTreeCount == 0)
	{
		m_ComVIBuffer->Fix_Vertex_By_Postion(_float3(0, 4.0f, 0));

	}

	SetUp_ParticleDesc();

	iDeathTreeCount++;
	return S_OK;
}

_int CParsedObject_TreeOfDeath::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;



	return _int();
}

_int CParsedObject_TreeOfDeath::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS), 7.f))
		m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CParsedObject_TreeOfDeath::Render()
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

_int CParsedObject_TreeOfDeath::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CParsedObject_TreeOfDeath::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}


HRESULT CParsedObject_TreeOfDeath::SetUp_Components()
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
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_TreeOfDeath"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
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

HRESULT CParsedObject_TreeOfDeath::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CParsedObject_TreeOfDeath::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	///////////////////////////////

	return S_OK;
}

HRESULT CParsedObject_TreeOfDeath::SetUp_ParticleDesc()
{
	
	m_ArrParticleDesc[0].eParticleID = Particle_Spread;
	m_ArrParticleDesc[0].TotalParticleTime = 3000.5f;
	m_ArrParticleDesc[0].EachParticleLifeTime = 0.5f;
	m_ArrParticleDesc[0].ParticleSize = _float3(1.f, 1.2f, 1.f);
	m_ArrParticleDesc[0].Particle_Power = 1;
	m_ArrParticleDesc[0].PowerRandomRange = _float2(0.2f, 1.f);
	m_ArrParticleDesc[0].MaxParticleCount = 15;
	m_ArrParticleDesc[0].szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_DeathTree_Effect");
	m_ArrParticleDesc[0].szTextureLayerTag = TEXT("DeathTree_Effect");
	m_ArrParticleDesc[0].m_bIsTextureAutoFrame = true;
	m_ArrParticleDesc[0].fAutoFrameMul = 6.f;
	m_ArrParticleDesc[0].FollowingTarget = nullptr;
	m_ArrParticleDesc[0].FixedTarget = m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 3.2f, 0);
	m_ArrParticleDesc[0].MaxBoundary = _float3(1, 20, 1);

	m_ArrParticleDesc[0].ParticleColorChage = true;
	m_ArrParticleDesc[0].TargetColor = _float3(5.f, 7.f, 92.f);
	m_ArrParticleDesc[0].TargetColor2 = _float3(48.f, 175.f, 53.f);

	m_ArrParticleDesc[0].m_bIsUI = false;
	m_ArrParticleDesc[0].ParticleStartRandomPosMin = _float3(-1.f, -0.2f, -1.f);
	m_ArrParticleDesc[0].ParticleStartRandomPosMax = _float3(1.0f, 0.1f, 1.f);

	m_ArrParticleDesc[0].vUp = _float3(0, 1, 0);
	m_ArrParticleDesc[0].MustDraw = false;

	m_ArrParticleDesc[0].IsParticleFameEndtoDie = false;
	m_ArrParticleDesc[0].AlphaBlendON = true;
	m_ArrParticleDesc[0].bSubPraticle = false;
	m_ArrParticleDesc[0].m_fAlphaTestValue = 50;


	m_ArrParticleDesc[1].eParticleID = Particle_Straight;
	m_ArrParticleDesc[1].TotalParticleTime = 3000.5f;
	m_ArrParticleDesc[1].EachParticleLifeTime = 0.5f;
	m_ArrParticleDesc[1].ParticleSize = _float3(1.f, 1.2f, 1.f);
	m_ArrParticleDesc[1].Particle_Power = 1;
	m_ArrParticleDesc[1].PowerRandomRange = _float2(0.2f, 1.f);
	m_ArrParticleDesc[1].MaxParticleCount = 15;
	m_ArrParticleDesc[1].szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_DeathTree_Effect");
	m_ArrParticleDesc[1].szTextureLayerTag = TEXT("DeathTree_Effect");
	m_ArrParticleDesc[1].m_bIsTextureAutoFrame = true;
	m_ArrParticleDesc[1].fAutoFrameMul = 6.f;
	m_ArrParticleDesc[1].FollowingTarget = nullptr;
	m_ArrParticleDesc[1].FixedTarget = m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(-1.5f, 1.75f, 0);
	m_ArrParticleDesc[1].MaxBoundary = _float3(2, 20, 2);
	m_ArrParticleDesc[1].ParticleColorChage = true;
	m_ArrParticleDesc[1].TargetColor = _float3(5.f, 7.f, 92.f);
	m_ArrParticleDesc[1].TargetColor2 = _float3(48.f, 175.f, 53.f);
	m_ArrParticleDesc[1].m_bIsUI = false;
	m_ArrParticleDesc[1].ParticleStartRandomPosMin = _float3(-0.1f, 0.1f, -1.5f);
	m_ArrParticleDesc[1].ParticleStartRandomPosMax = _float3(0.1f, 1.f, 1.5f);
	m_ArrParticleDesc[1].vUp = _float3(0, -1, 0);
	m_ArrParticleDesc[1].MustDraw = false;
	m_ArrParticleDesc[1].IsParticleFameEndtoDie = false;
	m_ArrParticleDesc[1].AlphaBlendON = true;
	m_ArrParticleDesc[1].bSubPraticle = false;
	m_ArrParticleDesc[1].m_fAlphaTestValue = 50;




	m_ArrParticleDesc[2].eParticleID = Particle_Straight;
	m_ArrParticleDesc[2].TotalParticleTime = 3000.5f;
	m_ArrParticleDesc[2].EachParticleLifeTime = 0.5f;
	m_ArrParticleDesc[2].ParticleSize = _float3(1.f, 1.2f, 1.f);
	m_ArrParticleDesc[2].Particle_Power = 1;
	m_ArrParticleDesc[2].PowerRandomRange = _float2(0.2f, 1.f);
	m_ArrParticleDesc[2].MaxParticleCount = 15;
	m_ArrParticleDesc[2].szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_DeathTree_Effect");
	m_ArrParticleDesc[2].szTextureLayerTag = TEXT("DeathTree_Effect");
	m_ArrParticleDesc[2].m_bIsTextureAutoFrame = true;
	m_ArrParticleDesc[2].fAutoFrameMul = 6.f;
	m_ArrParticleDesc[2].FollowingTarget = nullptr;
	m_ArrParticleDesc[2].FixedTarget = m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(1.5f, 1.75f, 0);
	m_ArrParticleDesc[2].MaxBoundary = _float3(2, 20, 2);
	m_ArrParticleDesc[2].ParticleColorChage = true;
	m_ArrParticleDesc[2].TargetColor = _float3(5.f, 7.f, 92.f);
	m_ArrParticleDesc[2].TargetColor2 = _float3(48.f, 175.f, 53.f);
	m_ArrParticleDesc[2].m_bIsUI = false;
	m_ArrParticleDesc[2].ParticleStartRandomPosMin = _float3(-0.1f, 0.1f, -1.5f);
	m_ArrParticleDesc[2].ParticleStartRandomPosMax = _float3(0.1f, 1.f, 1.5f);
	m_ArrParticleDesc[2].vUp = _float3(0, -1, 0);
	m_ArrParticleDesc[2].MustDraw = false;
	m_ArrParticleDesc[2].IsParticleFameEndtoDie = false;
	m_ArrParticleDesc[2].AlphaBlendON = true;
	m_ArrParticleDesc[2].bSubPraticle = false;

	m_ArrParticleDesc[2].m_fAlphaTestValue = 50;




	m_ArrParticleDesc[3].eParticleID = Particle_Straight;
	m_ArrParticleDesc[3].TotalParticleTime = 3000.5f;
	m_ArrParticleDesc[3].EachParticleLifeTime = 0.5f;
	m_ArrParticleDesc[3].ParticleSize = _float3(1.f, 1.2f, 1.f);
	m_ArrParticleDesc[3].Particle_Power = 1;
	m_ArrParticleDesc[3].PowerRandomRange = _float2(0.2f, 1.f);
	m_ArrParticleDesc[3].MaxParticleCount = 15;
	m_ArrParticleDesc[3].szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_DeathTree_Effect");
	m_ArrParticleDesc[3].szTextureLayerTag = TEXT("DeathTree_Effect");
	m_ArrParticleDesc[3].m_bIsTextureAutoFrame = true;
	m_ArrParticleDesc[3].fAutoFrameMul = 6.f;
	m_ArrParticleDesc[3].FollowingTarget = nullptr;
	m_ArrParticleDesc[3].FixedTarget = m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1.75f, -1.5f);
	m_ArrParticleDesc[3].MaxBoundary = _float3(2, 20, 2);
	m_ArrParticleDesc[3].ParticleColorChage = true;
	m_ArrParticleDesc[3].TargetColor = _float3(5.f, 7.f, 92.f);
	m_ArrParticleDesc[3].TargetColor2 = _float3(48.f, 175.f, 53.f);
	m_ArrParticleDesc[3].m_bIsUI = false;
	m_ArrParticleDesc[3].ParticleStartRandomPosMin = _float3(-1.5f, 0.1f, -0.1f);
	m_ArrParticleDesc[3].ParticleStartRandomPosMax = _float3(1.5f, 1.f, 0.1f);
	m_ArrParticleDesc[3].vUp = _float3(0, -1, 0);
	m_ArrParticleDesc[3].MustDraw = false;
	m_ArrParticleDesc[3].IsParticleFameEndtoDie = false;
	m_ArrParticleDesc[3].AlphaBlendON = true;
	m_ArrParticleDesc[3].bSubPraticle = false;

	m_ArrParticleDesc[3].m_fAlphaTestValue = 50;




	m_ArrParticleDesc[4].eParticleID = Particle_Straight;
	m_ArrParticleDesc[4].TotalParticleTime = 3000.5f;
	m_ArrParticleDesc[4].EachParticleLifeTime = 0.5f;
	m_ArrParticleDesc[4].ParticleSize = _float3(1.f, 1.2f, 1.f);
	m_ArrParticleDesc[4].Particle_Power = 1;
	m_ArrParticleDesc[4].PowerRandomRange = _float2(0.2f, 1.f);
	m_ArrParticleDesc[4].MaxParticleCount = 15;
	m_ArrParticleDesc[4].szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_DeathTree_Effect");
	m_ArrParticleDesc[4].szTextureLayerTag = TEXT("DeathTree_Effect");
	m_ArrParticleDesc[4].m_bIsTextureAutoFrame = true;
	m_ArrParticleDesc[4].fAutoFrameMul = 6.f;
	m_ArrParticleDesc[4].FollowingTarget = nullptr;
	m_ArrParticleDesc[4].FixedTarget = m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1.75f, 1.5f);
	m_ArrParticleDesc[4].MaxBoundary = _float3(2, 20, 2);
	m_ArrParticleDesc[4].ParticleColorChage = true;
	m_ArrParticleDesc[4].TargetColor = _float3(5.f, 7.f, 92.f);
	m_ArrParticleDesc[4].TargetColor2 = _float3(48.f, 175.f, 53.f);
	m_ArrParticleDesc[4].m_bIsUI = false;
	m_ArrParticleDesc[4].ParticleStartRandomPosMin = _float3(-1.5f, 0.1f, -0.1f);
	m_ArrParticleDesc[4].ParticleStartRandomPosMax = _float3(1.5f, 1.f, 0.1f);
	m_ArrParticleDesc[4].vUp = _float3(0, -1, 0);
	m_ArrParticleDesc[4].MustDraw = false;
	m_ArrParticleDesc[4].IsParticleFameEndtoDie = false;
	m_ArrParticleDesc[4].AlphaBlendON = true;
	m_ArrParticleDesc[4].bSubPraticle = false;

	m_ArrParticleDesc[4].m_fAlphaTestValue = 50;


	for (_uint i = 0; i < 5; i++)
	{

		GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, m_ArrParticleDesc[i]);
	}



		return S_OK;
}

CParsedObject_TreeOfDeath * CParsedObject_TreeOfDeath::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParsedObject_TreeOfDeath* pInstance = new CParsedObject_TreeOfDeath(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_TreeOfDeath");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CParsedObject_TreeOfDeath::Clone(void * pArg)
{
	CParsedObject_TreeOfDeath* pInstance = new CParsedObject_TreeOfDeath((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_TreeOfDeath");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CParsedObject_TreeOfDeath::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
