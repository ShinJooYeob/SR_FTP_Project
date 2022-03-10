#include "stdafx.h"
#include "..\public\ParticleObejct.h"


CParticleObject::CParticleObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CParticleObject::CParticleObject(const CParticleObject & rhs)
	: CGameObject(rhs)
{	
    //m_RateTime = rhs.m_RateTime;	
    //m_Scale = rhs.m_Scale;
	m_list_Particles = rhs.m_list_Particles;	
    //m_MaxParticles = rhs.m_MaxParticles;	
	m_boundingBox = rhs.m_boundingBox;
}

HRESULT CParticleObject::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	m_list_Particles.clear();
	//m_RateTime = 1.0f;
	//m_Scale = 1.0f;
	//m_MaxParticles = 10;


	return S_OK;
}

HRESULT CParticleObject::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	FAILED_CHECK(SetUp_ParticleDesc(pArg));

	// 부모 객체의 컴포넌트 생성
	// 자식 객체들은 부모객체의 컴포넌트를 사용하게 한다.
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (m_ParticleDesc.IsParticleFameEndtoDie)
		m_ComTexture->Change_TextureLayer_Wait(m_ParticleDesc.szTextureLayerTag);
	else
		m_ComTexture->Change_TextureLayer(m_ParticleDesc.szTextureLayerTag);



	if (m_ParticleDesc.FollowingTarget != nullptr)
		m_ComParentTransform->Set_MatrixState(CTransform::STATE_POS, m_ParticleDesc.FollowingTarget->Get_MatrixState(CTransform::STATE_POS));
	else
		m_ComParentTransform->Set_MatrixState(CTransform::STATE_POS, m_ParticleDesc.FixedTarget);

	m_ComParticleTransform->Scaled(m_ParticleDesc.ParticleSize);

	// 클론 시 자식 객체 생성
	Initialize_Child_Clone();


	return S_OK;
}


_int CParticleObject::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_fEffectLifeTime += fTimeDelta;



	_bool test = !lstrcmp(m_ParticleDesc.szTextureLayerTag, L"Meteo_dust_A");
	if (m_ParticleDesc.FollowingTarget != nullptr)
		m_ComParentTransform->Set_MatrixState(CTransform::STATE_POS, m_ParticleDesc.FollowingTarget->Get_MatrixState(CTransform::STATE_POS));
	else
		m_ComParentTransform->Set_MatrixState(CTransform::STATE_POS, m_ParticleDesc.FixedTarget);



	// 자식 객체 업데이트
	std::list<PARTICLEATT>::iterator iter;

	if (m_fEffectLifeTime > m_ParticleDesc.TotalParticleTime) {

		for (iter = m_list_Particles.begin(); iter != m_list_Particles.end(); )
		{
			m_boundingBox.ResetBoudingBox(iter->_NowparantPos);
			iter->_age += fTimeDelta;
			if (m_ParticleDesc.ParticleColorChage)
			{
				Update_ColorChange(&(*iter), fTimeDelta);
			}

			Update_Position_by_Velocity(&(*iter), fTimeDelta);

			if (m_ParticleDesc.bSubPraticle)
			{
				iter->SubParticle->Reset_FixedPostion((iter->_position));
			}

			// 범위 체크
			if (m_boundingBox.isPointInside(iter->_position) == false || iter->_age > iter->_lifeTime)
			{
				iter = m_list_Particles.erase(iter);

			}
			else 
			{
				iter++;
			}
		}

		if (m_list_Particles.size() == 0)
			DIED();

	}
	else 
	{
		for (iter = m_list_Particles.begin(); iter != m_list_Particles.end(); iter++)
		{
			m_boundingBox.ResetBoudingBox(iter->_NowparantPos);
			iter->_age += fTimeDelta;

			Update_Position_by_Velocity(&(*iter), fTimeDelta);
			
			if (m_ParticleDesc.ParticleColorChage)
			{
				Update_ColorChange(&(*iter), fTimeDelta);
			}

			if (m_ParticleDesc.bSubPraticle)
			{
				iter->SubParticle->Reset_FixedPostion((iter->_position));
			}
			// 범위 체크
			if (m_boundingBox.isPointInside(iter->_position) == false || iter->_age > iter->_lifeTime)
			{
				// 리셋
				ResetParticle(&(*iter));
			}
		}

	}
	const _tchar* temp = m_ComTexture->Get_NowTextureTag();
	if (m_ParticleDesc.IsParticleFameEndtoDie && m_ComTexture->Get_NowTextureTag() == nullptr)
		DIED();

	return 0;
}

_int CParticleObject::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


	m_ComTexture->Set_LoadTexutreNumber(0);
	//if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS)))
	//	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	if (m_ParticleDesc.m_bIsUI)
		m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this);

	else
		m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_AFTEROBJ, this);


	return _int();
}

void CParticleObject::ResetParticle(PARTICLEATT * attribute)
{
	_float3 RandomPos = _float3(0, 0, 0);


	GetRandomVector(&RandomPos, &m_ParticleDesc.ParticleStartRandomPosMin, &m_ParticleDesc.ParticleStartRandomPosMax);
	if (m_ParticleDesc.m_bIsUI)
		RandomPos.z = 0;
	attribute->_position = Get_ParentPos() + RandomPos;

	attribute->_lifeTime = m_ParticleDesc.EachParticleLifeTime * (((_float)(rand() % 1500 + 500))*0.001f);
	attribute->_age = 0;
	attribute->_color = m_ParticleDesc.TargetColor2;

	attribute->_force = m_ParticleDesc.Particle_Power * 
		((((_float)(rand() % _uint((m_ParticleDesc.PowerRandomRange.y - m_ParticleDesc.PowerRandomRange.x)*1000.f)) + m_ParticleDesc.PowerRandomRange.x * 1000.f))*0.001f);
	attribute->_NowparantPos = m_ComParentTransform->Get_MatrixState(CTransform::STATE_POS);

	Reset_Velocity(attribute->_velocity);
}

