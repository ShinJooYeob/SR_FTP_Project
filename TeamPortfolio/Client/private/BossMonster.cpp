#include "stdafx.h"
#include "..\public\BossMonster.h"

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
	mCurrentState = CBossMonster::BOSS_FSM_INIT;

	// �ʱ���� ����
	//	m_StateMachine.SetState();

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

	// ���� ������
	CTransform* trans = (CTransform*)mPlayerTarget->Get_Component(TAG_COM(Com_Transform));
	_float3 playerPos = trans->Get_MatrixState(CTransform::STATE_POS);
	playerPos.y += 1;

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, playerPos);

	// ���� ������Ʈ
//	FSMUpdate();

//	m_StateMachine.Update();


	// ���Ͽ� ���� ������ ����
	// if (m_StateMachine.GetState())
	// {
	// 
	// }

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

	// ���Ͽ� ���� ������
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

	return S_OK;
}

HRESULT CBossMonster::SetUp_RenderState()
{
	return S_OK;
}

HRESULT CBossMonster::Release_RenderState()
{
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
