#include "stdafx.h"
#include "..\public\BossMonster.h"
#include "..\public\BossParttern.h"
#include "Com_Gun.h"
#include "Player.h"
#include "Bullet.h"

#include "UI_BossStatusUI.h"
#include "ParticleObejct.h"

CBossMonster::CBossMonster(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CMonsterParent(pGraphicDevice)
{
}

CBossMonster::CBossMonster(const CBossMonster& rhs)
	: CMonsterParent(rhs)
{

}


HRESULT CBossMonster::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));
	mPlayerTarget = nullptr;
	mComGun = nullptr;	
	mCurrentPattern = nullptr;
	mMainCamera = nullptr;

	mPatternTime = 0;
	mNextPattern = 0;
	return S_OK;
}

HRESULT CBossMonster::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	mPlayerTarget = nullptr;
	mCurrentPattern = nullptr;

	m_ComTexture->Change_TextureLayer(TEXT("Idle1"));
	Set_Scale(8);

	m_pGraphicDevice->GetViewport(&mViewPort);

	auto CameraList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENE_STATIC, TAG_LAY(Layer_Camera_Main));
	mMainCamera = (CCamera_Main*)CameraList->front();
	Safe_AddRef(mMainCamera);

	mPatternTime = 0;
	mNextPattern = 0;

	mMaxHp = 10;

	m_Sphere.mRadius = 150.0f;
	
	// 보스 등장음
	GetSingle(CGameInstance)->PlaySound(TEXT("JH_Boss_Create.mp3"), CHANNEL_EFFECT, 0.8f);

	// 등장패턴
	Set_TestPattern_Create();

	return S_OK;
}

_int CBossMonster::Update(_float fDeltaTime)
{
	FAILED_CHECK(__super::Update(fDeltaTime));
	if (mbDying)
		return 0;

	if (mPlayerTarget == nullptr)
	{
		auto playerlist = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENE_STATIC, TAG_LAY(Layer_Player));
		mPlayerTarget = (CPlayer*)playerlist->front();
		Safe_AddRef(mPlayerTarget);
		if (mPlayerTarget == nullptr)
			return 0;

		// 위치 초기화
		mCameralocalPosition = _float3(-100,-100, 20);
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, Update_CameraPosition(mCameralocalPosition));


		// CTransform* trans = mPlayerTarget->Get_TransformCom();
		// _float3 playerPos = trans->Get_MatrixState(CTransform::STATE_POS);
		// playerPos.y += 1;
		// m_ComTransform->Set_MatrixState(CTransform::STATE_POS, playerPos);

	}
	mPatternTime += fDeltaTime;
	if (mPatternTime < 1)
		return 0;

	// 카메라 업데이트
	mCameralocalPosition.z = 20;
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS,Update_CameraPosition(mCameralocalPosition));


	mComGun->Update_CreateBullet(fDeltaTime);

	// AI 업데이트
	// 큐에서 패턴을 하나씩 실행한다.
	Update_BossPattern(fDeltaTime);
	
	mDefaultEffect -= fDeltaTime;
	if (mDefaultEffect < 0)
	{
		

		if (Get_IsTurn() || 
			CCamera_Main::CameraLookStateID::Look_Left_Axis == Get_CameraLookState() || 
			CCamera_Main::CameraLookStateID::Look_Right_Axis == Get_CameraLookState())
		{}
		else
			GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, CreateMonster_Default_ParticleDESC());

		//if (CCamera_Main::CameraLookStateID::Look_Front_Axis == Get_CameraLookState())
		//{
		//	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, CreateMonster_Default_ParticleDESC());

		//}
		//else if (CCamera_Main::CameraLookStateID::Look_Back_Axis == Get_CameraLookState())
		//{
		//	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, CreateMonster_Default_ParticleDESC());

		//}

	}
	return _int();
}

_int CBossMonster::LateUpdate(_float fDeltaTime)
{
	FAILED_CHECK(__super::LateUpdate(fDeltaTime));
	if (m_ComRenderer == nullptr)
		return -1;

	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	m_Sphere.mCenterPosition = m_Com_Viewport->WorldToView(GetPos());
	m_Com_Viewport->AddCollisionView(CCom_CollisionViewPort::COLL_MONSTER, this);

	return _int();
}