HRESULT CParticleObject::Set_SubParticleDesc(PARTICLEDESC * pSubParticleDesc)
{
	if (m_ParticleDesc.bSubPraticle == false || pSubParticleDesc == nullptr)
		return E_FAIL;

	memcpy(&m_SubParticleDesc, pSubParticleDesc, sizeof(PARTICLEDESC));


	m_SubParticleDesc.bSubPraticle = false;
	m_SubParticleDesc.FollowingTarget = nullptr;

	if (m_SubParticleDesc.TotalParticleTime > m_ParticleDesc.TotalParticleTime)
		m_SubParticleDesc.TotalParticleTime = m_ParticleDesc.TotalParticleTime;


	list<CGameObject*>* ParticleList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(m_eNowSceneNum, L"Layer_Particle");

	auto iter = m_list_Particles.begin();

	for (; iter != m_list_Particles.end(); iter++)
	{
		GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, m_SubParticleDesc);
		iter->SubParticle = (CParticleObject*)(ParticleList->back());
		((CParticleObject*)(ParticleList->back()))->Reset_FixedPostion(iter->_position);
	}


	


	return S_OK;
}

void CParticleObject::Update_ColorChange(PARTICLEATT * tParticleAtt, _float fTimeDelta)
{
	CGameInstance* pInstance = GetSingle(CGameInstance);
	
	if (tParticleAtt->_age < tParticleAtt->_lifeTime * 0.25f)
	{
		tParticleAtt->_color.x = pInstance->Easing(TYPE_Linear, m_ParticleDesc.TargetColor2.x, m_ParticleDesc.TargetColor.x, tParticleAtt->_age, tParticleAtt->_lifeTime * 0.25f);
		tParticleAtt->_color.y = pInstance->Easing(TYPE_Linear, m_ParticleDesc.TargetColor2.y, m_ParticleDesc.TargetColor.y, tParticleAtt->_age, tParticleAtt->_lifeTime * 0.25f);
		tParticleAtt->_color.z = pInstance->Easing(TYPE_Linear, m_ParticleDesc.TargetColor2.z, m_ParticleDesc.TargetColor.z, tParticleAtt->_age, tParticleAtt->_lifeTime * 0.25f);

	}
	else if (tParticleAtt->_age < tParticleAtt->_lifeTime * 0.5f)
	{
		tParticleAtt->_color.x = pInstance->Easing(TYPE_Linear, m_ParticleDesc.TargetColor.x, m_ParticleDesc.TargetColor2.x, tParticleAtt->_age - tParticleAtt->_lifeTime * 0.25f, tParticleAtt->_lifeTime * 0.25f);
		tParticleAtt->_color.y = pInstance->Easing(TYPE_Linear, m_ParticleDesc.TargetColor.y, m_ParticleDesc.TargetColor2.y, tParticleAtt->_age - tParticleAtt->_lifeTime * 0.25f, tParticleAtt->_lifeTime * 0.25f);
		tParticleAtt->_color.z = pInstance->Easing(TYPE_Linear, m_ParticleDesc.TargetColor.z, m_ParticleDesc.TargetColor2.z, tParticleAtt->_age - tParticleAtt->_lifeTime * 0.25f, tParticleAtt->_lifeTime * 0.25f);
	}
	else if (tParticleAtt->_age < tParticleAtt->_lifeTime * 0.75f)
	{
		tParticleAtt->_color.x = pInstance->Easing(TYPE_Linear, m_ParticleDesc.TargetColor2.x, m_ParticleDesc.TargetColor.x, tParticleAtt->_age - tParticleAtt->_lifeTime * 0.5f, tParticleAtt->_lifeTime * 0.25f);
		tParticleAtt->_color.y = pInstance->Easing(TYPE_Linear, m_ParticleDesc.TargetColor2.y, m_ParticleDesc.TargetColor.y, tParticleAtt->_age - tParticleAtt->_lifeTime * 0.5f, tParticleAtt->_lifeTime * 0.25f);
		tParticleAtt->_color.z = pInstance->Easing(TYPE_Linear, m_ParticleDesc.TargetColor2.z, m_ParticleDesc.TargetColor.z, tParticleAtt->_age - tParticleAtt->_lifeTime * 0.5f, tParticleAtt->_lifeTime * 0.25f);

	}
	else if (tParticleAtt->_age < tParticleAtt->_lifeTime)
	{

		tParticleAtt->_color.x = pInstance->Easing(TYPE_Linear, m_ParticleDesc.TargetColor.x, m_ParticleDesc.TargetColor2.x, tParticleAtt->_age - tParticleAtt->_lifeTime * 0.75f, tParticleAtt->_lifeTime * 0.25f);
		tParticleAtt->_color.y = pInstance->Easing(TYPE_Linear, m_ParticleDesc.TargetColor.y, m_ParticleDesc.TargetColor2.y, tParticleAtt->_age - tParticleAtt->_lifeTime * 0.75f, tParticleAtt->_lifeTime * 0.25f);
		tParticleAtt->_color.z = pInstance->Easing(TYPE_Linear, m_ParticleDesc.TargetColor.z, m_ParticleDesc.TargetColor2.z, tParticleAtt->_age - tParticleAtt->_lifeTime * 0.75f, tParticleAtt->_lifeTime * 0.25f);
	}

	if (tParticleAtt->_color.x < 0) tParticleAtt->_color.x = 0;
	if (tParticleAtt->_color.x > 255) tParticleAtt->_color.x = 255;
	if (tParticleAtt->_color.y < 0) tParticleAtt->_color.y = 0;
	if (tParticleAtt->_color.y > 255) tParticleAtt->_color.y = 255;
	if (tParticleAtt->_color.z	< 0) tParticleAtt->_color.z = 0;
	if (tParticleAtt->_color.z	> 255) tParticleAtt->_color.z = 255;

}

