#include "stdafx.h"
#include "..\public\Object_EscalatorCube.h"


CObject_EscalatorCube::CObject_EscalatorCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_EscalatorCube::CObject_EscalatorCube(const CObject_EscalatorCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_EscalatorCube::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;



	return S_OK;
}

HRESULT CObject_EscalatorCube::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));

	if (pArg != nullptr) {


		memcpy(&m_EscalatorDesc, pArg, sizeof(ESCALATORDESC));

		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, m_EscalatorDesc.vStartPos);

		CTransform::TRANSFORMDESC		TransformDesc;
		TransformDesc.fMovePerSec = m_EscalatorDesc.vStartPos.Get_Distance(m_EscalatorDesc.vEndPos) *0.25f;
		TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

		m_ComTransform->Set_TransformDesc(TransformDesc);

		m_Layer_Tag = TEXT("Layer_EscalatorCube");

		m_ComTexture->Change_TextureLayer(TEXT("EscalatorCube"));
	}


	//m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(-1.f, 2.f, 2.f));

	return S_OK;
}

_int CObject_EscalatorCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float3 RisingPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
	if (m_bCollisionSwitch == true)
	{
		m_bCollisionSwitch = false;
	}
	else if (m_EscalatorDesc.vStartPos.Get_Distance(RisingPos) > 0.2f)
	{
		//속도조절 가능

		m_ComTransform->MovetoTarget(m_EscalatorDesc.vStartPos, fTimeDelta);

		ParticleTime += fTimeDelta;
		if (ParticleTime > 0.2f)
		{
			Return_Particle();
			ParticleTime = 0;
		}

		//_float3 RisingCubePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
		//m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(RisingCubePos.x, RisingCubePos.y + m_fTimer, RisingCubePos.z));
	}

	if (m_bSoundCollison == true)
	{
		m_bSoundCollison = false;
	}
	else if (m_bSoundSwitch == true)
	{
		GetSingle(CGameInstance)->PlaySound(L"EH_EscalatorCube_Retun.wav", CHANNEL_OBJECT, 2.f);
		m_bSoundSwitch = false;
	}

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CObject_EscalatorCube::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;

	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS)))
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CObject_EscalatorCube::Render()
{
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture(1)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_ComVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CObject_EscalatorCube::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_EscalatorCube::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TAG_LAY(Layer_Player))||!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_FixCube")))
	{
		Collision_Descent(pDestObjects, fCollision_Distance, fDeltaTime);

		ParticleTime += fDeltaTime;
		if (ParticleTime > 0.2f)
		{
			Set_Particle();
			ParticleTime = 0;
		}
		if(m_bSoundCollison == false)
		{
			if (m_bSoundSwitch == false)
			{
				GetSingle(CGameInstance)->PlaySound(L"EH_EscalatorCube_advance.wav", CHANNEL_OBJECT, 2.f);

				m_bSoundSwitch = true;
			}

			m_bSoundCollison = true;
		}
	}

	return _int();
}

_int CObject_EscalatorCube::Collision_Descent(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	m_bCollisionSwitch = true;

	_float3 RisingCubePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

	if (m_EscalatorDesc.vEndPos.Get_Distance(RisingCubePos) > 0.2f ) 
	{
		CTransform* DestTransform = (CTransform*)pDestObjects->Get_Component(TAG_COM(Com_Transform));
		_float3 CubeToTargetDist = DestTransform->Get_MatrixState(CTransform::STATE_POS) - RisingCubePos;
		m_ComTransform->MovetoTarget(m_EscalatorDesc.vEndPos, fDeltaTime);

		RisingCubePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);



		DestTransform->Set_MatrixState(CTransform::STATE_POS, RisingCubePos + CubeToTargetDist);

	}
	return _int();
}

