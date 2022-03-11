#include "stdafx.h"
#include "..\public\Object_GravityCube.h"


CObject_GravityCube::CObject_GravityCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_GravityCube::CObject_GravityCube(const CObject_GravityCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_GravityCube::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_GravityCube::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSettingPoint);
		m_Layer_Tag = TEXT("Layer_GravityCube");

		m_ComTransform->Scaled({ 3.f,3.f,3.f });
		m_ComTexture->Change_TextureLayer(L"none", 15.f);
		m_ComTexture->Change_TextureLayer(L"Gravity",15.f);
	}

	return S_OK;
}

_int CObject_GravityCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);
	m_fFrame = fTimeDelta;


	return _int();
}

_int CObject_GravityCube::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;



	//객체에게 중력을 적용하기 위한 값

	if (FAILED(Collision_Gravity(fTimeDelta)))
		return -1;

	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS),3.f))
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CObject_GravityCube::Render()
{
	if (FAILED(m_ComTransform->Bind_WorldMatrix_Look_Camera()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;


	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CObject_GravityCube::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_GravityCube::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}

_int CObject_GravityCube::Collision_Gravity(_float fDeltaTime)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//객체에게 중력을 적용하기 위한 값
	CTransform* Player = (CTransform*)pGameInstance->Get_Commponent_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player), TAG_COM(Com_Transform));

	if (Player == nullptr)
		return E_FAIL;


	_float3& PlayerPos = Player->Get_MatrixState(CTransform::STATE_POS);

	_float3& GravityCubePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

	_float Distance = GravityCubePos.Get_Distance(PlayerPos);

	if (Distance < 2.5f) //거리 조절 가능
	{
		fDeltaTime *= 0.5f; // 속도 조절 가능
		Player->MovetoTarget(GravityCubePos, fDeltaTime);

		ParticleTime += fDeltaTime;
		if (ParticleTime > 0.2f)
		{
			Set_Particle_BlackDust();
			Set_Particle_DustSmall();
			ParticleTime = 0;
		}
	}


	RELEASE_INSTANCE(CGameInstance);

	return _int();
}


HRESULT CObject_GravityCube::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.vPivot = _float3(0.f,-1.f,0.f);
	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_GravityTexture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	///////////////////////////////////////////////////////
	/* For.Com_Collision */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////
	return S_OK;
}

HRESULT CObject_GravityCube::SetUp_OnTerrain(_float fDeltaTime)
{
	return S_OK;
}

HRESULT CObject_GravityCube::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 30);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	//m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(180, 255, 255, 255));

	return S_OK;
}

HRESULT CObject_GravityCube::Release_RenderState()
{
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	return S_OK;
}

void CObject_GravityCube::Set_Particle_BlackDust()
{
	PARTICLEDESC tDesc;
	//파티클이 흩날리는 종류 설정
	tDesc.eParticleID = Particle_Suck;

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 1.f;

	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 1.0f;

	//파티클의 사이즈를 설정
	tDesc.ParticleSize = _float3(0.1f, 0.1f, 0.1f);

	//파티클의 파워(이동속도)를 결정
	tDesc.Particle_Power = 4;

	//파티클의 파워(이동속도)의 랜덤 범위를 결정
	tDesc.PowerRandomRange = _float2(0.5f, 1.5f);

	//파티클이 한번에 최대 몇개까지 보일 것인지 설정
	tDesc.MaxParticleCount = 5;

	//파티클 텍스처 컴포넌트 이름을 설정 (기본적으로 자기 씬에 컴포넌트가 있는지 검사하고 스테틱에있는지도 검사함)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");

	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("Particle_BlackDust");

	//텍스처 오토프레임을 사용할 것인지 말 것인지 결정
	tDesc.m_bIsTextureAutoFrame = false;

	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	tDesc.FollowingTarget = m_ComTransform;
	//tDesc.FixedTarget = _float3(10, 10, 1);

	//파티클의 최대 이탈 범위(range)를 설정해 줌 
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	tDesc.MaxBoundary = _float3(10, 5, 10);

	//텍스처의 색상을 변경할 수 있는 기능 온오프
	//만약 true로 사용할 경우 텍스처의 원래 색상은 무시되고 타겟 색상으로 반짝반짝 거리게 설정됨
	//true로 사용할 경우 반드시 타겟 컬러를 설정해 줄 것
	tDesc.ParticleColorChage = false;
	tDesc.TargetColor = _float3(237, 186, 186);
	tDesc.TargetColor2 = _float3(200.f, 192.f, 231.f);

	//만약 UI에 그려져야한다면 true 월드에 그려져야한다면 false 로 설정할 것
	//UI 로 그리게 될 경우 위의 모든 좌표는 API 좌표 기준으로 셋팅할 것
	//World로 그리게 될 경우 위의 모든 좌표는 월드 좌표 기준으로 셋팅할 것
	tDesc.m_bIsUI = false;

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;
	/*tDesc.vUp = _float3(-1, -1, 0);*/

	//파티클의 랜덤 설정을 Min과 Max를 정해준다.
	tDesc.ParticleStartRandomPosMin = _float3(-5, -5, -5);
	tDesc.ParticleStartRandomPosMax = _float3(5, 5, 5);

	tDesc.MustDraw = true;
	//오브젝트 뒤에 가려지지 않게 만듬

	tDesc.IsParticleFameEndtoDie = false;
	//프레임이 한번만 돌것인지 정함

	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);



	//벚꽃
	//PARTICLEDESC tDesc;
	//tDesc.eParticleID = Particle_Suck;
	//tDesc.TotalParticleTime = 3600.f;
	//tDesc.EachParticleLifeTime = 1.0f;
	//tDesc.ParticleSize = _float3(0.3f, 0.3f, 0.3f);
	//tDesc.Particle_Power = 4;
	//tDesc.PowerRandomRange = _float2(0.5f, 1.5f);
	//tDesc.MaxParticleCount = 100;
	//tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	//tDesc.szTextureLayerTag = TEXT("greenleaf");
	//tDesc.m_bIsTextureAutoFrame = false;
	//tDesc.FollowingTarget = m_ComTransform;
	////tDesc.FixedTarget = _float3(10, 10, 1);
	//tDesc.MaxBoundary = _float3(10, 5, 10);
	//tDesc.ParticleColorChage = true;
	//tDesc.TargetColor = _float3(237, 186, 186);
	//tDesc.TargetColor2 = _float3(200.f, 192.f, 231.f);
	//tDesc.ParticleStartRandomPosMin = _float3(-10, -10, -10);
	//tDesc.ParticleStartRandomPosMax = _float3(10, 10, 10);


	//tDesc.m_bIsUI = false;
	///*tDesc.vUp = _float3(-1, -1, 0);*/
	//GetSingle(CParticleMgr)->Create_ParticleObject(SCENEID::SCENE_STAGESELECT, tDesc);
}

