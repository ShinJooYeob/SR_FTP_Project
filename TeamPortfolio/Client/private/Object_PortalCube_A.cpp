#include "stdafx.h"
#include "Player.h"
#include "..\public\Object_PortalCube_A.h"
#include "Object_PortalCube_B.h"


CObject_PortalCube_A::CObject_PortalCube_A(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_PortalCube_A::CObject_PortalCube_A(const CObject_PortalCube_A & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_PortalCube_A::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_PortalCube_A::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (pArg == nullptr)
		return E_FAIL;

	/* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;



	POTALDESC tDesc;
	memcpy(&tDesc, pArg, sizeof(POTALDESC));

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, tDesc.vPos_A_Cube);


	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(tDesc.iNowScene, TEXT("Layer_Potal"), TEXT("Prototype_GameObject_Object_PortalCube_B"), &tDesc.vPos_B_Cube))
		return E_FAIL;


	CObject_PortalCube_B* pTargetCube = (CObject_PortalCube_B*)(GetSingle(CGameInstance)->Get_ObjectList_from_Layer(m_eNowSceneNum, TEXT("Layer_Potal"))->back());

	if (pTargetCube == nullptr || lstrcmp(TEXT("Layer_Potal"), pTargetCube->Get_Layer_Tag()))
		return E_FAIL;

	m_pTargetCube = pTargetCube;
	if(FAILED(pTargetCube->Set_TargetCube(this)))
		return E_FAIL;


	m_Layer_Tag = TEXT("Layer_Potal");

	m_ComTransform->Scaled({ 1.5f,2.f,2.f });
	FAILED_CHECK(m_ComTexture->Change_TextureLayer(TEXT("Potal_B"), 6.f));
	FAILED_CHECK(m_ComTexture->Change_TextureLayer(TEXT("Potal_A"),15.f));

	return S_OK;
}

_int CObject_PortalCube_A::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_fTimeDelta = fTimeDelta;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);

	if (m_Seconds > 0 && m_Seconds < 7.f)
	{
		m_Seconds += fTimeDelta;
		if (m_Seconds > 7.f)
			m_Seconds = 0;

		if (m_pPlayer != nullptr)
		{

			if (m_Seconds < 3.f)
			{
				CTransform* vPlayerTransform = (CTransform*)(m_pPlayer->Get_Component(TAG_COM(Com_Transform)));
				_float3 vObjPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
				vPlayerTransform->MovetoTarget(vObjPos, fTimeDelta);

			}
			else {
				Object_Transfer(m_pPlayer, fTimeDelta);
				m_pPlayer = nullptr;
			}

		}

	}
	return _int();
}

_int CObject_PortalCube_A::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;

	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS)))
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CObject_PortalCube_A::Render()
{
	if (FAILED(m_ComTransform->Bind_WorldMatrix_Look_Camera()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fTimeDelta)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_ComVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CObject_PortalCube_A::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_PortalCube_A::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	if (m_Seconds == 0 &&!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_Player")))
	{
		m_pPlayer = (CPlayer*)pDestObjects;
		m_pPlayer->Set_PlayerPause(3.0f, TEXT("suckIn"));

		m_Seconds += fDeltaTime;
		m_pTargetCube->UsedPotal(fDeltaTime);

		GetSingle(CGameInstance)->PlaySound(L"EH_PotalCube.wav", CHANNEL_OBJECT);
		Set_Particle();

		//Object_Transfer(pDestObjects, fDeltaTime);
	}

	return _int();
}

_int CObject_PortalCube_A::Object_Transfer(CGameObject * pDestObjects, _float fDeltaTime)
{

		if (m_pTargetCube == nullptr || pDestObjects == nullptr)
			return -1;

		CTransform* Player = (CTransform*)(pDestObjects->Get_Component(TAG_COM(Com_Transform)));
		CTransform* PortalCube_B = (CTransform*)(m_pTargetCube->Get_Component(TAG_COM(Com_Transform)));
		_float3 PortalCube_B_Pos = PortalCube_B->Get_MatrixState(CTransform::STATE_POS);
		Player->Set_MatrixState(CTransform::STATE_POS, PortalCube_B_Pos);


	return _int();
}


void CObject_PortalCube_A::Set_Particle()
{
	PARTICLEDESC tDesc;
	//��ƼŬ�� �𳯸��� ���� ����
	tDesc.eParticleID = Particle_Fixed;

	//�� ��ƼŬ�� ���ʵ��� �𳯸� ������ ����
	tDesc.TotalParticleTime = 0.f;

	//��ƼŬ �ϳ� �ϳ��� ���ʵ��� �𳯸� ������ ����
	tDesc.EachParticleLifeTime = 30.f;

	//��ƼŬ�� ����� ����
	tDesc.ParticleSize = _float3(5.f, 5.f, 5.f);
	//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ����
	tDesc.Particle_Power = 2;
	//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ���� ������ ����
	tDesc.PowerRandomRange = _float2(1.f, 1.1f);
	//��ƼŬ�� �ѹ��� �ִ� ����� ���� ������ ����
	tDesc.MaxParticleCount = 1;

	//��ƼŬ �ؽ�ó ������Ʈ �̸��� ���� (�⺻������ �ڱ� ���� ������Ʈ�� �ִ��� �˻��ϰ� ����ƽ���ִ����� �˻���)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	//��ƼŬ �ؽ�ó ���̾� ������ƮŰ�� ������ �� ����
	tDesc.szTextureLayerTag = TEXT("Particle_Circle_Red");
	//�ؽ�ó ������������ ����� ������ �� ������ ����
	tDesc.m_bIsTextureAutoFrame = true;
	tDesc.fAutoFrameMul = 1.5;


	//FixedTarget �� ����ϸ� ������ ��ġ���� ����ؼ� ������
	//FollowingTarget�� ����ϸ� �ش� ������Ʈ�� ����ٴϸ鼭 ��ƼŬ�� �𳯷���
	//�� ���� �ϳ��� ��� ����
	//�Ѵ� ����ϰ� ���� ��쿡�� ��ƼŬ�� 2�� ���� ����� ��
	//FollowingTarget�� ��� ����ٳ���� ������Ʈ�� CTransform ������Ʈ�� �־��ָ� ��
	tDesc.FollowingTarget = m_ComTransform;
	//tDesc.FixedTarget = _float3(0,3,0);


	//��ƼŬ�� �ִ� ��Ż ����(range)�� ������ �� 
	//FollowingTarget �̳� FixedTarget �� ��ǥ �������� �ش� ����(+, -)�� ����� ����
	tDesc.MaxBoundary = _float3(3, 3, 3);


	//�ؽ�ó�� ������ ������ �� �ִ� ��� �¿���
	//���� true�� ����� ��� �ؽ�ó�� ���� ������ ���õǰ� Ÿ�� �������� ��¦��¦ �Ÿ��� ������
	//true�� ����� ��� �ݵ�� Ÿ�� �÷��� ������ �� ��
	tDesc.ParticleColorChage = false;
	tDesc.TargetColor = _float3(43.f, 240.f, 201.f);
	tDesc.TargetColor2 = _float3(89.f, 67.f, 216.f);


	//���� UI�� �׷������Ѵٸ� true ���忡 �׷������Ѵٸ� false �� ������ ��
	//UI �� �׸��� �� ��� ���� ��� ��ǥ�� API ��ǥ �������� ������ ��
	//World�� �׸��� �� ��� ���� ��� ��ǥ�� ���� ��ǥ �������� ������ ��
	tDesc.m_bIsUI = false;
	//UI Depth ���� (UI �������°� ���� �����Ҷ� ���� ����)
	tDesc.m_bUIDepth = 1;

	//������ �����ϰ� ���� �� ����ϴ� �ɼ�
	//ex) straight�� ����ϴµ� ������Ʈ�� ���������� �Ѹ��� ������ ������Ʈ�� right�� �־��ָ� ��
	//Ȥ�� x���� ���� �������� �Ѹ��� ������ _float3(1,0,0); �̷������� �־��ָ� ��;

	//tDesc.vUp = _float3(0.f,0.f,0.f);

	tDesc.ParticleStartRandomPosMin = _float3(0.f, 0.f, 0.f);
	tDesc.ParticleStartRandomPosMax = _float3(0.f, 0.f, 0.f);

	tDesc.MustDraw = false;
	//������Ʈ �ڿ� �������� �ʰ� ����

	tDesc.IsParticleFameEndtoDie = true;
	//�������� �ѹ��� �������� ����


	//Create_ParticleObject�� ȣ���Ͽ� �������� ���̵�� ���ݱ��� ������ desc�� �־��ָ� ��
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

}

HRESULT CObject_PortalCube_A::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. �ؽ���*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_PotalTexture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	///////////////////////////////////////////////////////
	/* For.Com_Collision */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////
	return S_OK;
}

HRESULT CObject_PortalCube_A::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 30);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(180, 255, 255, 255));
	return S_OK;
}

HRESULT CObject_PortalCube_A::Release_RenderState()
{
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	return S_OK;
}

CObject_PortalCube_A * CObject_PortalCube_A::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_PortalCube_A* pInstance = new CObject_PortalCube_A(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_PortalCube_A");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_PortalCube_A::Clone(void * pArg)
{
	CObject_PortalCube_A* pInstance = new CObject_PortalCube_A((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_PortalCube_A");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_PortalCube_A::Free()
{
	__super::Free();


	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