HRESULT CParticleObject::Reset_FixedPostion(_float3 vPos)
{
	m_ParticleDesc.FixedTarget = vPos;

	return S_OK;
}

_int CParticleObject::Render()
{
	if (0 > __super::Render())
		return -1;


	// 텍스처 바인딩은 한번만 하면 된다.
	SetUp_RenderState();

	if (m_ParticleDesc.m_bIsTextureAutoFrame)
	{
		FAILED_CHECK(m_ComTexture->Bind_Texture_AutoFrame(g_fDeltaTime * m_ParticleDesc.fAutoFrameMul));
	}
	else
	{
		FAILED_CHECK(m_ComTexture->Bind_Texture());
	}
	// 파티클 리스트를 한번에 Render
	if (false == m_list_Particles.empty())
	{

		for (auto iter: m_list_Particles)
		{
			if (iter._isAlive)
			{
				if (m_ParticleDesc.ParticleColorChage)
				{
					m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_XRGB(_uint(iter._color.x), _uint(iter._color.y), _uint(iter._color.z)));
				}

				if (m_ParticleDesc.m_bIsUI && m_ParticleDesc.FollowingTarget == nullptr)
				{
					_float3 UIPos = { 0,0,0 };
					UIPos.x = iter._position.x - g_iWinCX *0.5f;
					UIPos.y = -iter._position.y + g_iWinCY *0.5f;
					m_ComParticleTransform->Set_MatrixState(CTransform::STATE_POS, UIPos);
				}
				else {

				m_ComParticleTransform->Set_MatrixState(CTransform::STATE_POS, iter._position);
				}
				FAILED_CHECK(m_ComParticleTransform->Bind_WorldMatrix_Look_Camera());
				FAILED_CHECK(m_ComVIBuffer->Render());
			}
			
		}
	}
	//m_ComParentTransform->Set_MatrixState(CTransform::STATE_POS, Get_ParentPos());

	Release_RenderState();

	return _int();
}

_int CParticleObject::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_float3 CParticleObject::Get_ParentPos()
{

	if (m_ComParentTransform == nullptr)
		return _float3(0, 0, 0);


	return m_ComParentTransform->Get_MatrixState(CTransform::STATE_POS);

}



HRESULT CParticleObject::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_SubTransform"), (CComponent**)&m_ComParticleTransform, &TransformDesc)))
		return E_FAIL;
	TransformDesc.fMovePerSec = 2.f;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComParentTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, m_ParticleDesc.szTextureProtoTypeTag, TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
	{
		if (FAILED(__super::Add_Component(SCENE_STATIC, m_ParticleDesc.szTextureProtoTypeTag, TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
			return E_FAIL;
	}

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer_Rect */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	return S_OK;
}

HRESULT CParticleObject::SetUp_ParticleDesc(void * pArg)
{
	if (pArg == nullptr)
		return E_FAIL;


	memcpy(&m_ParticleDesc, pArg, sizeof(PARTICLEDESC));

	m_boundingBox.minPos = m_ParticleDesc.MaxBoundary.Get_Inverse();
	m_boundingBox.maxPos = m_ParticleDesc.MaxBoundary;

	m_vUp = m_ParticleDesc.vUp.Get_Nomalize();

	if (m_vUp != _float3(0, 1, 0))
	{

		if (m_vUp == _float3(0, -1, 0))
		{
			m_vRight = (m_vUp.Get_Cross(_float3(0.00000001f, 1, 0))).Get_Nomalize();
			m_vLook = (m_vRight.Get_Cross(m_vUp)).Get_Nomalize();

		}
		else 
		{
			m_vRight = (m_vUp.Get_Cross(_float3(0, 1, 0))).Get_Nomalize();
			m_vLook = (m_vRight.Get_Cross(m_vUp)).Get_Nomalize();
		}
	}

	if (m_ParticleDesc.PowerRandomRange.y - m_ParticleDesc.PowerRandomRange.x < 0.1f )
		m_ParticleDesc.PowerRandomRange.y += 0.1f;




	return S_OK;
}


HRESULT CParticleObject::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	if (m_ParticleDesc.MustDraw)
	{
		m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	if (m_ParticleDesc.AlphaBlendON)
	{

		m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, _uint(m_ParticleDesc.m_fAlphaTestValue));
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		//

	}
	else {

		m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, _uint(m_ParticleDesc.m_fAlphaTestValue));
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	}
	if (m_ParticleDesc.ParticleColorChage)
	{
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	}

	return S_OK;
}

