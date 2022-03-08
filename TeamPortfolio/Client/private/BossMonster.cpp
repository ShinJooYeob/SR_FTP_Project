#include "stdafx.h"
#include "..\public\BossMonster.h"
#include "..\public\BossParttern.h"

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

	return S_OK;
}

HRESULT CBossMonster::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	mPlayerTarget = nullptr;
	m_ComTransform->Scaled(_float3(5, 5, 5));
	m_ComTexture->Change_TextureLayer(TEXT("Idle"));

	// 패턴 초기화
	if (!mQueue_Partern.empty())
	{
		mQueue_Partern.pop();
	}
	mCurrentPattern = nullptr;

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
	// mComGun;

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
		Set_TestPattern();

	// 패턴이 끝나면 다음 패턴을 업데이트 시켜준다.

	if (mCurrentPattern == nullptr)
	{
		mCurrentPattern = mQueue_Partern.front();
		mQueue_Partern.pop();
		return;
	}

	if (mCurrentPattern->IsPatternEnd())
	{
		mCurrentPattern = mQueue_Partern.front();
		mQueue_Partern.pop();
	}
	// 2. 패턴 실행
	mCurrentPattern->Action(deltatime);


}

HRESULT CBossMonster::Set_TestPattern()
{
	CBoss_Action_Move::Action_Move_Desc desc = {};

	desc.mMonsterObject = this;
	desc.mEndPos = GetScreenToWorld(_float2(200, 720 * 0.5f), 0);
	desc.mTimerMax = 1.0f;
	desc.mEasingType = TYPE_Linear;
	mQueue_Partern.emplace(new CBoss_Action_Move(desc));


	CBoss_Action_Move::Action_Move_Desc desc2 = {};

	desc2.mMonsterObject = this;
	desc2.mEndPos = GetScreenToWorld(_float2(1080, 720*0.5f), 0);
	desc2.mTimerMax = 1.0f;
	desc2.mEasingType = TYPE_Linear;
	mQueue_Partern.emplace(new CBoss_Action_Move(desc2));

	
//	mQueue_Partern.emplace(new CBoss_Action_Move());





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
	Safe_Release(mPlayerTarget);
	__super::Free();
	
}
