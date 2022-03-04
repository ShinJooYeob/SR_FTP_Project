#include "stdafx.h"
#include "..\public\ParticleObejct.h"


CParticleObject::CParticleObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CParticleObject::CParticleObject(const CParticleObject & rhs)
	: CGameObject(rhs)
{	
    m_RateTime = rhs.m_RateTime;	
    m_Scale = rhs.m_Scale;
	m_list_Particles = rhs.m_list_Particles;	
    m_MaxParticles = rhs.m_MaxParticles;	
	m_boundingBox = rhs.m_boundingBox;
}

HRESULT CParticleObject::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	m_list_Particles.clear();
	m_RateTime = 1.0f;
	m_Scale = 1.0f;
	m_MaxParticles = 10;
	m_boundingBox.minPos = _float3(-10,-10,-10);
	m_boundingBox.maxPos = _float3(10,10,10);

	return S_OK;
}

HRESULT CParticleObject::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	// 부모 객체의 컴포넌트 생성
	// 자식 객체들은 부모객체의 컴포넌트를 사용하게 한다.
	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_ComTexture->Change_TextureLayer(TEXT("Object_Star"));
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(0, 1, 0));
	m_ComTransform->Scaled(_float3(0.3f, 0.3f, 0.3f));

	// 클론 시 자식 객체 생성
	Initialize_Child_Clone();

	return S_OK;
}

_int CParticleObject::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	// 자식 객체 업데이트
	std::list<PARTICLEATT>::iterator i;
	for (i = m_list_Particles.begin(); i != m_list_Particles.end(); i++)
	{
		i->_position += i->_velocity * i->_force* fTimeDelta;

		// 범위 체크
		if (m_boundingBox.isPointInside(i->_position) == false)
		{
			// 리셋
			ResetParticle(&(*i));
		}
	}

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

	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_AFTEROBJ, this);


	return _int();
}

void CParticleObject::ResetParticle(PARTICLEATT * attribute)
{
	attribute->_position = _float3(0, 0, 0);

}

_int CParticleObject::Render()
{
	if (0 > __super::Render())
		return -1;


	// 텍스처 바인딩은 한번만 하면 된다.
	SetUp_RenderState();

	FAILED_CHECK(m_ComTexture->Bind_Texture());

	// 파티클 리스트를 한번에 Render
	if (false == m_list_Particles.empty())
	{

		for (auto iter: m_list_Particles)
		{
			if (iter._isAlive)
			{
				m_ComTransform->Set_MatrixState(CTransform::STATE_POS, iter._position);
				FAILED_CHECK(m_ComTransform->Bind_WorldMatrix());
				FAILED_CHECK(m_ComVIBuffer->Render());
			}
			
		}
	}
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, Get_ParentPos());

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

	if (m_ComVIBuffer == nullptr)
		return _float3(0, 0, 0);
	return m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

}



HRESULT CParticleObject::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_UI_Result_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer_Rect */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	return S_OK;
}

HRESULT CParticleObject::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return S_OK;
}

HRESULT CParticleObject::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	return S_OK;
}

//CParticleObject * CParticleObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
//{
//	CParticleObject* pInstance = new CParticleObject(pGraphic_Device);
//
//	if (FAILED(pInstance->Initialize_Prototype(pArg)))
//	{
//		MSGBOX("Fail to Create CParticleObject");
//		Safe_Release(pInstance);
//
//	}
//
//
//	return pInstance;
//}

//CGameObject * CParticleObject::Clone(void * pArg)
//{
//	CParticleObject* pInstance = new CParticleObject((*this));
//
//	if (FAILED(pInstance->Initialize_Clone(pArg)))
//	{
//		MSGBOX("Fail to Create CParticleObject");
//		Safe_Release(pInstance);
//
//	}
//
//
//	return pInstance;
//}

void CParticleObject::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}

CParticleeObj_Base::CParticleeObj_Base(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CParticleObject(pGraphic_Device)
{
}

CParticleeObj_Base::CParticleeObj_Base(const CParticleeObj_Base & rhs)
	: CParticleObject(rhs)
{
}

HRESULT CParticleeObj_Base::Initialize_Child_Clone()
{

	m_list_Particles.clear();
	m_RateTime = 1.0f;
	m_Scale = 1.0f;
	m_MaxParticles = 10;

	// 자식 객체 생성
	PARTICLEATT part;
	_float3 RandomVelocity = _float3(0,0,0);
	

	part._position = Get_ParentPos();
	

	part._lifeTime = m_RateTime;
	part._age = m_Scale;
//	part._color = COLOR_BLUE;
//	part._colorFade = COLOR_WHITE;

	for (int i = 0;i<m_MaxParticles;i++)
	{
		GetRandomVector(&RandomVelocity, &_float3(-5, -5, -5), &_float3(5, 5, 5));
		part._force = GetRandomFloat(1,10);

		part._velocity = RandomVelocity.Get_Nomalize();
		m_list_Particles.push_front(part);
	}
	return S_OK;
}

_int CParticleeObj_Base::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;
	return 0;
}

_int CParticleeObj_Base::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;
	return 0;
}

CParticleeObj_Base * CParticleeObj_Base::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParticleeObj_Base* pInstance = new CParticleeObj_Base(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Base");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CParticleeObj_Base::Clone(void * pArg)
{
	CParticleeObj_Base* pInstance = new CParticleeObj_Base(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParticleeObj_Base");
		Safe_Release(pInstance);
	}

	return pInstance;
}
