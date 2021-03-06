#include "stdafx.h"
#include "..\public\Object_VanishCube.h"


CObject_VanishCube::CObject_VanishCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_VanishCube::CObject_VanishCube(const CObject_VanishCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_VanishCube::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_VanishCube::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));

	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		memcpy(&m_fTempPos, pArg, sizeof(_float3)); //원래 자리로 돌아가기 위한 템프 포지션
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSettingPoint);
		m_Layer_Tag=(TEXT("Layer_VanishCube"));

		m_ComTexture->Change_TextureLayer(TEXT("AppearCube"));
	}

	return S_OK;
}

_int CObject_VanishCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	//사라진 큐브가 다시 나타나기 위한 예외처리
	_float3 VanishCubePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
	if (m_bCollisionSwitch == true)
	{
		m_bCollisionSwitch = false;
	}
	else if(m_bCollisionSwitch == false && m_fTempPos == VanishCubePos)
	{
		m_fTimer = 0;
	}
	
	if (m_fTimer > 0.5f)
	{
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, m_fTempPos);
	}
	else if(m_fTempPos != VanishCubePos)
	{
		m_fTimer += fTimeDelta;
	}


	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CObject_VanishCube::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;

	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS)))
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CObject_VanishCube::Render()
{
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_ComVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CObject_VanishCube::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_VanishCube::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TAG_LAY(Layer_Player)))
	{
 		Collision_Vanish(fDeltaTime);


	}

	return _int();
}

_int CObject_VanishCube::Collision_Vanish(_float fDeltaTime)
{

	m_bCollisionSwitch = true;

	m_fTimer += fDeltaTime;

	if (m_fTimer > 0.5f)
	{
		GetSingle(CGameInstance)->PlaySound(L"EH_VanishCube_1.wav", CHANNEL_OBJECT);
		//GetSingle(CGameInstance)->PlaySound(L"EH_VanishCube_2.wav", CHANNEL_OBJECT);
		Set_Particle();
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, NOT_EXIST_BLOCK);

		m_fTimer = 0;
	};



	return _int();
}


void CObject_VanishCube::Set_VenishDesc(void * pArg)
{
	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		memcpy(&m_fTempPos, pArg, sizeof(_float3)); //원래 자리로 돌아가기 위한 템프 포지션
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, NOT_EXIST_BLOCK);
		m_Layer_Tag = (TEXT("Layer_AppearCube"));
	}
}

void CObject_VanishCube::Set_Particle()
{
	PARTICLEDESC tDesc;
	//파티클이 흩날리는 종류 설정
	tDesc.eParticleID = Particle_Straight;

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 0.7f;

	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 1.2f;

	//파티클의 사이즈를 설정
	tDesc.ParticleSize = _float3(1.f, 1.f, 1.f);
	//파티클의 파워(이동속도)를 결정
	tDesc.Particle_Power = 10;
	//파티클의 파워(이동속도)의 랜덤 범위를 결정
	tDesc.PowerRandomRange = _float2(0.5f, 1.f);
	//파티클이 한번에 최대 몇개까지 보일 것인지 설정
	tDesc.MaxParticleCount = 15;

	//파티클 텍스처 컴포넌트 이름을 설정 (기본적으로 자기 씬에 컴포넌트가 있는지 검사하고 스테틱에있는지도 검사함)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("Particle_Rectangle");
	//텍스처 오토프레임을 사용할 것인지 말 것인지 결정
	tDesc.m_bIsTextureAutoFrame = false;


	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	//tDesc.FollowingTarget = m_ComTransform;
	tDesc.FixedTarget = m_fTempPos;


	//파티클의 최대 이탈 범위(range)를 설정해 줌 
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	tDesc.MaxBoundary = _float3(3, 3, 3);


	//텍스처의 색상을 변경할 수 있는 기능 온오프
	//만약 true로 사용할 경우 텍스처의 원래 색상은 무시되고 타겟 색상으로 반짝반짝 거리게 설정됨
	//true로 사용할 경우 반드시 타겟 컬러를 설정해 줄 것
	tDesc.ParticleColorChage = true;
	tDesc.TargetColor = _float3(255, 26, 31);
	tDesc.TargetColor2 = _float3(125.f, 34.f, 145.f);


	//만약 UI에 그려져야한다면 true 월드에 그려져야한다면 false 로 설정할 것
	//UI 로 그리게 될 경우 위의 모든 좌표는 API 좌표 기준으로 셋팅할 것
	//World로 그리게 될 경우 위의 모든 좌표는 월드 좌표 기준으로 셋팅할 것
	tDesc.m_bIsUI = false;
	//UI Depth 설정 (UI 가려지는거 순서 결정할때 쓰는 변수)
	tDesc.m_bUIDepth = 1;

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;

	tDesc.vUp = _float3(0, -1, 0);

	tDesc.ParticleStartRandomPosMin = _float3(-0.5f, -0.5f, -0.5f);
	tDesc.ParticleStartRandomPosMax = _float3(0.5f, 0.5f, 0.5f);

	tDesc.MustDraw = true;
	//오브젝트 뒤에 가려지지 않게 만듬

	//tDesc.IsParticleFameEndtoDie = false;
	//프레임이 한번만 돌것인지 정함

	//Create_ParticleObject를 호출하여 스테이지 아이디와 지금까지 설정한 desc를 넣어주면 됨
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);
}

HRESULT CObject_VanishCube::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Cube_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	///////////////////////////////////////////////////////
	/* For.Com_Collision */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////
	return S_OK;
}

HRESULT CObject_VanishCube::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CObject_VanishCube::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	return S_OK;
}

CObject_VanishCube * CObject_VanishCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_VanishCube* pInstance = new CObject_VanishCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_VanishCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_VanishCube::Clone(void * pArg)
{
	CObject_VanishCube* pInstance = new CObject_VanishCube((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_VanishCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_VanishCube::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
