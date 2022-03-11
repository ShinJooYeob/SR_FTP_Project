#include "stdafx.h"
#include "..\public\Object_GravityCube.h"


CObject_GravityCube::CObject_GravityCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_GravityCube::CObject_GravityCube(const CObject_GravityCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_GravityCube::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_GravityCube::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSettingPoint);
		m_Layer_Tag = TEXT("Layer_GravityCube");

		m_ComTransform->Scaled({ 3.f,3.f,3.f });
		m_ComTexture->Change_TextureLayer(L"none", 15.f);
		m_ComTexture->Change_TextureLayer(L"Gravity",15.f);
	}

	return S_OK;
}

_int CObject_GravityCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);
	m_fFrame = fTimeDelta;


	return _int();
}

_int CObject_GravityCube::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;



	//��ü���� �߷��� �����ϱ� ���� ��

	if (FAILED(Collision_Gravity(fTimeDelta)))
		return -1;

	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS),3.f))
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	return _int();
}

_int CObject_GravityCube::Render()
{
	if (FAILED(m_ComTransform->Bind_WorldMatrix_Look_Camera()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;


	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CObject_GravityCube::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_GravityCube::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}

_int CObject_GravityCube::Collision_Gravity(_float fDeltaTime)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	//��ü���� �߷��� �����ϱ� ���� ��
	CTransform* Player = (CTransform*)pGameInstance->Get_Commponent_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player), TAG_COM(Com_Transform));

	if (Player == nullptr)
		return E_FAIL;


	_float3& PlayerPos = Player->Get_MatrixState(CTransform::STATE_POS);

	_float3& GravityCubePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

	_float Distance = GravityCubePos.Get_Distance(PlayerPos);

	if (Distance < 2.5f) //�Ÿ� ���� ����
	{
		fDeltaTime *= 0.5f; // �ӵ� ���� ����
		Player->MovetoTarget(GravityCubePos, fDeltaTime);

		ParticleTime += fDeltaTime;
		if (ParticleTime > 0.2f)
		{
			Set_Particle_BlackDust();
			Set_Particle_DustSmall();
			ParticleTime = 0;
		}
	}


	RELEASE_INSTANCE(CGameInstance);

	return _int();
}


HRESULT CObject_GravityCube::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.vPivot = _float3(0.f,-1.f,0.f);
	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. �ؽ���*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_GravityTexture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
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

HRESULT CObject_GravityCube::SetUp_OnTerrain(_float fDeltaTime)
{
	return S_OK;
}

HRESULT CObject_GravityCube::SetUp_RenderState()
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

	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	//m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(180, 255, 255, 255));

	return S_OK;
}

HRESULT CObject_GravityCube::Release_RenderState()
{
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	return S_OK;
}