_int CBossMonster::Render()
{

	FAILED_CHECK(__super::Render());


	return S_OK;
}

_int CBossMonster::LateRender()
{
	FAILED_CHECK(__super::LateRender());


	return S_OK();
}

bool CBossMonster::Get_IsTurn() const 
{
	return mMainCamera->Get_bIsTuring();
}

CCamera_Main::CameraLookStateID CBossMonster::Get_CameraLookState()const
{
	return mMainCamera->Get_CameraLookState();
}

const _tchar * CBossMonster::Get_NowTextureTag() const
{
	return m_ComTexture->Get_NowTextureTag();
}

void CBossMonster::Update_Die(float deltatime)
{
	if (mbDying == false)
		return;

	// 죽음 연출 후 삭제
	mTimer -= deltatime;;
	if (mTimer <= 0)
	{
		Die();
	}
}


HRESULT CBossMonster::SetUp_Components()
{
	FAILED_CHECK(__super::SetUp_Components());

	// 총 컴포넌트 초기화
	CCom_Gun::GUNDESC desc;
	desc.mSceneID = m_eNowSceneNum;
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Gun), TAG_COM(Com_Gun), (CComponent**)&mComGun,&desc)))
		return E_FAIL;	
	mComGun->Set_MonsterObject(this);

	return S_OK;
}

HRESULT CBossMonster::SetUp_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	return S_OK;
}

HRESULT CBossMonster::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

HRESULT CBossMonster::CreateObject(_int Damage)
{
	return S_OK;
}

HRESULT CBossMonster::Hit(_int Damage)
{
	if (mbDying)
		return S_OK;

	m_BossStatusUI->Change_VersusPoint(Damage);	

	if (mMaxHp <= m_BossStatusUI->Get_TargetPotint())
	{
		mbDying = true;
		m_ComTexture->Change_TextureLayer_Wait(TEXT("Die"), 8.0f);
		GetSingle(CGameInstance)->PlaySound(TEXT("JH_Boss_Destory.wav"), CHANNEL_EFFECT);

		mTimer = FIX_DEADANIMATIONTIMER;
	}
	else
	{
		GetSingle(CGameInstance)->PlaySound(TEXT("JH_Boss_Hit.wav"), CHANNEL_EFFECT);
		m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("Hit"), TEXT("Idle1"), 1.0f);
		GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, CreateMonster_Hit_ParticleDESC());

	}

	
	return S_OK;
}

HRESULT CBossMonster::Die()
{
	DIED();
	m_BossStatusUI->Change_VersusPoint(-5);
	return S_OK;
}

void CBossMonster::Update_BossPattern(_float deltatime)
{
	
	if (mQueue_Partern.empty())
	{
		Choose_NextPattern();		
	}

	// 패턴이 끝나면 다음 패턴을 업데이트 시켜준다.

	if (mCurrentPattern == nullptr)
	{
		mCurrentPattern = mQueue_Partern.front();
		mQueue_Partern.pop();
		return;
	}

	if (mCurrentPattern->IsPatternEnd())
	{
		Safe_Release(mCurrentPattern);
		mCurrentPattern = mQueue_Partern.front();
		mQueue_Partern.pop();
	}

	// 2. 패턴 실행
	mCurrentPattern->Action(deltatime);


}



HRESULT CBossMonster::Set_TestPattern_Create()
{
	// 등장 이동 연출

	// 5초후에 오른쪽 아래~ 위 가운데 오른쪽 위 순으로 천천히 등장
	Set_MovePattern(_float2(LEFT_POS -5, BOTTOM_POS -3), 5.0f, TYPE_Linear);

	Set_MovePattern(_float2(LEFT_POS, BOTTOM_POS), 2, TYPE_SinInOut);
	Set_MovePattern(_float2(LEFT_POS, BOTTOM_POS - 3), 1, TYPE_SinInOut);
	Set_MovePattern(_float2(LEFT_POS, TOP_POS), 2, TYPE_QuadIn);
	Set_MovePattern(_float2(2, 2), 1, TYPE_QuadInOut);
	Set_MovePattern(_float2(RIGHT_POS, TOP_POS), 1, TYPE_CubicInOut);

	return S_OK;
}