HRESULT CParticleObject::Release_RenderState()
{
	if (m_ParticleDesc.MustDraw && m_ParticleDesc.m_bIsUI ==false)
	{
		m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

	return S_OK;
}



void CParticleObject::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComParentTransform);
	Safe_Release(m_ComParticleTransform);
	Safe_Release(m_ComRenderer);
}
///////////구 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CParticleeObj_Ball::CParticleeObj_Ball(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CParticleObject(pGraphic_Device)
{
}

CParticleeObj_Ball::CParticleeObj_Ball(const CParticleeObj_Ball & rhs)
	: CParticleObject(rhs)
{
}

void CParticleeObj_Ball::Reset_Velocity(_float3 & fAttVlocity)
{
	_float3 RandomVelocity = _float3(0, 0, 0);
	GetRandomVector(&RandomVelocity, &_float3(-5,-5,-5), &_float3(5, 5, 5));

	if (m_ParticleDesc.m_bIsUI)
		RandomVelocity.z = 0;


	fAttVlocity = RandomVelocity.Get_Nomalize();
}

void CParticleeObj_Ball::Update_Position_by_Velocity(PARTICLEATT* tParticleAtt,_float fTimeDelta)
{

	tParticleAtt->_position += tParticleAtt->_velocity * tParticleAtt->_force * fTimeDelta;
}

HRESULT CParticleeObj_Ball::Initialize_Child_Clone()
{

	m_list_Particles.clear();

	// 자식 객체 생성
	PARTICLEATT part;
	_float3 RandomVelocity = _float3(0,0,0);
	


	_float3 RandomPos = _float3(0, 0, 0);

//	part._color = COLOR_BLUE;
//	part._colorFade = COLOR_WHITE;
	for (_uint i = 0;i<m_ParticleDesc.MaxParticleCount;i++)
	{
		part._lifeTime = m_ParticleDesc.EachParticleLifeTime * (((_float)(rand() % 1500 + 500))*0.001f);
		part._age = 0;
		GetRandomVector(&RandomPos, &m_ParticleDesc.ParticleStartRandomPosMin, &m_ParticleDesc.ParticleStartRandomPosMax);
		if (m_ParticleDesc.m_bIsUI)
			RandomPos.z = 0;
		part._position = Get_ParentPos() + RandomPos;

		part._color = m_ParticleDesc.TargetColor2;
		Reset_Velocity(part._velocity);

		part._NowparantPos = m_ComParentTransform->Get_MatrixState(CTransform::STATE_POS);

		part._force = m_ParticleDesc.Particle_Power *
			((((_float)(rand() % _uint((m_ParticleDesc.PowerRandomRange.y - m_ParticleDesc.PowerRandomRange.x)*1000.f)) + m_ParticleDesc.PowerRandomRange.x * 1000.f))*0.001f);

		m_list_Particles.push_front(part);
	}
	return S_OK;
}

_int CParticleeObj_Ball::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;
	return 0;
}

_int CParticleeObj_Ball::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;
	return 0;
}

CParticleeObj_Ball * CParticleeObj_Ball::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParticleeObj_Ball* pInstance = new CParticleeObj_Ball(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Ball");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CParticleeObj_Ball::Clone(void * pArg)
{
	CParticleeObj_Ball* pInstance = new CParticleeObj_Ball(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Ball");
		Safe_Release(pInstance);
	}

	return pInstance;
}

///////////직선 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CParticleeObj_Straight::CParticleeObj_Straight(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CParticleObject(pGraphic_Device)
{
}

CParticleeObj_Straight::CParticleeObj_Straight(const CParticleeObj_Straight & rhs)
	: CParticleObject(rhs)
{
}

void CParticleeObj_Straight::Reset_Velocity(_float3 & fAttVlocity)
{
	/*_float3 RandomVelocity = _float3(0, 0, 0);
	GetRandomVector(&RandomVelocity, &_float3(0, 0, 0), &m_vUp);*/
	if (m_ParticleDesc.m_bIsUI)
		m_vUp.z = 0;

	fAttVlocity = m_vUp.Get_Nomalize();
}

void CParticleeObj_Straight::Update_Position_by_Velocity(PARTICLEATT * tParticleAtt, _float fTimeDelta)
{
	tParticleAtt->_position += tParticleAtt->_velocity * tParticleAtt->_force * fTimeDelta;
}

HRESULT CParticleeObj_Straight::Initialize_Child_Clone()
{
	m_list_Particles.clear();

	// 자식 객체 생성
	PARTICLEATT part;
	_float3 RandomVelocity = _float3(0, 0, 0);



	_float3 RandomPos = _float3(0, 0, 0);


	//	part._color = COLOR_BLUE;
	//	part._colorFade = COLOR_WHITE;
	for (_uint i = 0; i < m_ParticleDesc.MaxParticleCount; i++)
	{
		part._lifeTime = m_ParticleDesc.EachParticleLifeTime * (((_float)(rand() % 1500 + 500))*0.001f);
		part._age = 0;
		GetRandomVector(&RandomPos, &m_ParticleDesc.ParticleStartRandomPosMin, &m_ParticleDesc.ParticleStartRandomPosMax);
		if (m_ParticleDesc.m_bIsUI)
			RandomPos.z = 0;
		part._position = Get_ParentPos() + RandomPos;

		Reset_Velocity(part._velocity);
		part._NowparantPos = m_ComParentTransform->Get_MatrixState(CTransform::STATE_POS);
		part._force = m_ParticleDesc.Particle_Power *
			((((_float)(rand() % _uint((m_ParticleDesc.PowerRandomRange.y - m_ParticleDesc.PowerRandomRange.x)*1000.f)) + m_ParticleDesc.PowerRandomRange.x * 1000.f))*0.001f);
		m_list_Particles.push_front(part);
		part._color = m_ParticleDesc.TargetColor2;
	}


	return S_OK;
}

_int CParticleeObj_Straight::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;
	return _int();
}

