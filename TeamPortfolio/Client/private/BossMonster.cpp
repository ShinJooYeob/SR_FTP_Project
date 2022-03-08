#include "stdafx.h"
#include "..\public\BossMonster.h"
#include "..\public\BossParttern.h"
#include "Com_Gun.h"

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
	m_ComTexture->Change_TextureLayer(TEXT("Idle"));

	mPatternTime = 0;
	mNextPattern = 0;
	return S_OK;
}

_int CBossMonster::Update(_float fDeltaTime)
{
	FAILED_CHECK(__super::Update(fDeltaTime));
	if (mPlayerTarget == nullptr)
	{
		auto playerlist  = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENE_STATIC, TAG_LAY(Layer_Player));
		mPlayerTarget = playerlist->front();
		Safe_AddRef(mPlayerTarget);
		if (mPlayerTarget == nullptr)
			return 0;

		// 위치 초기화
		CTransform* trans = (CTransform*)mPlayerTarget->Get_Component(TAG_COM(Com_Transform));
		_float3 playerPos = trans->Get_MatrixState(CTransform::STATE_POS);
		playerPos.y += 1;

		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, playerPos);

	}
	mPatternTime += fDeltaTime;
	if (mPatternTime < 5)
		return 0;

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

	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

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
	return S_OK;
}

HRESULT CBossMonster::Die()
{
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

HRESULT CBossMonster::Set_TestMovePattern1()
{
	CBoss_Action_Move::Action_Move_Desc desc = {};

	desc.mMonsterObject = this;
	desc.mEndScreenPos = _float2(200, 720 * 0.2f);
	desc.mTimerMax = 0.5f;
	desc.mEasingType = TYPE_Linear;
	mQueue_Partern.push(new CBoss_Action_Move(desc));


	desc.mMonsterObject = this;
	desc.mEndScreenPos = _float2(1080, 720*0.2f);
	desc.mTimerMax = 0.5f;
	desc.mEasingType = TYPE_Linear;
	mQueue_Partern.push(new CBoss_Action_Move(desc));

	return S_OK;
}

HRESULT CBossMonster::Set_TestAttackPattern1()
{
	CBoss_Pattern_Attack::Action_Attack_Desc desc = {};
	desc.mAttackCount = 5;
	desc.mMonsterObject = this;
	desc.meBuelletType = E_BulletType::BULLETTYPE_Dir;
	desc.mTimerMax = 0.3f;


	mQueue_Partern.push(new CBoss_Pattern_Attack(desc));

	// CBoss_Pattern_Attack::Action_Attack_Desc desc = {};
	// desc.mAttackCount = 5;
	// desc.mCom_Gun = mComGun;
	// desc.meBuelletType = CCom_Gun::E_BulletType::BULLETTYPE_Dir;
	// 
	// mQueue_Partern.push(new CBoss_Pattern_Attack(desc));


	return S_OK;
}

HRESULT CBossMonster::Choose_NextPattern()
{
	if (mNextPattern %2 ==  0)
	{
		Set_TestMovePattern1();
	}
	else
	{
		Set_TestAttackPattern1();

	}

	mNextPattern++;
	return S_OK;
}

HRESULT CBossMonster::CreateObjectBullet_Target()
{
	CTransform* trans = (CTransform*)mPlayerTarget->Get_Component(TAG_COM(Com_Transform));
	_float3 playerPos = trans->Get_MatrixState(CTransform::STATE_POS);
	_float3 Position = GetPos();
	_float3 TargetDir = {0,0,0};

	TargetDir = playerPos - Position;
	TargetDir = TargetDir.Get_Nomalize();

	mComGun->CreateBullet_Target(m_eNowSceneNum, Position,TargetDir);

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

	__super::Free();
	
}
