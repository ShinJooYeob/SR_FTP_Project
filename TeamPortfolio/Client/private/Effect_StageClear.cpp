#include "stdafx.h"
#include "..\public\Effect_StageClear.h"



CEffect_StageClear::CEffect_StageClear(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CEffect_StageClear::CEffect_StageClear(const CEffect_StageClear & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_StageClear::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CEffect_StageClear::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;



	if (pArg == nullptr)
		return E_FAIL;

	_float4 TempArg;
	memcpy(&TempArg, pArg, sizeof(_float4));

	_float3 vPlayerPos = _float3(TempArg.x, TempArg.y, TempArg.z);
	m_iAcheiveStarNum = TempArg.w;

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vPlayerPos);



	for (_uint i = 0 ; i< m_iAcheiveStarNum; i++)
	{
		STAROBJATT ObjAtt;
		ObjAtt.vPos = ObjAtt.vPlayerPos = vPlayerPos;
		ObjAtt.fDegreeAngle = 360 / m_iAcheiveStarNum *i;
		ObjAtt.fAge = ((i) - _float (m_iAcheiveStarNum)  ) * 0.3f;
		ObjAtt.LifeTime = 5;
		ObjAtt.fDist = 6;
		ObjAtt.bIsDead = false;

		ObjAtt.vTargetPos = Caculate_TargetPos(ObjAtt.vPlayerPos, ObjAtt.fDegreeAngle, ObjAtt.fDist);

		ObjAtt.vTargetPos.y += (ObjAtt.fDist  * 2.f) / _float(m_iAcheiveStarNum) * ((_float(m_iAcheiveStarNum) * 0.5f - i));

		m_StarObjectList.push_back(ObjAtt);
	}

	
	m_ParticleDesc.eParticleID = Particle_Ball;
	m_ParticleDesc.TotalParticleTime = 0.0f;
	m_ParticleDesc.EachParticleLifeTime = 0.2f;
	m_ParticleDesc.ParticleSize = _float3(0.3f, 0.3f, 0.3f);
	m_ParticleDesc.Particle_Power = 1;
	m_ParticleDesc.PowerRandomRange = _float2(0.5f, 1.5f);
	m_ParticleDesc.MaxParticleCount = 4;
	m_ParticleDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Effect_StageClear");
	m_ParticleDesc.szTextureLayerTag = TEXT("Particle");
	m_ParticleDesc.m_bIsTextureAutoFrame = true;
	m_ParticleDesc.fAutoFrameMul = 2.f;

	m_ParticleDesc.MaxBoundary = _float3(10, 10, 10);
	m_ParticleDesc.ParticleColorChage = false;

	m_ParticleDesc.ParticleStartRandomPosMin = _float3(-0.2f, -0.2f, -0.2f);
	m_ParticleDesc.ParticleStartRandomPosMax = _float3(0.2f, 0.2f, 0.2f);


	m_ParticleDesc.m_bIsUI = false;
	/*tDesc.vUp = _float3(-1, -1, 0);*/

	return S_OK;
}

_int CEffect_StageClear::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	//오브젝트 갱신
	if (m_iChecker < m_iAcheiveStarNum)
	{
		auto iter = m_StarObjectList.begin();
		for (; iter != m_StarObjectList.end(); iter++)
		{
			if (iter->fAge < 0)
			{
				iter->fAge += fTimeDelta;

				if (iter->fAge > -0.3f)
				{
					iter->vPos = EaseingFloat3(TYPE_Linear, iter->vPlayerPos, iter->vTargetPos, iter->fAge + 0.3f, 0.3f);
				}
				if (iter->fAge >= 0)
				{
					iter->fAge = 0;
					m_iChecker++;
				}
			}
		}
	}
	else if(m_iChecker < m_iAcheiveStarNum * 2.f)
	{


		auto iter = m_StarObjectList.begin();

		CGameInstance* pInstance = GetSingle(CGameInstance);

		for (_uint i =0; i<m_StarObjectList.size();i++)
		{
			if (!(iter->bIsDead))
			{
				iter->fAge += fTimeDelta;
				iter->fDist = pInstance->Easing(TYPE_Linear, 6, 0, iter->fAge, iter->LifeTime);
				iter->fDegreeAngle -= fTimeDelta * 360.f;

				iter->vPos = Caculate_TargetPos(iter->vPlayerPos, iter->fDegreeAngle, iter->fDist);
				iter->vPos.y += (iter->fDist  * 2.f) / _float(m_iAcheiveStarNum) * ((_float(m_iAcheiveStarNum) * 0.5f - i));


				m_ParticleDesc.FixedTarget = iter->vPos;
				GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, m_ParticleDesc);

				if (iter->fAge > iter->LifeTime)
				{
					iter->bIsDead = true;
					m_iChecker++;
				}
			}

			iter++;
		}
	}
	else 
	{
		DIED();
	}

	return _int();
}

_int CEffect_StageClear::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);


	return _int();
}

_int CEffect_StageClear::Render()
{
	if (nullptr == m_ComVIBuffer || m_ComTransform == nullptr)
		return E_FAIL;



	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(g_fDeltaTime)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;
	
	auto iter = m_StarObjectList.begin();
	for (; iter != m_StarObjectList.end();iter++)
	{

		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, iter->vPos);

		if (FAILED(m_ComTransform->Bind_WorldMatrix_Look_Camera()))
			return E_FAIL;
		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;
	}


	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CEffect_StageClear::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;




	return _int();
}

_int CEffect_StageClear::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}

HRESULT CEffect_StageClear::SetUp_Components()
{	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Texture_Effect_StageClear"), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
		return E_FAIL;


	return S_OK;
}



HRESULT CEffect_StageClear::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 32);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CEffect_StageClear::Release_RenderState()
{

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	return S_OK;
}

_float3 CEffect_StageClear::Caculate_TargetPos(_float3 vPlayerPos, _float fDegree, _float fDistance)
{
	_float3 Result;


	Result.x = cosf(D3DXToRadian(fDegree))*fDistance + vPlayerPos.x;
	Result.z = sinf(D3DXToRadian(fDegree))*fDistance + vPlayerPos.z;
	Result.y = vPlayerPos.y;


	return Result;
}

_float3 CEffect_StageClear::EaseingFloat3(EasingTypeID id, _float3 StartPos, _float3 EndPos, _float curTime, _float maxTime)
{
	_float3 newPos = _float3(0, 0, 0);
	newPos.x = GetSingle(CGameInstance)->Easing(id, StartPos.x, EndPos.x, curTime, maxTime);
	newPos.y = GetSingle(CGameInstance)->Easing(id, StartPos.y, EndPos.y, curTime, maxTime);
	newPos.z = GetSingle(CGameInstance)->Easing(id, StartPos.z, EndPos.z, curTime, maxTime);

	return newPos;
}

CEffect_StageClear * CEffect_StageClear::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CEffect_StageClear* pInstance = new CEffect_StageClear(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CEffect_StageClear");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CEffect_StageClear::Clone(void * pArg)
{
	CEffect_StageClear* pInstance = new CEffect_StageClear((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CEffect_StageClear");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CEffect_StageClear::Free()
{
	__super::Free();

	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