_int CParticleeObj_Straight::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;
	return _int();
}

CParticleeObj_Straight * CParticleeObj_Straight::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParticleeObj_Straight* pInstance = new CParticleeObj_Straight(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Straight");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CParticleeObj_Straight::Clone(void * pArg)
{
	CParticleeObj_Straight* pInstance = new CParticleeObj_Straight(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Straight");
		Safe_Release(pInstance);
	}

	return pInstance;
}


///////////분수 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CParticleeObj_Fountain::CParticleeObj_Fountain(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CParticleObject(pGraphic_Device)
{
}

CParticleeObj_Fountain::CParticleeObj_Fountain(const CParticleeObj_Fountain & rhs)
	: CParticleObject(rhs)
{
}

void CParticleeObj_Fountain::Reset_Velocity(_float3 & fAttVlocity)
{
	_float3 RandomVelocity = _float3(0, 0, 0);

	_float3 vRightAddLook = _float3(0,0,0);

	GetRandomVector(&RandomVelocity, &(m_vRight.Get_Inverse()), &m_vRight);
	vRightAddLook += RandomVelocity;
	GetRandomVector(&RandomVelocity, &(m_vLook.Get_Inverse()), &m_vLook);
	vRightAddLook += RandomVelocity;

	if (m_ParticleDesc.m_bIsUI)
		vRightAddLook.z = 0;

	fAttVlocity = vRightAddLook.Get_Nomalize();
}

void CParticleeObj_Fountain::Update_Position_by_Velocity(PARTICLEATT * tParticleAtt, _float fTimeDelta)
{

	tParticleAtt->_position += tParticleAtt->_velocity * tParticleAtt->_force * 0.3f * fTimeDelta;

	if (tParticleAtt->_age < tParticleAtt->_lifeTime * 0.7f )
	{

		_float TempForce = GetSingle(CGameInstance)->Easing(TYPE_QuadOut,
			tParticleAtt->_force, 0,
			tParticleAtt->_age,
			tParticleAtt->_lifeTime* 0.7f);

		tParticleAtt->_position += m_vUp * TempForce * fTimeDelta;


	}
	else {

		_float TempForce = GetSingle(CGameInstance)->Easing(TYPE_QuadOut,
			0, tParticleAtt->_force,
			tParticleAtt->_age - tParticleAtt->_lifeTime * 0.7f,
			tParticleAtt->_lifeTime * 0.3f);

		tParticleAtt->_position += _float3(0,-1,0)* TempForce * fTimeDelta;

	}


}

void CParticleeObj_Fountain::ResetParticle(PARTICLEATT * attribute)
{
	_float3 RandomPos = _float3(0, 0, 0);
	GetRandomVector(&RandomPos, &m_ParticleDesc.ParticleStartRandomPosMin, &m_ParticleDesc.ParticleStartRandomPosMax);
	if (m_ParticleDesc.m_bIsUI)
		RandomPos.z = 0;
	attribute->_position = Get_ParentPos() + RandomPos;

	attribute->_lifeTime = m_ParticleDesc.EachParticleLifeTime * (((_float)(rand() % 1500 + 500))*0.001f);
	attribute->_age = 0;

	attribute->_force = m_ParticleDesc.Particle_Power *
		((((_float)(rand() % _uint((m_ParticleDesc.PowerRandomRange.y - m_ParticleDesc.PowerRandomRange.x)*1000.f)) + m_ParticleDesc.PowerRandomRange.x * 1000.f))*0.001f);

	attribute->_NowparantPos = m_ComParentTransform->Get_MatrixState(CTransform::STATE_POS);

	attribute->_color = m_ParticleDesc.TargetColor2;
	Reset_Velocity(attribute->_velocity);
}

HRESULT CParticleeObj_Fountain::Initialize_Child_Clone()
{
	m_list_Particles.clear();

	// 자식 객체 생성
	PARTICLEATT part;
	_float3 RandomVelocity = _float3(0, 0, 0);



	_float3 RandomPos = _float3(0, 0, 0);


	//	part._color = COLOR_BLUE;
	//	part._colorFade = COLOR_WHITE;
	for (_uint i = 0; i < m_ParticleDesc.MaxParticleCount; i++)
	{
		GetRandomVector(&RandomPos, &m_ParticleDesc.ParticleStartRandomPosMin, &m_ParticleDesc.ParticleStartRandomPosMax);
		if (m_ParticleDesc.m_bIsUI)
			RandomPos.z = 0;
		part._lifeTime = m_ParticleDesc.EachParticleLifeTime * (((_float)(rand() % 1500 + 500))*0.001f);
		part._age = 0;

		part._position = Get_ParentPos() + RandomPos;

		Reset_Velocity(part._velocity);
		part._NowparantPos = m_ComParentTransform->Get_MatrixState(CTransform::STATE_POS);

		part._force = m_ParticleDesc.Particle_Power *
			((((_float)(rand() % _uint((m_ParticleDesc.PowerRandomRange.y - m_ParticleDesc.PowerRandomRange.x)*1000.f)) + m_ParticleDesc.PowerRandomRange.x * 1000.f))*0.001f); 
		m_list_Particles.push_front(part);
		part._color = m_ParticleDesc.TargetColor2;
	}
	return S_OK;
}

_int CParticleeObj_Fountain::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;
	return _int();
}

