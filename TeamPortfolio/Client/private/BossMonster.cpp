#include "stdafx.h"
#include "..\public\BossMonster.h"
#include "..\public\BossParttern.h"
#include "Com_Gun.h"
#include "Player.h"
#include "Bullet.h"

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


	m_ComTransform->Scaled(_float3(5, 5, 5));
	m_ComTexture->Change_TextureLayer(TEXT("Idle1"));

	m_pGraphicDevice->GetViewport(&mViewPort);

	auto CameraList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENE_STATIC, TAG_LAY(Layer_Camera_Main));
	mMainCamera = (CCamera_Main*)CameraList->front();
	Safe_AddRef(mMainCamera);

	mPatternTime = 0;
	mNextPattern = 0;

	mMaxHp = 10;
	mHp = mMaxHp;
	return S_OK;
}

_int CBossMonster::Update(_float fDeltaTime)
{
	FAILED_CHECK(__super::Update(fDeltaTime));
	if (mPlayerTarget == nullptr)
	{
		auto playerlist  = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENE_STATIC, TAG_LAY(Layer_Player));
		mPlayerTarget = (CPlayer*)playerlist->front();
		Safe_AddRef(mPlayerTarget);
		if (mPlayerTarget == nullptr)
			return 0;

		// 위치 초기화
		CTransform* trans = mPlayerTarget->Get_TransformCom();
		_float3 playerPos = trans->Get_MatrixState(CTransform::STATE_POS);
		playerPos.y += 1;

		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, playerPos);

	}
	mPatternTime += fDeltaTime;
	if (mPatternTime < 1)
		return 0;

	

	_float3 newPos = GetPos();

	newPos = Update_CameraPosition(newPos);
	SetPos(newPos);


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


HRESULT CBossMonster::SetUp_Components()
{
	FAILED_CHECK(__super::SetUp_Components());

	// 총 컴포넌트 초기화
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Gun), TAG_COM(Com_Gun), (CComponent**)&mComGun)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CBossMonster::SetUp_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return S_OK;
}

HRESULT CBossMonster::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

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
	float wincx = mViewPort.Width;
	float wincy = mViewPort.Height;

	float LeftPos = wincx * 0.1f;

	//	Set_MovePattern(_float2(LeftPos, wincy*0.1f), 0.3f, TYPE_QuadIn);
	//	Set_AttackPattern(1, BULLETTYPE_Dir, 1.0f, _float3(1, 0, 0));


	Set_MovePattern(_float2(LeftPos, wincy*0.2f), 0.1f, TYPE_QuadIn);

	Set_AttackPattern(5, BULLETTYPE_CamDir, 0.3f, 6.0f, _float3());

	Set_MovePattern(_float2(LeftPos, wincy*0.5f), 0.3f, TYPE_QuadIn);
	Set_MovePattern(_float2(LeftPos, wincy*0.1f), 0.3f, TYPE_QuadIn);
	Set_MovePattern(_float2(LeftPos, wincy*0.6f), 0.3f, TYPE_QuadIn);

	


	return S_OK;
}

HRESULT CBossMonster::Set_TestPattern2()
{
	float wincx = mViewPort.Width;
	float wincy = mViewPort.Height;

	float LeftPos = wincx * 0.1f;

	Set_MovePattern(_float2(LeftPos, wincy*0.5f), 0.5f, TYPE_Linear);

	float randFloat = GetRandomFloat(0.1f, 0.8f);

	for (int i = 0; i < 10; i++)
	{
		randFloat = GetRandomFloat(0.1f, 0.8f);
		Set_MovePattern(_float2(LeftPos, wincy*randFloat), 0.2f, TYPE_Linear);
		Set_AttackPattern(2, BULLETTYPE_CamDir, 0.5f,4.0f, _float3());

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

void CBossMonster::Set_MovePattern(_float2 screenPos,_float time, EasingTypeID id)
{
	CBoss_Action_Move::Action_Move_Desc descMove = {};

	descMove.mMonsterObject = this;

	descMove.mEndScreenPos = screenPos;
	descMove.mTimerMax = time;
	descMove.mEasingType = id;
	mQueue_Partern.push(new CBoss_Action_Move(descMove));
}

void CBossMonster::Set_AttackPattern(_float attackcount, E_BulletType bulletType, _float time,_float speed, _float3 moveidr)
{
	CBoss_Pattern_Attack::Action_Attack_Desc descAttack = {};

	descAttack.mMonsterObject = this;
	descAttack.mCameraMain =  mMainCamera;

	descAttack.mAttackCount = attackcount;
	descAttack.meBuelletType = bulletType;
	descAttack.mTimerMax = time;
	descAttack.mBulletSpeed = speed;
	descAttack.mDir = moveidr.Get_Nomalize();
	mQueue_Partern.push(new CBoss_Pattern_Attack(descAttack));
}

HRESULT CBossMonster::CreateObjectBullet_Target()
{
	CTransform* trans = mPlayerTarget->Get_TransformCom();
	_float3 playerPos = trans->Get_MatrixState(CTransform::STATE_POS);
	_float3 GunPos = Get_OffsetPos(_float3(0,-1,0));

	_float3 TargetDir = {0,0,0};

	TargetDir = playerPos - GunPos;
	TargetDir = TargetDir.Get_Nomalize();
	
	mComGun->CreateBullet_Target(m_eNowSceneNum, GunPos,TargetDir);

	return S_OK;
}

HRESULT CBossMonster::CreateObjectBullet_Target_Dir(_float3 dir,_float speed)
{
	_float3 GunPos = Get_OffsetPos(_float3(0, -1, 0));

	mComGun->CreateBullet_Target(m_eNowSceneNum, GunPos, dir, speed);

	return S_OK;
}

_float3 CBossMonster::Get_OffsetPos(_float3 offset)
{
	_float3 newPos = GetPos();
	newPos += offset;
	return newPos;
}

HRESULT CBossMonster::ViewPortHit(CGameObject * hitobj)
{
	if (!lstrcmp(hitobj->Get_Layer_Tag(), TAG_LAY(Layer_Bullet)))
	{
		float lifetime = (int)((CBullet*)hitobj)->GetLifeTime();
		if (lifetime > 2.0f)
		{
			if (mMainCamera->Get_bIsTuring())
				return S_OK;
			if (
				mMainCamera->Get_CameraLookState() == CCamera_Main::Look_Left_Axis ||
				mMainCamera->Get_CameraLookState() == CCamera_Main::Look_Right_Axis
				)
				return S_OK;

			((CBullet*)hitobj)->Die();
			Hit(-1);
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

