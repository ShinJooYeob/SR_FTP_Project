#include "stdafx.h"
#include "..\public\ParsedObject_Blossoms.h"


CParsedObject_Blossoms::CParsedObject_Blossoms(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParsedObject(pGraphic_Device)
{
}

CParsedObject_Blossoms::CParsedObject_Blossoms(const CParsedObject_Blossoms & rhs)
	: CParsedObject(rhs)
{
}

HRESULT CParsedObject_Blossoms::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParsedObject_Blossoms::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		m_Layer_Tag = (TEXT("Layer_Blossoms"));
		m_ComTransform->Scaled(_float3(1.35f, 1.35f, 1.35f));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSettingPoint);
	}

	FAILED_CHECK(m_ComTexture->Change_TextureLayer(TEXT("Blossoms")));

	Set_Particle_Cloud();
	Set_Particle_Fog();
	Set_Particle_Blossom();

	return S_OK;
}

_int CParsedObject_Blossoms::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;



	return _int();
}

_int CParsedObject_Blossoms::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS),8))
		m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CParsedObject_Blossoms::Render()
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

_int CParsedObject_Blossoms::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CParsedObject_Blossoms::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}


HRESULT CParsedObject_Blossoms::SetUp_Components()
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
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Blossoms"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
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

HRESULT CParsedObject_Blossoms::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CParsedObject_Blossoms::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	///////////////////////////////

	return S_OK;
}

HRESULT CParsedObject_Blossoms::Set_Particle_Cloud()
{
	return E_NOTIMPL;
}

HRESULT CParsedObject_Blossoms::Set_Particle_Fog()
{
	PARTICLEDESC tDesc;
	tDesc.eParticleID = Particle_Straight;
	tDesc.TotalParticleTime = 3600.f;
	tDesc.EachParticleLifeTime = 10.f;
	tDesc.ParticleSize = _float3(40.f, 40.f, 40.f);
	tDesc.Particle_Power = 0.2;
	tDesc.PowerRandomRange = _float2(0.5f, 1.5f);
	tDesc.MaxParticleCount = 3;
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	tDesc.szTextureLayerTag = TEXT("Particle_Fog");
	tDesc.m_bIsTextureAutoFrame = false;
	//tDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player))->Get_Component(TAG_COM(Com_Transform)));
	_float3 Pos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
	tDesc.FixedTarget = Pos;
	tDesc.MaxBoundary = _float3(30, 30, 30);
	tDesc.ParticleColorChage = true;
	tDesc.TargetColor = _float3(237, 186, 186);
	tDesc.TargetColor2 = _float3(200.f, 192.f, 231.f);
	tDesc.ParticleStartRandomPosMin = _float3(-3, -2, -3);
	tDesc.ParticleStartRandomPosMax = _float3(2, 0, 2);


	tDesc.m_bIsUI = false;
	tDesc.vUp = _float3(1.f, 0.f, 1.f);

	tDesc.MustDraw = true;
	tDesc.AlphaBlendON = true;
	tDesc.m_fAlphaTestValue = 0.f;

	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	tDesc.vUp = _float3(-1.f, 0.f, -1.f);
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);
	return S_OK;
}

HRESULT CParsedObject_Blossoms::Set_Particle_Blossom()
{
	PARTICLEDESC tDesc;
	tDesc.eParticleID = Particle_Straight;
	tDesc.TotalParticleTime = 3600.f;
	tDesc.EachParticleLifeTime = 5.0f;
	tDesc.ParticleSize = _float3(0.3f, 0.3f, 0.3f);
	tDesc.Particle_Power = 2;
	tDesc.PowerRandomRange = _float2(0.5f, 1.5f);
	tDesc.MaxParticleCount = 70;
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	tDesc.szTextureLayerTag = TEXT("greenleaf");
	tDesc.m_bIsTextureAutoFrame = false;
	_float3 Pos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
	tDesc.FixedTarget = Pos;
	tDesc.MaxBoundary = _float3(30, 10, 30);
	tDesc.ParticleColorChage = true;
	tDesc.TargetColor = _float3(237, 186, 186);
	tDesc.TargetColor2 = _float3(200.f, 192.f, 231.f);
	tDesc.ParticleStartRandomPosMin = _float3(-15, -10, -15);
	tDesc.ParticleStartRandomPosMax = _float3(15, 10, 15);

	tDesc.MustDraw = true;
	tDesc.m_bIsUI = false;
	tDesc.vUp = _float3(1.f, -1.f, 1.f);

	GetSingle(CParticleMgr)->Create_ParticleObject(SCENEID::SCENE_STAGE1, tDesc);

	tDesc.TargetColor = _float3(255.f, 149.f, 184.f);
	tDesc.TargetColor2 = _float3(255.f, 91.f, 145.f);
	tDesc.vUp = _float3(-1.f, -1.f, -1.f);
	GetSingle(CParticleMgr)->Create_ParticleObject(SCENEID::SCENE_STAGE1, tDesc);

	tDesc.MaxParticleCount = 10;
	tDesc.TargetColor = _float3(255.f, 151.f, 250.f);
	tDesc.TargetColor2 = _float3(255.f, 151.f, 250.f);
	tDesc.vUp = _float3(-1.f, 1.f, -1.f);
	GetSingle(CParticleMgr)->Create_ParticleObject(SCENEID::SCENE_STAGE1, tDesc);


	return S_OK;
}

CParsedObject_Blossoms * CParsedObject_Blossoms::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParsedObject_Blossoms* pInstance = new CParsedObject_Blossoms(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_Blossoms");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CParsedObject_Blossoms::Clone(void * pArg)
{
	CParsedObject_Blossoms* pInstance = new CParsedObject_Blossoms((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_Blossoms");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CParsedObject_Blossoms::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