void CObject_GravityCube::Set_Particle_DustSmall()
{
	PARTICLEDESC tDesc;
	//파티클이 흩날리는 종류 설정
	tDesc.eParticleID = Particle_Suck;

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 0.f;

	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 20.f;

	//파티클의 사이즈를 설정
	tDesc.ParticleSize = _float3(0.3f, 0.3f, 0.3f);

	//파티클의 파워(이동속도)를 결정
	tDesc.Particle_Power = 4;

	//파티클의 파워(이동속도)의 랜덤 범위를 결정
	tDesc.PowerRandomRange = _float2(0.5f, 1.5f);

	//파티클이 한번에 최대 몇개까지 보일 것인지 설정
	tDesc.MaxParticleCount = 10;

	//파티클 텍스처 컴포넌트 이름을 설정 (기본적으로 자기 씬에 컴포넌트가 있는지 검사하고 스테틱에있는지도 검사함)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");

	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("Particle_DustSmall");

	//텍스처 오토프레임을 사용할 것인지 말 것인지 결정
	tDesc.m_bIsTextureAutoFrame = true;

	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	tDesc.FollowingTarget = m_ComTransform;
	//tDesc.FixedTarget = _float3(10, 10, 1);

	//파티클의 최대 이탈 범위(range)를 설정해 줌 
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	tDesc.MaxBoundary = _float3(10, 5, 10);

	//텍스처의 색상을 변경할 수 있는 기능 온오프
	//만약 true로 사용할 경우 텍스처의 원래 색상은 무시되고 타겟 색상으로 반짝반짝 거리게 설정됨
	//true로 사용할 경우 반드시 타겟 컬러를 설정해 줄 것
	tDesc.ParticleColorChage = false;
	tDesc.TargetColor = _float3(237, 186, 186);
	tDesc.TargetColor2 = _float3(200.f, 192.f, 231.f);

	//만약 UI에 그려져야한다면 true 월드에 그려져야한다면 false 로 설정할 것
	//UI 로 그리게 될 경우 위의 모든 좌표는 API 좌표 기준으로 셋팅할 것
	//World로 그리게 될 경우 위의 모든 좌표는 월드 좌표 기준으로 셋팅할 것
	tDesc.m_bIsUI = false;

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;
	/*tDesc.vUp = _float3(-1, -1, 0);*/

	//파티클의 랜덤 설정을 Min과 Max를 정해준다.
	tDesc.ParticleStartRandomPosMin = _float3(-5, -5, -5);
	tDesc.ParticleStartRandomPosMax = _float3(5, 5, 5);

	tDesc.MustDraw = true;
	//오브젝트 뒤에 가려지지 않게 만듬

	tDesc.IsParticleFameEndtoDie = true;
	//프레임이 한번만 돌것인지 정함

	GetSingle(CParticleMgr)->Create_ParticleObject(SCENEID::SCENE_STAGESELECT, tDesc);
}

CObject_GravityCube * CObject_GravityCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_GravityCube* pInstance = new CObject_GravityCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_GravityCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_GravityCube::Clone(void * pArg)
{
	CObject_GravityCube* pInstance = new CObject_GravityCube((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_GravityCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_GravityCube::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