HRESULT CBossMonster::Set_BossPattern1()
{
	// 상하에서 플레이어쪽으로 이동하면서 총알
	
	Set_MovePattern(NomalPos, 1.f, TYPE_SinIn);

	for (int i = 0; i <1; i++)
	{
		Set_MovePattern(CenterPos, 0.5f, TYPE_SinInOut);
		Set_MovePattern(UpFront, 0.5f, TYPE_SinInOut);

		Set_Attack_PlayerTargetPattern(
			DefaultSpawnOffset,
			2, 6, 1.f,
			BULLETTYPE_MOVE_NOMAL);
		Set_Delay(2);

		Set_MovePattern(CenterPos, 0.5f, TYPE_SinInOut);
		Set_MovePattern(BottomFront, 0.5f, TYPE_SinInOut);
		Set_Attack_PlayerTargetPattern(
			DefaultSpawnOffset,
			3, 6, 1.f,
			BULLETTYPE_MOVE_NOMAL);
		Set_Delay(3);

		Set_MovePattern(CenterPos, 0.5f, TYPE_SinInOut);
		Set_Delay(2);
	}

	return S_OK;
}
HRESULT CBossMonster::Set_BossPattern2()
{
	// 위에서 아래쪽으로 탄환발사 
	Set_MovePattern(NomalPos, 0.5f, TYPE_SinIn);
	
	float TargetX = 0;

	for (int i = 0; i < 3; i++)
	{
		if (i % 2 == 0)
			TargetX = GetRandomFloat(LEFT_POS + 3.f, 0);
		
		else
			TargetX = GetRandomFloat(0, RIGHT_POS - 3.f);

		Set_MovePattern(_float2(TargetX, TOP_POS), 0.2f, TYPE_SinInOut);
		Set_Attack_WorldPattern(
			DefaultSpawnOffset,
			DOWNVEC,
			1, 10, 0,
			BULLETTYPE_MOVE_NOMAL);
		Set_Delay(1);
	}
	return S_OK;
}


HRESULT CBossMonster::Set_BossPattern3()
{
	// 랜덤 Y 위치에서 발사


	Set_MovePattern(NomalPos, 0.3f, TYPE_SinInOut);

	for (int i = 0; i < 1; i++)
	{
		Set_MovePattern(BottomFront, 3, TYPE_SinIn);

		Set_Attack_PlayerTargetPattern(
			DefaultSpawnOffset,
			5, 6, 1.f,
			BULLETTYPE_MOVE_NOMAL);

		Set_Delay(6);
	}

	return S_OK;
}

HRESULT CBossMonster::Set_BossMoveDefault()
{
	Set_MovePattern(CenterPos, 2.0f, TYPE_SinInOut);
	return S_OK;
}


HRESULT CBossMonster::Choose_NextPattern()
{
	// Set_BossPattern3();


	 switch (mNextPattern)
	 {
	 case 0:
	 	Set_BossPattern1();
	 	mNextPattern++;
	 	break;
	 case 1:
	 	Set_BossPattern2();
	 	mNextPattern++;
	 	break;
	 case 2:
	 	Set_BossPattern3();
	 	mNextPattern = 0;
	 	break;
	 }
	
	return S_OK;
}

void CBossMonster::Set_MovePattern(_float2 endPos,_float time, EasingTypeID id)
{
	CBoss_Action_Move_Local::Action_MoveLocal_Desc descMove = {};

	descMove.mMonsterObject = this;

	descMove.mEndLocalPos = _float3(endPos.x, endPos.y,mCameralocalPosition.z);
	descMove.mTimerMax = time;
	descMove.mEasingType = id;
	mQueue_Partern.push(new CBoss_Action_Move_Local(descMove));
}

void CBossMonster::Set_Attack_WorldPattern(_float3 startPosOffset, _float3 endPos, _float count, _float speed, _float dealytime, E_BulletType_MOVE type2)
{
	Action_BulletCommon descAttack = {};

	descAttack.mMonsterObject = this;

	descAttack.mAttackCount = count;
	
	descAttack.mBulletSpawnOffset = startPosOffset;
	descAttack.mEndPos = endPos;
	descAttack.mBulletSpeed = speed;

	descAttack.mDealyTimeMax = dealytime;
	descAttack.meBulletType_Move = type2;


	mQueue_Partern.push(new CBoss_Pattern_Attack_WorldDir(descAttack));
}

