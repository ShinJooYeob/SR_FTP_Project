#include "stdafx.h"
#include "..\Public\BackGround.h"
#include "Camera_Main.h"


CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice),
	m_ComRenderer(nullptr),
	m_ComVIBuffer(nullptr),
	m_ComTransform(nullptr),
	m_ComTexture(nullptr)
{
}

CBackGround::CBackGround(const CBackGround & rhs)
	:CGameObject(rhs)
{
}

HRESULT CBackGround::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;




	return S_OK;
}

HRESULT CBackGround::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_ComTexture->Change_TextureLayer(TEXT("door"));
	m_ComTransform->Scaled(_float3(1, 1, 1));
	return S_OK;
}


_int CBackGround::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	return _int();
}

_int CBackGround::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;

	//렌더링 그룹에 넣어주는 역활
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;


	return _int();
}

_int CBackGround::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture()))
		return E_FAIL;


	RenderState();
	//렌더링 그룹에 들어가면 순서에 맞게 이 랜더가 호출되고 호출이 됬으면 버텍스 버퍼를 그려줘라
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;
	ReleaseState();

	return _int();
}

_int CBackGround::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

void CBackGround::RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

}

void CBackGround::ReleaseState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CBackGround::SetUp_Components()
{

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec= D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer) , TAG_COM(Com_Renderer),(CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect) , TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform,&TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Texture_JH_Effect"), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	


	return S_OK;
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg)
{
	CBackGround* pInstance = new CBackGround(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg))) 
	{
		MSGBOX("Fail to Create BackGround_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CBackGround::Clone(void * pArg)
{
	CBackGround* pInstance = new CBackGround((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create BackGround_Clone");
		Safe_Release(pInstance);
	}


	return pInstance;
}

void CBackGround::Free()
{

	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
