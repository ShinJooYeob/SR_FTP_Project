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

	/* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
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
		//�ӵ����� ����

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
	//��ƼŬ�� �𳯸��� ���� ����
	tDesc.eParticleID = Particle_Fixed;

	//�� ��ƼŬ�� ���ʵ��� �𳯸� ������ ����
	tDesc.TotalParticleTime = 0.f;

	//��ƼŬ �ϳ� �ϳ��� ���ʵ��� �𳯸� ������ ����
	tDesc.EachParticleLifeTime = 2.7f;

	//��ƼŬ�� ����� ����
	tDesc.ParticleSize = _float3(1.f, 1.f, 1.f);
	//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ����
	tDesc.Particle_Power = 2;
	//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ���� ������ ����
	tDesc.PowerRandomRange = _float2(1.f, 1.1f);
	//��ƼŬ�� �ѹ��� �ִ� ����� ���� ������ ����
	tDesc.MaxParticleCount = 1;

	//��ƼŬ �ؽ�ó ������Ʈ �̸��� ���� (�⺻������ �ڱ� ���� ������Ʈ�� �ִ��� �˻��ϰ� ����ƽ���ִ����� �˻���)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	//��ƼŬ �ؽ�ó ���̾� ������ƮŰ�� ������ �� ����
	tDesc.szTextureLayerTag = TEXT("Particle_Rect");
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
	tDesc.ParticleColorChage = true;
	tDesc.TargetColor = _float3(241, 181, 241);
	tDesc.TargetColor2 = _float3(200.f, 191.f, 231.f);


	//���� UI�� �׷������Ѵٸ� true ���忡 �׷������Ѵٸ� false �� ������ ��
	//UI �� �׸��� �� ��� ���� ��� ��ǥ�� API ��ǥ �������� ������ ��
	//World�� �׸��� �� ��� ���� ��� ��ǥ�� ���� ��ǥ �������� ������ ��
	tDesc.m_bIsUI = false;
	//UI Depth ���� (UI �������°� ���� �����Ҷ� ���� ����)
	tDesc.m_bUIDepth = 1;

	//������ �����ϰ� ���� �� ����ϴ� �ɼ�
	//ex) straight�� ����ϴµ� ������Ʈ�� ���������� �Ѹ��� ������ ������Ʈ�� right�� �־��ָ� ��
	//Ȥ�� x���� ���� �������� �Ѹ��� ������ _float3(1,0,0); �̷������� �־��ָ� ��;

	tDesc.vUp = m_EscalatorDesc.vStartPos - m_EscalatorDesc.vEndPos;

	tDesc.ParticleStartRandomPosMin = _float3(0.f, 0.f, 0.f);
	tDesc.ParticleStartRandomPosMax = _float3(0.f, 0.f, 0.f);

	tDesc.MustDraw = false;
	//������Ʈ �ڿ� �������� �ʰ� ����

	tDesc.IsParticleFameEndtoDie = true;
	//�������� �ѹ��� �������� ����


	//Create_ParticleObject�� ȣ���Ͽ� �������� ���̵�� ���ݱ��� ������ desc�� �־��ָ� ��
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);
}

void CObject_EscalatorCube::Return_Particle()
{
	PARTICLEDESC tDesc;
	//��ƼŬ�� �𳯸��� ���� ����
	tDesc.eParticleID = Particle_Fixed;

	//�� ��ƼŬ�� ���ʵ��� �𳯸� ������ ����
	tDesc.TotalParticleTime = 0.f;

	//��ƼŬ �ϳ� �ϳ��� ���ʵ��� �𳯸� ������ ����
	tDesc.EachParticleLifeTime = 2.7f;

	//��ƼŬ�� ����� ����
	tDesc.ParticleSize = _float3(1.f, 1.f, 1.f);
	//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ����
	tDesc.Particle_Power = 2;
	//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ���� ������ ����
	tDesc.PowerRandomRange = _float2(1.f, 1.1f);
	//��ƼŬ�� �ѹ��� �ִ� ����� ���� ������ ����
	tDesc.MaxParticleCount = 1;

	//��ƼŬ �ؽ�ó ������Ʈ �̸��� ���� (�⺻������ �ڱ� ���� ������Ʈ�� �ִ��� �˻��ϰ� ����ƽ���ִ����� �˻���)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	//��ƼŬ �ؽ�ó ���̾� ������ƮŰ�� ������ �� ����
	tDesc.szTextureLayerTag = TEXT("Particle_Rect");
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
	tDesc.ParticleColorChage = true;
	tDesc.TargetColor = _float3(176.f, 249.f, 174.f);
	tDesc.TargetColor2 = _float3(240.f, 206.f, 183.f);


	//���� UI�� �׷������Ѵٸ� true ���忡 �׷������Ѵٸ� false �� ������ ��
	//UI �� �׸��� �� ��� ���� ��� ��ǥ�� API ��ǥ �������� ������ ��
	//World�� �׸��� �� ��� ���� ��� ��ǥ�� ���� ��ǥ �������� ������ ��
	tDesc.m_bIsUI = false;
	//UI Depth ���� (UI �������°� ���� �����Ҷ� ���� ����)
	tDesc.m_bUIDepth = 1;

	//������ �����ϰ� ���� �� ����ϴ� �ɼ�
	//ex) straight�� ����ϴµ� ������Ʈ�� ���������� �Ѹ��� ������ ������Ʈ�� right�� �־��ָ� ��
	//Ȥ�� x���� ���� �������� �Ѹ��� ������ _float3(1,0,0); �̷������� �־��ָ� ��;

	tDesc.vUp = m_EscalatorDesc.vStartPos - m_EscalatorDesc.vEndPos;

	tDesc.ParticleStartRandomPosMin = _float3(0.f, 0.f, 0.f);
	tDesc.ParticleStartRandomPosMax = _float3(0.f, 0.f, 0.f);

	tDesc.MustDraw = false;
	//������Ʈ �ڿ� �������� �ʰ� ����

	tDesc.IsParticleFameEndtoDie = true;
	//�������� �ѹ��� �������� ����


	//Create_ParticleObject�� ȣ���Ͽ� �������� ���̵�� ���ݱ��� ������ desc�� �־��ָ� ��
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

	/* For. �ؽ���*/
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