void CBossMonster::Set_Attack_LocalDirPattern(_float3 startPosOffset, _float angle, _float count, _float speed, _float dealytime, E_BulletType_MOVE type2)
{
	Action_BulletCommon descAttack = {};

	descAttack.mMonsterObject = this;

	descAttack.mAttackCount = count;

	descAttack.mBulletSpawnOffset = startPosOffset;
	descAttack.mEndPos = _float3(0,0,0);
	descAttack.mBulletSpeed = speed;

	descAttack.mDealyTimeMax = dealytime;
	descAttack.meBulletType_Move = type2;

	CBoss_Pattern_Attack_LocalDir* patternDir = new CBoss_Pattern_Attack_LocalDir(descAttack);
	patternDir->mAngle = angle;

	mQueue_Partern.push(patternDir);

}

void CBossMonster::Set_Attack_PlayerTargetPattern(_float3 startPosOffset, _float count, _float speed, _float dealytime, E_BulletType_MOVE type2)
{
	Action_BulletCommon descAttack = {};

	descAttack.mMonsterObject = this;

	descAttack.mAttackCount = count;

	descAttack.mBulletSpawnOffset = startPosOffset;
	descAttack.mEndPos = _float3(0, 0, 0);
	descAttack.mBulletSpeed = speed;

	descAttack.mDealyTimeMax = dealytime;
	descAttack.meBulletType_Move = type2;

	CBoss_Pattern_Attack_PlayerTarget* patternDir = new CBoss_Pattern_Attack_PlayerTarget(descAttack);
	patternDir->SetTarget(mPlayerTarget->Get_TransformCom());
	mQueue_Partern.push(patternDir);
}

void CBossMonster::Set_Delay(_float Timer)
{
	CBoss_Pattern_Dealy* patternDir = new CBoss_Pattern_Dealy(Timer);
	mQueue_Partern.push(patternDir);
}

_float3 CBossMonster::Get_OffsetPos(_float3 offset)
{
	_float3 newPos = GetPos();
	newPos += offset;
	return newPos;
}

void CBossMonster::Start_AttackAniMaion(float frameSpeed)
{
	m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("Attack"), TEXT("Idle1"), frameSpeed);
}

_float3 CBossMonster::Update_CameraPosition(_float3 localPos)
{
	_float3 NewPos = localPos;

	D3DXVec3TransformCoord(&NewPos, &NewPos, &mMainCamera->Get_Camera_Transform()->Get_WorldMatrix());
	return (NewPos);
}

HRESULT CBossMonster::ViewPortHit(CGameObject * hitobj)
{
	if (!lstrcmp(hitobj->Get_Layer_Tag(), TAG_LAY(Layer_Bullet)))
	{
		if (m_BossStatusUI->Get_bCanHit())
		{
			float lifetime = (int)((CBullet*)hitobj)->GetLifeTime();
			if (lifetime > 1.0f)
			{
				if (mMainCamera->Get_bIsTuring())
					return S_OK;
				if (
					mMainCamera->Get_CameraLookState() == CCamera_Main::Look_Left_Axis ||
					mMainCamera->Get_CameraLookState() == CCamera_Main::Look_Right_Axis
					)
				{
					return S_OK;
				}

				((CBullet*)hitobj)->Die();
				Hit(-1);
			}

		}
	}
	return S_OK;
}

