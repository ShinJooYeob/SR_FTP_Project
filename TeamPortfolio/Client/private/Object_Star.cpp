#include "stdafx.h"
#include "..\public\Object_Star.h"
#include "UI_Result.h"

CObject_Star::CObject_Star(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_Star::CObject_Star(const CObject_Star & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_Star::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_Star::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		memcpy(&m_StarDesc, pArg, sizeof(STARDESC));
		TempPos = m_StarDesc.fTransform;
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, m_StarDesc.fTransform);
	}

	m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));

	m_ComTexture->Change_TextureLayer(TEXT("Object_Star"));

	return S_OK;
}

_int CObject_Star::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FLEXIBLE, this);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CGameInstance);
	
	return _int();
}

_int CObject_Star::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;

	//if (FAILED(SetUp_OnTerrain(fTimeDelta)))
	//	return -1;

	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CObject_Star::Render()
{

	//if (FAILED(m_ComTransform->Bind_WorldMatrix()))
	//	return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix_Look_Camera()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_ComRectVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CObject_Star::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_Star::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	const _tchar* test = pDestObjects->Get_Layer_Tag();

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TAG_LAY(Layer_Player)))
	{
		CUI_Result* UI_Result =(CUI_Result*)pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum,TEXT("Layer_UI_Result"));
		GetSingle(CGameInstance)->PlaySound(L"EH_StarCube.wav", CHANNEL_OBJECT);
		UI_Result->Set_RankStar();
		Set_Particle();
		GetSingle(CQuest)->Set_QuestIndexIncrease(QUEST_2, 1);
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, NOT_EXIST_BLOCK);
		DIED();
		//m_pCollisionCom->Collision_Pushed(m_ComTransform, fCollision_Distance, fDeltaTime);
	}



	RELEASE_INSTANCE(CGameInstance);

	return _int();
}
HRESULT CObject_Star::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. �ؽ���*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_UI_Result_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer_Rect */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComRectVIBuffer)))
		return E_FAIL;

	///////////////////////////////////////////////////////
	/* For.Com_Collision */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////
	return S_OK;
}

HRESULT CObject_Star::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	/*
	_float4		vSourColor, vDestColor;

	(vSourColor.rgb) * vSourColor.a + (vDestColor.rgb) * (1.f - vSourColor.a);*/

	return S_OK;
}

HRESULT CObject_Star::Release_RenderState()
{

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	return S_OK;
}