void CObject_EscalatorCube::Set_Particle()
{
	PARTICLEDESC tDesc;
	//파티클이 흩날리는 종류 설정
	tDesc.eParticleID = Particle_Fixed;

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 0.f;

	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 2.7f;

	//파티클의 사이즈를 설정
	tDesc.ParticleSize = _float3(1.f, 1.f, 1.f);
	//파티클의 파워(이동속도)를 결정
	tDesc.Particle_Power = 2;
	//파티클의 파워(이동속도)의 랜덤 범위를 결정
	tDesc.PowerRandomRange = _float2(1.f, 1.1f);
	//파티클이 한번에 최대 몇개까지 보일 것인지 설정
	tDesc.MaxParticleCount = 1;

	//파티클 텍스처 컴포넌트 이름을 설정 (기본적으로 자기 씬에 컴포넌트가 있는지 검사하고 스테틱에있는지도 검사함)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("Particle_Rect");
	//텍스처 오토프레임을 사용할 것인지 말 것인지 결정
	tDesc.m_bIsTextureAutoFrame = true;
	tDesc.fAutoFrameMul = 1.5;


	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	tDesc.FollowingTarget = m_ComTransform;
	//tDesc.FixedTarget = _float3(0,3,0);


	//파티클의 최대 이탈 범위(range)를 설정해 줌 
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	tDesc.MaxBoundary = _float3(3, 3, 3);


	//텍스처의 색상을 변경할 수 있는 기능 온오프
	//만약 true로 사용할 경우 텍스처의 원래 색상은 무시되고 타겟 색상으로 반짝반짝 거리게 설정됨
	//true로 사용할 경우 반드시 타겟 컬러를 설정해 줄 것
	tDesc.ParticleColorChage = true;
	tDesc.TargetColor = _float3(241, 181, 241);
	tDesc.TargetColor2 = _float3(200.f, 191.f, 231.f);


	//만약 UI에 그려져야한다면 true 월드에 그려져야한다면 false 로 설정할 것
	//UI 로 그리게 될 경우 위의 모든 좌표는 API 좌표 기준으로 셋팅할 것
	//World로 그리게 될 경우 위의 모든 좌표는 월드 좌표 기준으로 셋팅할 것
	tDesc.m_bIsUI = false;
	//UI Depth 설정 (UI 가려지는거 순서 결정할때 쓰는 변수)
	tDesc.m_bUIDepth = 1;

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;

	tDesc.vUp = m_EscalatorDesc.vStartPos - m_EscalatorDesc.vEndPos;

	tDesc.ParticleStartRandomPosMin = _float3(0.f, 0.f, 0.f);
	tDesc.ParticleStartRandomPosMax = _float3(0.f, 0.f, 0.f);

	tDesc.MustDraw = false;
	//오브젝트 뒤에 가려지지 않게 만듬

	tDesc.IsParticleFameEndtoDie = true;
	//프레임이 한번만 돌것인지 정함


	//Create_ParticleObject를 호출하여 스테이지 아이디와 지금까지 설정한 desc를 넣어주면 됨
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);
}

void CObject_EscalatorCube::Return_Particle()
{
	PARTICLEDESC tDesc;
	//파티클이 흩날리는 종류 설정
	tDesc.eParticleID = Particle_Fixed;

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 0.f;

	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 2.7f;

	//파티클의 사이즈를 설정
	tDesc.ParticleSize = _float3(1.f, 1.f, 1.f);
	//파티클의 파워(이동속도)를 결정
	tDesc.Particle_Power = 2;
	//파티클의 파워(이동속도)의 랜덤 범위를 결정
	tDesc.PowerRandomRange = _float2(1.f, 1.1f);
	//파티클이 한번에 최대 몇개까지 보일 것인지 설정
	tDesc.MaxParticleCount = 1;

	//파티클 텍스처 컴포넌트 이름을 설정 (기본적으로 자기 씬에 컴포넌트가 있는지 검사하고 스테틱에있는지도 검사함)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("Particle_Rect");
	//텍스처 오토프레임을 사용할 것인지 말 것인지 결정
	tDesc.m_bIsTextureAutoFrame = true;
	tDesc.fAutoFrameMul = 1.5;


	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	tDesc.FollowingTarget = m_ComTransform;
	//tDesc.FixedTarget = _float3(0,3,0);


	//파티클의 최대 이탈 범위(range)를 설정해 줌 
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	tDesc.MaxBoundary = _float3(3, 3, 3);


	//텍스처의 색상을 변경할 수 있는 기능 온오프
	//만약 true로 사용할 경우 텍스처의 원래 색상은 무시되고 타겟 색상으로 반짝반짝 거리게 설정됨
	//true로 사용할 경우 반드시 타겟 컬러를 설정해 줄 것
	tDesc.ParticleColorChage = true;
	tDesc.TargetColor = _float3(176.f, 249.f, 174.f);
	tDesc.TargetColor2 = _float3(240.f, 206.f, 183.f);


	//만약 UI에 그려져야한다면 true 월드에 그려져야한다면 false 로 설정할 것
	//UI 로 그리게 될 경우 위의 모든 좌표는 API 좌표 기준으로 셋팅할 것
	//World로 그리게 될 경우 위의 모든 좌표는 월드 좌표 기준으로 셋팅할 것
	tDesc.m_bIsUI = false;
	//UI Depth 설정 (UI 가려지는거 순서 결정할때 쓰는 변수)
	tDesc.m_bUIDepth = 1;

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;

	tDesc.vUp = m_EscalatorDesc.vStartPos - m_EscalatorDesc.vEndPos;

	tDesc.ParticleStartRandomPosMin = _float3(0.f, 0.f, 0.f);
	tDesc.ParticleStartRandomPosMax = _float3(0.f, 0.f, 0.f);

	tDesc.MustDraw = false;
	//오브젝트 뒤에 가려지지 않게 만듬

	tDesc.IsParticleFameEndtoDie = true;
	//프레임이 한번만 돌것인지 정함


	//Create_ParticleObject를 호출하여 스테이지 아이디와 지금까지 설정한 desc를 넣어주면 됨
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);
}


HRESULT CObject_EscalatorCube::SetUp_Components()
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

HRESULT CObject_EscalatorCube::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 32);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CObject_EscalatorCube::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	///////////////////////////////

	return S_OK;
}

CObject_EscalatorCube * CObject_EscalatorCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_EscalatorCube* pInstance = new CObject_EscalatorCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_EscalatorCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_EscalatorCube::Clone(void * pArg)
{
	CObject_EscalatorCube* pInstance = new CObject_EscalatorCube((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_EscalatorCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_EscalatorCube::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