_int CParticleeObj_Fountain::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;
	return _int();
}

CParticleeObj_Fountain * CParticleeObj_Fountain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParticleeObj_Fountain* pInstance = new CParticleeObj_Fountain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Fountain");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CParticleeObj_Fountain::Clone(void * pArg)
{
	CParticleeObj_Fountain* pInstance = new CParticleeObj_Fountain(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Fountain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

///////////부채 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CParticleeObj_Cone::CParticleeObj_Cone(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CParticleObject(pGraphic_Device)
{
}

CParticleeObj_Cone::CParticleeObj_Cone(const CParticleeObj_Cone & rhs)
	: CParticleObject(rhs)
{
}

void CParticleeObj_Cone::Reset_Velocity(_float3 & fAttVlocity)
{

	_float3 vAddedRightLook = _float3(0, 0, 0);
	vAddedRightLook += m_vRight;
	//vAddedRightLook += m_vUp;
	vAddedRightLook += m_vLook;

	
	_float3 RandomVelocity = _float3(0, 0, 0);

	GetRandomVector(&RandomVelocity, &(vAddedRightLook.Get_Inverse()), &(vAddedRightLook));

	//RandomVelocity += m_vUp * (((_float)(rand() % 500 + 1000))*0.001f);
	RandomVelocity += m_vUp;


	if (m_ParticleDesc.m_bIsUI)
		RandomVelocity.z = 0;

	fAttVlocity = RandomVelocity.Get_Nomalize();
}

void CParticleeObj_Cone::Update_Position_by_Velocity(PARTICLEATT * tParticleAtt, _float fTimeDelta)
{
	tParticleAtt->_position += tParticleAtt->_velocity * tParticleAtt->_force * fTimeDelta;
}


HRESULT CParticleeObj_Cone::Initialize_Child_Clone()
{
	m_list_Particles.clear();

	// 자식 객체 생성
	PARTICLEATT part;
	_float3 RandomVelocity = _float3(0, 0, 0);



	_float3 RandomPos = _float3(0, 0, 0);


	//	part._color = COLOR_BLUE;
	//	part._colorFade = COLOR_WHITE;
	for (_uint i = 0; i < m_ParticleDesc.MaxParticleCount; i++)
	{
		part._lifeTime = m_ParticleDesc.EachParticleLifeTime * (((_float)(rand() % 1500 + 500)) * 0.001f);
		part._age = 0;
		GetRandomVector(&RandomPos, &m_ParticleDesc.ParticleStartRandomPosMin, &m_ParticleDesc.ParticleStartRandomPosMax);
		if (m_ParticleDesc.m_bIsUI)
			RandomPos.z = 0;
		part._position = Get_ParentPos() + RandomPos;

		part._color = m_ParticleDesc.TargetColor2;
		Reset_Velocity(part._velocity);

		part._force = m_ParticleDesc.Particle_Power *
			((((_float)(rand() % _uint((m_ParticleDesc.PowerRandomRange.y - m_ParticleDesc.PowerRandomRange.x)*1000.f)) + m_ParticleDesc.PowerRandomRange.x * 1000.f))*0.001f); 
		m_list_Particles.push_front(part);
	}
	return S_OK;
}

_int CParticleeObj_Cone::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;
	return _int();
}

_int CParticleeObj_Cone::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;
	return _int();
}

CParticleeObj_Cone * CParticleeObj_Cone::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParticleeObj_Cone* pInstance = new CParticleeObj_Cone(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Cone");
		Safe_Release(pInstance);

	}

	return pInstance;
}

CGameObject * CParticleeObj_Cone::Clone(void * pArg)
{
	CParticleeObj_Cone* pInstance = new CParticleeObj_Cone(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Cone");
		Safe_Release(pInstance);
	}

	return pInstance;
}


///////////퍼지는 형태 파티클///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CParticleeObj_Spread::CParticleeObj_Spread(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CParticleObject(pGraphic_Device)
{
}

CParticleeObj_Spread::CParticleeObj_Spread(const CParticleeObj_Spread & rhs)
	: CParticleObject(rhs)
{
}

void CParticleeObj_Spread::Reset_Velocity(_float3 & fAttVlocity)
{
	_float3 RandomVelocity = _float3(0, 0, 0);

	_float3 vRightAddLook = _float3(0, 0, 0);

	GetRandomVector(&RandomVelocity, &(m_vRight.Get_Inverse()), &m_vRight);
	vRightAddLook += RandomVelocity;
	GetRandomVector(&RandomVelocity, &(m_vLook.Get_Inverse()), &m_vLook);
	vRightAddLook += RandomVelocity;



	if (m_ParticleDesc.m_bIsUI)
		vRightAddLook.z = 0;

	fAttVlocity = vRightAddLook.Get_Nomalize();
}