void CObject_GravityCube::Set_Particle_BlackDust()
{
	PARTICLEDESC tDesc;
	//��ƼŬ�� �𳯸��� ���� ����
	tDesc.eParticleID = Particle_Suck;

	//�� ��ƼŬ�� ���ʵ��� �𳯸� ������ ����
	tDesc.TotalParticleTime = 1.f;

	//��ƼŬ �ϳ� �ϳ��� ���ʵ��� �𳯸� ������ ����
	tDesc.EachParticleLifeTime = 1.0f;

	//��ƼŬ�� ����� ����
	tDesc.ParticleSize = _float3(0.1f, 0.1f, 0.1f);

	//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ����
	tDesc.Particle_Power = 4;

	//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ���� ������ ����
	tDesc.PowerRandomRange = _float2(0.5f, 1.5f);

	//��ƼŬ�� �ѹ��� �ִ� ����� ���� ������ ����
	tDesc.MaxParticleCount = 5;

	//��ƼŬ �ؽ�ó ������Ʈ �̸��� ���� (�⺻������ �ڱ� ���� ������Ʈ�� �ִ��� �˻��ϰ� ����ƽ���ִ����� �˻���)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");

	//��ƼŬ �ؽ�ó ���̾� ������ƮŰ�� ������ �� ����
	tDesc.szTextureLayerTag = TEXT("Particle_BlackDust");

	//�ؽ�ó ������������ ����� ������ �� ������ ����
	tDesc.m_bIsTextureAutoFrame = false;

	//FixedTarget �� ����ϸ� ������ ��ġ���� ����ؼ� ������
	//FollowingTarget�� ����ϸ� �ش� ������Ʈ�� ����ٴϸ鼭 ��ƼŬ�� �𳯷���
	//�� ���� �ϳ��� ��� ����
	//�Ѵ� ����ϰ� ���� ��쿡�� ��ƼŬ�� 2�� ���� ����� ��
	//FollowingTarget�� ��� ����ٳ���� ������Ʈ�� CTransform ������Ʈ�� �־��ָ� ��
	tDesc.FollowingTarget = m_ComTransform;
	//tDesc.FixedTarget = _float3(10, 10, 1);

	//��ƼŬ�� �ִ� ��Ż ����(range)�� ������ �� 
	//FollowingTarget �̳� FixedTarget �� ��ǥ �������� �ش� ����(+, -)�� ����� ����
	tDesc.MaxBoundary = _float3(10, 5, 10);

	//�ؽ�ó�� ������ ������ �� �ִ� ��� �¿���
	//���� true�� ����� ��� �ؽ�ó�� ���� ������ ���õǰ� Ÿ�� �������� ��¦��¦ �Ÿ��� ������
	//true�� ����� ��� �ݵ�� Ÿ�� �÷��� ������ �� ��
	tDesc.ParticleColorChage = false;
	tDesc.TargetColor = _float3(237, 186, 186);
	tDesc.TargetColor2 = _float3(200.f, 192.f, 231.f);

	//���� UI�� �׷������Ѵٸ� true ���忡 �׷������Ѵٸ� false �� ������ ��
	//UI �� �׸��� �� ��� ���� ��� ��ǥ�� API ��ǥ �������� ������ ��
	//World�� �׸��� �� ��� ���� ��� ��ǥ�� ���� ��ǥ �������� ������ ��
	tDesc.m_bIsUI = false;

	//������ �����ϰ� ���� �� ����ϴ� �ɼ�
	//ex) straight�� ����ϴµ� ������Ʈ�� ���������� �Ѹ��� ������ ������Ʈ�� right�� �־��ָ� ��
	//Ȥ�� x���� ���� �������� �Ѹ��� ������ _float3(1,0,0); �̷������� �־��ָ� ��;
	/*tDesc.vUp = _float3(-1, -1, 0);*/

	//��ƼŬ�� ���� ������ Min�� Max�� �����ش�.
	tDesc.ParticleStartRandomPosMin = _float3(-5, -5, -5);
	tDesc.ParticleStartRandomPosMax = _float3(5, 5, 5);

	tDesc.MustDraw = true;
	//������Ʈ �ڿ� �������� �ʰ� ����

	tDesc.IsParticleFameEndtoDie = false;
	//�������� �ѹ��� �������� ����

	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);



	//����
	//PARTICLEDESC tDesc;
	//tDesc.eParticleID = Particle_Suck;
	//tDesc.TotalParticleTime = 3600.f;
	//tDesc.EachParticleLifeTime = 1.0f;
	//tDesc.ParticleSize = _float3(0.3f, 0.3f, 0.3f);
	//tDesc.Particle_Power = 4;
	//tDesc.PowerRandomRange = _float2(0.5f, 1.5f);
	//tDesc.MaxParticleCount = 100;
	//tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	//tDesc.szTextureLayerTag = TEXT("greenleaf");
	//tDesc.m_bIsTextureAutoFrame = false;
	//tDesc.FollowingTarget = m_ComTransform;
	////tDesc.FixedTarget = _float3(10, 10, 1);
	//tDesc.MaxBoundary = _float3(10, 5, 10);
	//tDesc.ParticleColorChage = true;
	//tDesc.TargetColor = _float3(237, 186, 186);
	//tDesc.TargetColor2 = _float3(200.f, 192.f, 231.f);
	//tDesc.ParticleStartRandomPosMin = _float3(-10, -10, -10);
	//tDesc.ParticleStartRandomPosMax = _float3(10, 10, 10);


	//tDesc.m_bIsUI = false;
	///*tDesc.vUp = _float3(-1, -1, 0);*/
	//GetSingle(CParticleMgr)->Create_ParticleObject(SCENEID::SCENE_STAGESELECT, tDesc);
}

