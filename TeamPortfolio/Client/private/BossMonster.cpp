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
	mHp = mMaxHp;
	m_Sphere.mRadius = 150.0f;

	return S_OK;
}

_int CBossMonster::Update(_float fDeltaTime)
{
	FAILED_CHECK(__super::Update(fDeltaTime));
	if (mPlayerTarget == nullptr)
	{
		auto playerlist = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENE_STATIC, TAG_LAY(Layer_Player));
		mPlayerTarget = (CPlayer*)playerlist->front();
		Safe_AddRef(mPlayerTarget);
		if (mPlayerTarget == nullptr)
			return 0;

		// 위치 초기화
		mCameralocalPosition = _float3(100, 0, 20);
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



	// AI 업데이트
	// 큐에서 패턴을 하나씩 실행한다.
	Update_BossPattern(fDeltaTime);
	return _int();
}

_int CBossMonster::LateUpdate(_float fDeltaTime)
{
	FAILED_CHECK(__super::LateUpdate(fDeltaTime));
	if (m_ComRenderer == nullptr)
		return -1;
	if (mHp <= 0)
		Die();

	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

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


HRESULT CBossMonster::SetUp_Components()
{
	FAILED_CHECK(__super::SetUp_Components());

	// 총 컴포넌트 초기화
	CCom_Gun::GUNDESC desc;
	desc.mSceneID = m_eNowSceneNum;
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Gun), TAG_COM(Com_Gun), (CComponent**)&mComGun,&desc)))
		return E_FAIL;	

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
	mHp += Damage;

	return S_OK;
}

HRESULT CBossMonster::Die()
{
	DIED();
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

HRESULT CBossMonster::Set_TestPattern1()
{
	float RightX = 10;
	float Topy = 5;
	float Bottomy = -5;


	//Set_AttackPattern(startPos, _float3(-1, 0, 0), 10, 6,0.5f,
	//	BULLETTYPE_CREATE_DIR,BULLETTYPE_MOVE_NOMAL);
	//
	//Set_AttackPattern(startPos, _float3(-1, 0, 0), 10, 6, 0.5f,
	//	BULLETTYPE_CREATE_DIR, BULLETTYPE_MOVE_NOMAL);

	Set_MovePattern(_float2(RightX, Topy), 1.f, TYPE_Linear);
	Set_MovePattern(_float2(RightX, Bottomy), 1.f, TYPE_Linear);
	
	//Set_MovePattern(_float2(RightX, Topy), 1.f, TYPE_Linear);
	//Set_MovePattern(_float2(RightX, Bottomy), 1.f, TYPE_Linear);

	//Set_MovePattern(_float2(RightX, Topy), 1.f, TYPE_Linear);
	//Set_MovePattern(_float2(RightX, Bottomy), 1.f, TYPE_Linear);

	return S_OK;
}

HRESULT CBossMonster::Set_TestPattern2()
{
	float RightX = 10;
	float Topy = 5.f;
	float Bottomy = -2.f;
	float TargetY = GetRandomFloat(Bottomy, Topy);

	_float3 SpawnOffset = _float3(0, -3, 0);

	for (int i = 0; i < 1; i++)
	{
		TargetY = GetRandomFloat(Bottomy, Topy);
		Set_MovePattern(_float2(RightX, TargetY), 1.f, TYPE_Linear);
		if (mMainCamera->Get_CameraLookState() == CCamera_Main::Look_Front_Axis)
		{
			Set_Attack_LocalDirPattern(SpawnOffset,
				180,
				2, 6, 1.0f,
				BULLETTYPE_MOVE_NOMAL);
		}

		else if (mMainCamera->Get_CameraLookState() == CCamera_Main::Look_Back_Axis)
		{
			Set_Attack_LocalDirPattern(SpawnOffset,
				0,
				2, 6, 1.0f,
				BULLETTYPE_MOVE_NOMAL);
		}

	}

	return S_OK;
}

HRESULT CBossMonster::Choose_NextPattern()
{
	if (mNextPattern % 2 ==  0)
	{
		Set_TestPattern1();
	}
	else
	{
		Set_TestPattern2();
	}
	
	mNextPattern++;
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

_float3 CBossMonster::Get_OffsetPos(_float3 offset)
{
	_float3 newPos = GetPos();
	newPos += offset;
	return newPos;
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
				m_BossStatusUI->Change_VersusPoint(-1);
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

