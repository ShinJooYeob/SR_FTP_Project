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

	// 화면밖이면 죽음
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

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	return S_OK;
}

HRESULT CBullet::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;

}

PARTICLEDESC CBullet::CreateBulletParticleDESC()
{
	PARTICLEDESC tDesc;
	tDesc.eParticleID = Particle_Ball;

	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JH_Effect");
	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("beshot");

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 5.0f;
	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 0.2f;
	tDesc.Particle_Power = 2.0f;

	float randSize = GetRandomFloat(1.f, 3.f);

	tDesc.ParticleSize = _float3(0.3f, 0.3f, 0.3f);
	tDesc.PowerRandomRange = _float2(0.8f, 1.2f);

	tDesc.MaxParticleCount = 10;

	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	// tDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player))->Get_Component(TAG_COM(Com_Transform)));

	tDesc.FollowingTarget = m_ComTransform;

	//파티클의 최대 이탈 범위(range)를 설정해 줌
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	const float range = 15;
	tDesc.MaxBoundary = _float3(range, range, range);

	// 파티클 시작 위치 랜덤
	tDesc.ParticleStartRandomPosMin = _float3(-0.5f, -0.5f, -2);
	tDesc.ParticleStartRandomPosMax = _float3(0.5f, 0.5f, -1);

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;
	tDesc.vUp = _float3(0, 1, 0);

	tDesc.m_bIsTextureAutoFrame = true;
	tDesc.fAutoFrameMul = 3.f;


	tDesc.ParticleColorChage = false;
	tDesc.m_bIsUI = false;
	tDesc.MustDraw = false;
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