void CParticleeObj_Spread::Update_Position_by_Velocity(PARTICLEATT * tParticleAtt, _float fTimeDelta)
{
	tParticleAtt->_position += tParticleAtt->_velocity * tParticleAtt->_force * fTimeDelta;

	if (tParticleAtt->_age < tParticleAtt->_lifeTime * 0.7f)
	{

		_float TempForce = GetSingle(CGameInstance)->Easing(TYPE_QuadOut,
			tParticleAtt->_force * 0.4f, 0,
			tParticleAtt->_age,
			tParticleAtt->_lifeTime* 0.7f);

		tParticleAtt->_position += m_vUp * TempForce * fTimeDelta;


	}
	else {

		_float TempForce = GetSingle(CGameInstance)->Easing(TYPE_QuadOut,
			0, tParticleAtt->_force * 0.4f,
			tParticleAtt->_age - tParticleAtt->_lifeTime * 0.7f,
			tParticleAtt->_lifeTime * 0.3f);

		tParticleAtt->_position += _float3(0, -1, 0)* TempForce * fTimeDelta;

	}

}

void CParticleeObj_Spread::ResetParticle(PARTICLEATT * attribute)
{
	_float3 RandomPos = _float3(0, 0, 0);
	GetRandomVector(&RandomPos, &m_ParticleDesc.ParticleStartRandomPosMin, &m_ParticleDesc.ParticleStartRandomPosMax);
	if (m_ParticleDesc.m_bIsUI)
		RandomPos.z = 0;
	attribute->_position = Get_ParentPos() + RandomPos;

	attribute->_lifeTime = m_ParticleDesc.EachParticleLifeTime * (((_float)(rand() % 1500 + 500))*0.001f);
	attribute->_age = 0;

	attribute->_color = m_ParticleDesc.TargetColor2;
	attribute->_force = m_ParticleDesc.Particle_Power *
		((((_float)(rand() % _uint((m_ParticleDesc.PowerRandomRange.y - m_ParticleDesc.PowerRandomRange.x)*1000.f)) + m_ParticleDesc.PowerRandomRange.x * 1000.f))*0.001f);

	attribute->_NowparantPos = m_ComParentTransform->Get_MatrixState(CTransform::STATE_POS);

	Reset_Velocity(attribute->_velocity);
}

HRESULT CParticleeObj_Spread::Initialize_Child_Clone()
{
	m_list_Particles.clear();

	// 자식 객체 생성
	PARTICLEATT part;
	_float3 RandomVelocity = _float3(0, 0, 0);



	_float3 RandomPos = _float3(0, 0, 0);


	//	part._color = COLOR_BLUE;
	//	part._colorFade = COLOR_WHITE;
	for (_uint i = 0; i < m_ParticleDesc.MaxParticleCount; i++)
	{
		GetRandomVector(&RandomPos, &m_ParticleDesc.ParticleStartRandomPosMin, &m_ParticleDesc.ParticleStartRandomPosMax);
		if (m_ParticleDesc.m_bIsUI)
			RandomPos.z = 0;
		part._lifeTime = m_ParticleDesc.EachParticleLifeTime * (((_float)(rand() % 1500 + 500))*0.001f);
		part._age = 0;

		part._position = Get_ParentPos() + RandomPos;

		part._color = m_ParticleDesc.TargetColor2;
		Reset_Velocity(part._velocity);
		part._NowparantPos = m_ComParentTransform->Get_MatrixState(CTransform::STATE_POS);

		part._force = m_ParticleDesc.Particle_Power *
			((((_float)(rand() % _uint((m_ParticleDesc.PowerRandomRange.y - m_ParticleDesc.PowerRandomRange.x)*1000.f)) + m_ParticleDesc.PowerRandomRange.x * 1000.f))*0.001f); 
		m_list_Particles.push_front(part);
	}
	return S_OK;
}

_int CParticleeObj_Spread::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;
	return _int();
}

_int CParticleeObj_Spread::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;
	return _int();
}