void CObject_GravityCube::Set_Particle_DustSmall()
{
	PARTICLEDESC tDesc;
	//��ƼŬ�� �𳯸��� ���� ����
	tDesc.eParticleID = Particle_Suck;

	//�� ��ƼŬ�� ���ʵ��� �𳯸� ������ ����
	tDesc.TotalParticleTime = 0.f;

	//��ƼŬ �ϳ� �ϳ��� ���ʵ��� �𳯸� ������ ����
	tDesc.EachParticleLifeTime = 20.f;

	//��ƼŬ�� ����� ����
	tDesc.ParticleSize = _float3(0.3f, 0.3f, 0.3f);

	//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ����
	tDesc.Particle_Power = 4;

	//��ƼŬ�� �Ŀ�(�̵��ӵ�)�� ���� ������ ����
	tDesc.PowerRandomRange = _float2(0.5f, 1.5f);

	//��ƼŬ�� �ѹ��� �ִ� ����� ���� ������ ����
	tDesc.MaxParticleCount = 10;

	//��ƼŬ �ؽ�ó ������Ʈ �̸��� ���� (�⺻������ �ڱ� ���� ������Ʈ�� �ִ��� �˻��ϰ� ����ƽ���ִ����� �˻���)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");

	//��ƼŬ �ؽ�ó ���̾� ������ƮŰ�� ������ �� ����
	tDesc.szTextureLayerTag = TEXT("Particle_DustSmall");

	//�ؽ�ó ������������ ����� ������ �� ������ ����
	tDesc.m_bIsTextureAutoFrame = true;

	//FixedTarget �� ����ϸ� ������ ��ġ���� ����ؼ� ������
	//FollowingTarget�� ����ϸ� �ش� ������Ʈ�� ����ٴϸ鼭 ��ƼŬ�� �𳯷���
	//�� ���� �ϳ��� ��� ����
	//�Ѵ� ����ϰ� ���� ��쿡�� ��ƼŬ�� 2�� ���� ����� ��
	//FollowingTarget�� ��� ����ٳ���� ������Ʈ�� CTransform ������Ʈ�� �־��ָ� ��
	tDesc.FollowingTarget = m_ComTransform;
	//tDesc.FixedTarget = _float3(10, 10, 1);

	//��ƼŬ�� �ִ� ��Ż ����(range)�� ������ �� 
	//FollowingTarget �̳� FixedTarget �� ��ǥ �������� �ش� ����(+, -)�� ����� ����
	tDesc.MaxBoundary = _float3(10, 5, 10);

	//�ؽ�ó�� ������ ������ �� �ִ� ��� �¿���
	//���� true�� ����� ��� �ؽ�ó�� ���� ������ ���õǰ� Ÿ�� �������� ��¦��¦ �Ÿ��� ������
	//true�� ����� ��� �ݵ�� Ÿ�� �÷��� ������ �� ��
	tDesc.ParticleColorChage = false;
	tDesc.TargetColor = _float3(237, 186, 186);
	tDesc.TargetColor2 = _float3(200.f, 192.f, 231.f);

	//���� UI�� �׷������Ѵٸ� true ���忡 �׷������Ѵٸ� false �� ������ ��
	//UI �� �׸��� �� ��� ���� ��� ��ǥ�� API ��ǥ �������� ������ ��
	//World�� �׸��� �� ��� ���� ��� ��ǥ�� ���� ��ǥ �������� ������ ��
	tDesc.m_bIsUI = false;

	//������ �����ϰ� ���� �� ����ϴ� �ɼ�
	//ex) straight�� ����ϴµ� ������Ʈ�� ���������� �Ѹ��� ������ ������Ʈ�� right�� �־��ָ� ��
	//Ȥ�� x���� ���� �������� �Ѹ��� ������ _float3(1,0,0); �̷������� �־��ָ� ��;
	/*tDesc.vUp = _float3(-1, -1, 0);*/

	//��ƼŬ�� ���� ������ Min�� Max�� �����ش�.
	tDesc.ParticleStartRandomPosMin = _float3(-5, -5, -5);
	tDesc.ParticleStartRandomPosMax = _float3(5, 5, 5);

	tDesc.MustDraw = true;
	//������Ʈ �ڿ� �������� �ʰ� ����

	tDesc.IsParticleFameEndtoDie = true;
	//�������� �ѹ��� �������� ����

	GetSingle(CParticleMgr)->Create_ParticleObject(SCENEID::SCENE_STAGESELECT, tDesc);
}

CObject_GravityCube * CObject_GravityCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_GravityCube* pInstance = new CObject_GravityCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_GravityCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_GravityCube::Clone(void * pArg)
{
	CObject_GravityCube* pInstance = new CObject_GravityCube((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_GravityCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_GravityCube::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
