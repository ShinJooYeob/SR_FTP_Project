#include "stdafx.h"
#include "..\public\MonsterParent.h"

CMonsterParent::CMonsterParent(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
	m_ComTransform = nullptr;
	m_ComRenderer = nullptr;
	m_ComVIBuffer = nullptr;
	m_ComTexture = nullptr;
	m_ComCollision = nullptr;
	m_ComShader = nullptr;
}

CMonsterParent::CMonsterParent(const CMonsterParent& rhs)
	: CGameObject(rhs)
{
	
	m_ComTransform = rhs.m_ComTransform;
	m_ComRenderer = rhs.m_ComRenderer;
	m_ComVIBuffer = rhs.m_ComVIBuffer;
	m_ComTexture = rhs.m_ComTexture;
	m_ComCollision = rhs.m_ComCollision;
	m_ComShader = rhs.m_ComShader;

	Safe_AddRef(m_ComTransform);
	Safe_AddRef(m_ComRenderer);
	Safe_AddRef(m_ComVIBuffer);
	Safe_AddRef(m_ComTexture);
	Safe_AddRef(m_ComCollision);
	Safe_AddRef(m_ComShader);
}


HRESULT CMonsterParent::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonsterParent::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	return S_OK;
}

_int CMonsterParent::Update(_float fDeltaTime)
{
	FAILED_CHECK(__super::Update(fDeltaTime));

	return _int();
}

_int CMonsterParent::LateUpdate(_float fDeltaTime)
{
	FAILED_CHECK(__super::LateUpdate(fDeltaTime));


	return _int();
}

_int CMonsterParent::Render()
{


	FAILED_CHECK(__super::Render());

	// 세이더 랜더 방식
	if (nullptr == m_ComVIBuffer ||
		nullptr == m_ComShader	 ||
		nullptr == m_ComTexture)
		return E_FAIL;



	_Matrix	projMat,viewMat;
	m_pGraphicDevice->GetTransform(D3DTS_PROJECTION,&projMat);
	m_pGraphicDevice->GetTransform(D3DTS_VIEW,&viewMat);

	// 셰이더 월드 설정
	m_ComShader->SetUp_ValueOnShader("g_WorldMatrix", m_ComTransform->Get_WorldMatrix(), sizeof(_Matrix));
	m_ComShader->SetUp_ValueOnShader("g_ViewMatrix", &viewMat, sizeof(_Matrix));
	m_ComShader->SetUp_ValueOnShader("g_ProjMatrix", &projMat, sizeof(_Matrix));
	// 셰이더 텍스처 설정
//	m_ComTexture->Bind_OnShader(m_ComShader, "g_Texture");

	m_ComShader->Begin_Shader(0);
	m_ComVIBuffer->Render();
	m_ComShader->End_Shader();

	return S_OK;
}

_int CMonsterParent::LateRender()
{
	FAILED_CHECK(__super::LateRender());
	
	return S_OK();
}

HRESULT CMonsterParent::SetUp_Components()
{

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_Test), TAG_COM(Com_Shader), (CComponent**)&m_ComShader)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Texture_Default), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	// 정의 안한것은 자식클래스에서

	return S_OK;
}

void CMonsterParent::Free()
{
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComCollision);	
	Safe_Release(m_ComShader);
	__super::Free();
	
}