CParticleeObj_Spread * CParticleeObj_Spread::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParticleeObj_Spread* pInstance = new CParticleeObj_Spread(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Spread");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CParticleeObj_Spread::Clone(void * pArg)
{
	CParticleeObj_Spread* pInstance = new CParticleeObj_Spread(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Spread");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleeObj_Fixed::CParticleeObj_Fixed(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CParticleObject(pGraphic_Device)
{
}

CParticleeObj_Fixed::CParticleeObj_Fixed(const CParticleeObj_Fixed & rhs)
	: CParticleObject(rhs)
{
}

void CParticleeObj_Fixed::Reset_Velocity(_float3 & fAttVlocity)
{

}

void CParticleeObj_Fixed::Update_Position_by_Velocity(PARTICLEATT * tParticleAtt, _float fTimeDelta)
{



}

void CParticleeObj_Fixed::ResetParticle(PARTICLEATT * attribute)
{
	_float3 RandomPos = _float3(0, 0, 0);
	GetRandomVector(&RandomPos, &m_ParticleDesc.ParticleStartRandomPosMin, &m_ParticleDesc.ParticleStartRandomPosMax);
	if (m_ParticleDesc.m_bIsUI)
		RandomPos.z = 0;
	attribute->_position = Get_ParentPos() + RandomPos;

	attribute->_lifeTime = m_ParticleDesc.EachParticleLifeTime * (((_float)(rand() % 1500 + 500))*0.001f);
	attribute->_age = 0;


	attribute->_NowparantPos = m_ComParentTransform->Get_MatrixState(CTransform::STATE_POS);

}

HRESULT CParticleeObj_Fixed::Initialize_Child_Clone()
{
	m_list_Particles.clear();

	// 자식 객체 생성
	PARTICLEATT part;


	_float3 RandomPos = _float3(0, 0, 0);


	//	part._color = COLOR_BLUE;
	//	part._colorFade = COLOR_WHITE;

	for (_uint i = 0; i < m_ParticleDesc.MaxParticleCount; i++)
	{
		GetRandomVector(&RandomPos, &m_ParticleDesc.ParticleStartRandomPosMin, &m_ParticleDesc.ParticleStartRandomPosMax);

		if (m_ParticleDesc.m_bIsUI)
			RandomPos.z = 0;

		part._lifeTime = m_ParticleDesc.EachParticleLifeTime * (((_float)(rand() % 1500 + 500))*0.001f);
		part._age = 0;

		part._position = Get_ParentPos() + RandomPos;

		part._NowparantPos = m_ComParentTransform->Get_MatrixState(CTransform::STATE_POS);

		m_list_Particles.push_front(part);
	}
	return S_OK;
}

_int CParticleeObj_Fixed::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;
	return _int();
}

_int CParticleeObj_Fixed::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;
	return _int();
}

CParticleeObj_Fixed * CParticleeObj_Fixed::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParticleeObj_Fixed* pInstance = new CParticleeObj_Fixed(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Fixed");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CParticleeObj_Fixed::Clone(void * pArg)
{
	CParticleeObj_Fixed* pInstance = new CParticleeObj_Fixed(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Fixed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


//////////////////////////////////////////////////////////////////////////

CParticleeObj_Suck::CParticleeObj_Suck(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CParticleObject(pGraphic_Device)
{
}

CParticleeObj_Suck::CParticleeObj_Suck(const CParticleeObj_Suck & rhs)
	: CParticleObject(rhs)
{
}

void CParticleeObj_Suck::Reset_Velocity(_float3 & fAttVlocity)
{
}

void CParticleeObj_Suck::Update_Position_by_Velocity(PARTICLEATT * tParticleAtt, _float fTimeDelta)
{
	_float3 TargetPos = {};
	if (m_ParticleDesc.FollowingTarget == nullptr)
	{
		TargetPos = m_ParticleDesc.FixedTarget;

	}
	else {
		TargetPos = m_ParticleDesc.FollowingTarget->Get_MatrixState(CTransform::STATE_POS);
	}

	tParticleAtt->_velocity = (TargetPos - tParticleAtt->_position).Get_Nomalize();


	tParticleAtt->_position += tParticleAtt->_velocity * tParticleAtt->_force * fTimeDelta;

	if (TargetPos.Get_Distance(tParticleAtt->_position) < 0.1f)
		tParticleAtt->_age = tParticleAtt->_lifeTime + 1000.f;


}

void CParticleeObj_Suck::ResetParticle(PARTICLEATT * attribute)
{
	_float3 RandomPos = _float3(0, 0, 0);
	GetRandomVector(&RandomPos, &m_ParticleDesc.ParticleStartRandomPosMin, &m_ParticleDesc.ParticleStartRandomPosMax);

	if (m_ParticleDesc.m_bIsUI)
		RandomPos.z = 0;

	attribute->_position = Get_ParentPos() + RandomPos;

	attribute->_lifeTime = m_ParticleDesc.EachParticleLifeTime * (((_float)(rand() % 1500 + 500))*0.001f);
	attribute->_age = 0;


	attribute->_NowparantPos = m_ComParentTransform->Get_MatrixState(CTransform::STATE_POS);

	attribute->_force = m_ParticleDesc.Particle_Power *
		((((_float)(rand() % _uint((m_ParticleDesc.PowerRandomRange.y - m_ParticleDesc.PowerRandomRange.x)*1000.f)) + m_ParticleDesc.PowerRandomRange.x * 1000.f))*0.001f);

}

HRESULT CParticleeObj_Suck::Initialize_Child_Clone()
{
	m_list_Particles.clear();

	// 자식 객체 생성
	PARTICLEATT part;


	_float3 RandomPos = _float3(0, 0, 0);


	//	part._color = COLOR_BLUE;
	//	part._colorFade = COLOR_WHITE;

	for (_uint i = 0; i < m_ParticleDesc.MaxParticleCount; i++)
	{
		GetRandomVector(&RandomPos, &m_ParticleDesc.ParticleStartRandomPosMin, &m_ParticleDesc.ParticleStartRandomPosMax);

		if (m_ParticleDesc.m_bIsUI)
			RandomPos.z = 0;

		part._lifeTime = m_ParticleDesc.EachParticleLifeTime * (((_float)(rand() % 1500 + 500))*0.001f);
		part._age = 0;

		part._position = Get_ParentPos() + RandomPos;

		part._force = m_ParticleDesc.Particle_Power *
			((((_float)(rand() % _uint((m_ParticleDesc.PowerRandomRange.y - m_ParticleDesc.PowerRandomRange.x)*1000.f)) + m_ParticleDesc.PowerRandomRange.x * 1000.f))*0.001f);

		part._NowparantPos = m_ComParentTransform->Get_MatrixState(CTransform::STATE_POS);

		m_list_Particles.push_front(part);
	}
	return S_OK;
}

_int CParticleeObj_Suck::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	return _int();
}

_int CParticleeObj_Suck::LateUpdate(_float fTimeDelta)
{

	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	return _int();
}

CParticleeObj_Suck * CParticleeObj_Suck::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParticleeObj_Suck* pInstance = new CParticleeObj_Suck(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Suck");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CParticleeObj_Suck::Clone(void * pArg)
{
	CParticleeObj_Suck* pInstance = new CParticleeObj_Suck(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Suck");
		Safe_Release(pInstance);
	}

	return pInstance;
}
