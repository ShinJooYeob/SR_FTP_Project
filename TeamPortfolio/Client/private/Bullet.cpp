#include "stdafx.h"
#include "..\public\Bullet.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CMonsterParent(pGraphicDevice)
{

}

CBullet::CBullet(const CBullet& rhs)
	: CMonsterParent(rhs)
{

}


HRESULT CBullet::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;
	return S_OK;
}

HRESULT CBullet::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;
	
	m_ComTexture->Change_TextureLayer(TEXT("Bullet"));
	
	if (pArg != nullptr)
	{
		memcpy(&mDesc, pArg, sizeof(BULLETDESC));

		mBulletLifeTime = 0;
		SetBulletSpeed(mDesc.BulletSpeed);
		SetPos(mDesc.StartOffset);
	}
		
	Set_Scale(2.0f);


	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, CreateBulletParticleDESC());

	return S_OK;
}

_int CBullet::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	mBulletLifeTime += fDeltaTime;


	switch (mDesc.BulletType)
	{
	case BULLETTYPE_MOVE_NOMAL:
		m_ComTransform->MovetoDir(mDesc.MoveDir, fDeltaTime);
		break;

	default:
		break;
	}

	return _int();
}

_int CBullet::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;

	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	m_Sphere.mCenterPosition = m_Com_Viewport->WorldToView(GetPos());
	m_Com_Viewport->AddCollisionView(CCom_CollisionViewPort::COLL_BULLET,this);

	// ȭ����̸� ����
	if (m_Com_Viewport->isScreenOutPos(m_Sphere.mCenterPosition,m_Sphere.mRadius))
	{
		Die();
	}
	return _int();
}


_int CBullet::Render()
{

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

_int CBullet::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;
	
	return S_OK();
}



HRESULT CBullet::SetUp_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	return S_OK;
}

HRESULT CBullet::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;

}

PARTICLEDESC CBullet::CreateBulletParticleDESC()
{
	PARTICLEDESC tDesc;
	tDesc.eParticleID = Particle_Ball;

	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JH_Effect");
	//��ƼŬ �ؽ�ó ���̾� ������ƮŰ�� ������ �� ����
	tDesc.szTextureLayerTag = TEXT("beshot");

	//�� ��ƼŬ�� ���ʵ��� �𳯸� ������ ����
	tDesc.TotalParticleTime = 5.0f;
	//��ƼŬ �ϳ� �ϳ��� ���ʵ��� �𳯸� ������ ����
	tDesc.EachParticleLifeTime = 0.2f;
	tDesc.Particle_Power = 2.0f;

	float randSize = GetRandomFloat(1.f, 3.f);

	tDesc.ParticleSize = _float3(0.3f, 0.3f, 0.3f);
	tDesc.PowerRandomRange = _float2(0.8f, 1.2f);

	tDesc.MaxParticleCount = 10;

	//FixedTarget �� ����ϸ� ������ ��ġ���� ����ؼ� ������
	//FollowingTarget�� ����ϸ� �ش� ������Ʈ�� ����ٴϸ鼭 ��ƼŬ�� �𳯷���
	//�� ���� �ϳ��� ��� ����
	//�Ѵ� ����ϰ� ���� ��쿡�� ��ƼŬ�� 2�� ���� ����� ��
	//FollowingTarget�� ��� ����ٳ���� ������Ʈ�� CTransform ������Ʈ�� �־��ָ� ��
	// tDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player))->Get_Component(TAG_COM(Com_Transform)));

	tDesc.FollowingTarget = m_ComTransform;

	//��ƼŬ�� �ִ� ��Ż ����(range)�� ������ ��
	//FollowingTarget �̳� FixedTarget �� ��ǥ �������� �ش� ����(+, -)�� ����� ����
	const float range = 15;
	tDesc.MaxBoundary = _float3(range, range, range);

	// ��ƼŬ ���� ��ġ ����
	tDesc.ParticleStartRandomPosMin = _float3(-0.5f, -0.5f, 0);
	tDesc.ParticleStartRandomPosMax = _float3(0.5f, 0.5f, 0);

	//������ �����ϰ� ���� �� ����ϴ� �ɼ�
	//ex) straight�� ����ϴµ� ������Ʈ�� ���������� �Ѹ��� ������ ������Ʈ�� right�� �־��ָ� ��
	//Ȥ�� x���� ���� �������� �Ѹ��� ������ _float3(1,0,0); �̷������� �־��ָ� ��;
	tDesc.vUp = _float3(0, 1, 0);

	tDesc.m_bIsTextureAutoFrame = true;
	tDesc.fAutoFrameMul = 3.f;


	tDesc.ParticleColorChage = false;
	tDesc.m_bIsUI = false;
	tDesc.MustDraw = true;
	tDesc.IsParticleFameEndtoDie = false;
	tDesc.AlphaBlendON = true;

	// tDesc.m_fAlphaTestValue = 180.f;
	return tDesc;


}

void CBullet::SetBulletSpeed(_float speed)
{
	CTransform::TRANSFORMDESC desc;
	desc.fMovePerSec = speed;
	desc.fRotationPerSec = D3DXToRadian(90.0f);

	m_ComTransform->Set_TransformDesc(desc);
}

HRESULT CBullet::ViewPortHit(CGameObject * hitobj)
{	
	return S_OK;
}

HRESULT CBullet::CreateObject(_int Damage)
{
	return S_OK;
}

HRESULT CBullet::Hit(_int Damage)
{
	return S_OK;
}

HRESULT CBullet::Die()
{
	DIED();

	return S_OK;
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CBullet* pInstance = new CBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CBullet");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CBullet * CBullet::Clone(void * pArg)
{
	CBullet* pInstance = new CBullet((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CBullet");
		Safe_Release(pInstance);

	}
	return pInstance;
}

void CBullet::Free()
{
	__super::Free();

}