CBossMonster * CBossMonster::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CBossMonster* pInstance = new CBossMonster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CBossMonster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBossMonster * CBossMonster::Clone(void * pArg)
{
	CBossMonster* pInstance = new CBossMonster((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CBossMonster");
		Safe_Release(pInstance);

	}
	return pInstance;

}

void CBossMonster::Free()
{
	Safe_Release(mCurrentPattern);

	while (!mQueue_Partern.empty())
	{
		Safe_Release(mQueue_Partern.front());
		mQueue_Partern.pop();		
	}


	Safe_Release(mPlayerTarget);
	Safe_Release(mComGun);
	Safe_Release(mMainCamera);
	
	__super::Free();
	
}


// 이펙트 제작
PARTICLEDESC CBossMonster::CreateMonster_Hit_ParticleDESC()
{
	PARTICLEDESC tDesc;
	tDesc.eParticleID = Particle_Straight;

	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JH_Effect");
	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("hit");

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 0.4f;
	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 0.1f;
	tDesc.Particle_Power = 1.5f;

	tDesc.ParticleSize = _float3(8, 8, 8);
	tDesc.PowerRandomRange = _float2(1, 1);

	tDesc.MaxParticleCount = 10;

	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	// tDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player))->Get_Component(TAG_COM(Com_Transform)));

	tDesc.FollowingTarget = m_ComTransform;

	//파티클의 최대 이탈 범위(range)를 설정해 줌
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	const float range = 15;
	tDesc.MaxBoundary = _float3(range, range, range);

	// 파티클 시작 위치 랜덤
	tDesc.ParticleStartRandomPosMin = _float3(0, 0, -1);
	tDesc.ParticleStartRandomPosMax = _float3(0, 0, 1);

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;
	if (CCamera_Main::CameraLookStateID::Look_Front_Axis == Get_CameraLookState())
	{
		tDesc.ParticleStartRandomPosMin = _float3(0, 0, 1);
		tDesc.ParticleStartRandomPosMax = _float3(0, 0, 1);
		tDesc.vUp = RIGHTVEC;
	}
	else if (CCamera_Main::CameraLookStateID::Look_Back_Axis == Get_CameraLookState())
	{
		tDesc.ParticleStartRandomPosMin = _float3(0, 0, -1);
		tDesc.ParticleStartRandomPosMax = _float3(0, 0, -1);
		tDesc.vUp = LEFTVEC;
	}

	tDesc.m_bIsTextureAutoFrame = false;
	tDesc.fAutoFrameMul = 3.f;


	tDesc.ParticleColorChage = true;
	tDesc.TargetColor =  _float3(250, 128, 114);
	tDesc.TargetColor2 = _float3(251, 174, 210);

	tDesc.m_bIsUI = false;
	tDesc.MustDraw = false;
	tDesc.IsParticleFameEndtoDie = false;
	tDesc.AlphaBlendON = true;


	return tDesc;
}

PARTICLEDESC CBossMonster::CreateMonster_Default_ParticleDESC()
{
	PARTICLEDESC tDesc;
	tDesc.eParticleID = Particle_Fixed;

	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JH_Effect");
	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("CreatePylon");

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 0.05f;
	mDefaultEffect = 0.05f;


	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 0.05f;
	tDesc.Particle_Power = 1.5f;

	tDesc.ParticleSize = _float3(5, 5, 5);
	tDesc.PowerRandomRange = _float2(1, 1);

	tDesc.MaxParticleCount = 1;

	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	// tDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player))->Get_Component(TAG_COM(Com_Transform)));

	tDesc.FollowingTarget = m_ComTransform;

	//파티클의 최대 이탈 범위(range)를 설정해 줌
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	const float range = 15;
	tDesc.MaxBoundary = _float3(range, range, range);

	// 파티클 시작 위치 랜덤
	tDesc.ParticleStartRandomPosMin = _float3(0, 0, 0);
	tDesc.ParticleStartRandomPosMax = _float3(0, 0, 0);

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;
	const float rangerand = 0;

	_float3 backPos =  m_ComTransform->Get_MatrixState(CTransform::STATE_LOOK);

	
	if (CCamera_Main::CameraLookStateID::Look_Front_Axis == Get_CameraLookState())
	{
		tDesc.ParticleStartRandomPosMin = backPos;
		tDesc.ParticleStartRandomPosMax = backPos;
	}
	else if (CCamera_Main::CameraLookStateID::Look_Back_Axis == Get_CameraLookState())
	{
		tDesc.ParticleStartRandomPosMin = backPos.Get_Inverse();
		tDesc.ParticleStartRandomPosMax = backPos.Get_Inverse();
	}
	


	tDesc.m_bIsTextureAutoFrame = true;
	tDesc.fAutoFrameMul = 2.f;


	tDesc.ParticleColorChage = true;
	tDesc.TargetColor = _float3(191, 10, 48);
	tDesc.TargetColor2 = _float3(191, 10, 48);

	tDesc.m_bIsUI = false;
	tDesc.MustDraw = false;
	tDesc.IsParticleFameEndtoDie = false;
	tDesc.AlphaBlendON = true;

	return tDesc;
}

