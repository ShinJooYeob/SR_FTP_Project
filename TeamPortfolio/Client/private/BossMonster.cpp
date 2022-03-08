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

//	mCurrentState = CBossMonster::BOSS_FSM_INIT;

	// 패턴 초기화
	if (!mQueue_Partern.empty())
	{
		mQueue_Partern.pop();
	}

	Set_RandomPattern();


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

	}

	// 더미 움직임
	CTransform* trans = (CTransform*)mPlayerTarget->Get_Component(TAG_COM(Com_Transform));
	_float3 playerPos = trans->Get_MatrixState(CTransform::STATE_POS);
	playerPos.y += 1;

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, playerPos);


	// AI 업데이트
	UpdateBossPattern();
	// 큐에서 하나씩 빼서 패턴을 실행한다.


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

	// 패턴에 따른 랜더링
	// if (m_StateMachine.GetState())
	// {
	// 
	// }

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
	return E_NOTIMPL;
}

HRESULT CBossMonster::Hit(_int Damage)
{
	return E_NOTIMPL;
}

HRESULT CBossMonster::Die()
{
	return E_NOTIMPL;
}

void CBossMonster::UpdateBossPattern()
{
	if (mQueue_Partern.empty())
		Set_RandomPattern();

	// 1. 패턴을 하나씩 꺼낸다.
	if (mCurrentPattern == nullptr)
	{
		mCurrentPattern = mQueue_Partern.back();
		mQueue_Partern.pop();
	}

	// 2. 패턴 실행

//	mCurrentPattern->

}

HRESULT CBossMonster::Set_RandomPattern()
{
	mQueue_Partern.emplace(new CBoss_Pattern_Move1());
	mQueue_Partern.emplace(new CBoss_Pattern_Move2());

//	mQueue_Partern.emplace(CBoss_Pattern_Attack1());
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