void CObject_Star::Set_Particle()
{
	PARTICLEDESC tDesc;
	//��ƼŬ�� �𳯸��� ���� ����
	tDesc.eParticleID = Particle_Ball;

	//�� ��ƼŬ�� ���ʵ��� �𳯸� ������ ����
	tDesc.TotalParticleTime = 0.5f;

	//��ƼŬ �ϳ� �ϳ��� ���ʵ��� �𳯸� ������ ����
	tDesc.EachParticleLifeTime = 1.5f;

	//��ƼŬ�� ����� ����
	tDesc.ParticleSize = _float3(1.f, 1.f, 1.f);
	//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ����
	tDesc.Particle_Power = 50;
	//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ���� ������ ����
	tDesc.PowerRandomRange = _float2(0.4f, 1.0f);
	//��ƼŬ�� �ѹ��� �ִ� ����� ���� ������ ����
	tDesc.MaxParticleCount = 10;

	//��ƼŬ �ؽ�ó ������Ʈ �̸��� ���� (�⺻������ �ڱ� ���� ������Ʈ�� �ִ��� �˻��ϰ� ����ƽ���ִ����� �˻���)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_UI_Result_Texture");
	//��ƼŬ �ؽ�ó ���̾� ������ƮŰ�� ������ �� ����
	tDesc.szTextureLayerTag = TEXT("Object_Star");
	//�ؽ�ó ������������ ����� ������ �� ������ ����
	tDesc.m_bIsTextureAutoFrame = false;


	//FixedTarget �� ����ϸ� ������ ��ġ���� ����ؼ� ������
	//FollowingTarget�� ����ϸ� �ش� ������Ʈ�� ����ٴϸ鼭 ��ƼŬ�� �𳯷���
	//�� ���� �ϳ��� ��� ����
	//�Ѵ� ����ϰ� ���� ��쿡�� ��ƼŬ�� 2�� ���� ����� ��
	//FollowingTarget�� ��� ����ٳ���� ������Ʈ�� CTransform ������Ʈ�� �־��ָ� ��
	//tDesc.FollowingTarget = m_ComTransform;
	tDesc.FixedTarget = TempPos;


	//��ƼŬ�� �ִ� ��Ż ����(range)�� ������ �� 
	//FollowingTarget �̳� FixedTarget �� ��ǥ �������� �ش� ����(+, -)�� ����� ����
	tDesc.MaxBoundary = _float3(50, 50, 50);


	//�ؽ�ó�� ������ ������ �� �ִ� ��� �¿���
	//���� true�� ����� ��� �ؽ�ó�� ���� ������ ���õǰ� Ÿ�� �������� ��¦��¦ �Ÿ��� ������
	//true�� ����� ��� �ݵ�� Ÿ�� �÷��� ������ �� ��
	tDesc.ParticleColorChage = true;
	tDesc.TargetColor = _float3(255.f, 125.f, 19.f);
	tDesc.TargetColor2 = _float3(255.f, 0.f, 0.f);


	//���� UI�� �׷������Ѵٸ� true ���忡 �׷������Ѵٸ� false �� ������ ��
	//UI �� �׸��� �� ��� ���� ��� ��ǥ�� API ��ǥ �������� ������ ��
	//World�� �׸��� �� ��� ���� ��� ��ǥ�� ���� ��ǥ �������� ������ ��
	tDesc.m_bIsUI = false;
	//UI Depth ���� (UI �������°� ���� �����Ҷ� ���� ����)
	tDesc.m_bUIDepth = 0;

	//������ �����ϰ� ���� �� ����ϴ� �ɼ�
	//ex) straight�� ����ϴµ� ������Ʈ�� ���������� �Ѹ��� ������ ������Ʈ�� right�� �־��ָ� ��
	//Ȥ�� x���� ���� �������� �Ѹ��� ������ _float3(1,0,0); �̷������� �־��ָ� ��;

	//tDesc.vUp = _float3(1, 1, 0);

	tDesc.ParticleStartRandomPosMin = _float3(-0.5f, -0.5f, -0.5f);
	tDesc.ParticleStartRandomPosMax = _float3(0.5f, 0.5f, 0.5f);

	tDesc.MustDraw = true;
	//������Ʈ �ڿ� �������� �ʰ� ����

	//tDesc.IsParticleFameEndtoDie = false;
	//�������� �ѹ��� �������� ����

	//Create_ParticleObject�� ȣ���Ͽ� �������� ���̵�� ���ݱ��� ������ desc�� �־��ָ� ��
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	tDesc.TargetColor = _float3(37.f, 255.f, 19.f);
	tDesc.TargetColor2 = _float3(255.f, 0.f, 0.f);
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	tDesc.TargetColor = _float3(0.f, 26, 255.f);
	tDesc.TargetColor2 = _float3(242.f, 254.f, 20.f);
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	tDesc.TargetColor = _float3(197.f, 56, 154.f);
	tDesc.TargetColor2 = _float3(96.f, 57.f, 196.f);
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

}

CObject_Star * CObject_Star::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_Star* pInstance = new CObject_Star(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_Star");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_Star::Clone(void * pArg)
{
	CObject_Star* pInstance = new CObject_Star((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_Star");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_Star::Free()
{
	__super::Free();

	Safe_Release(m_ComRectVIBuffer);
	Safe_Release(m_ComCubeVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
