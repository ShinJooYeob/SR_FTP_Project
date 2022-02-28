#include "stdafx.h"
#include "ObjectTool_ToolWire.h"
#include "VIBuffer.h"
#include "Picking.h"

CObjectTool_ToolWire::CObjectTool_ToolWire(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice),
	m_ComRenderer(nullptr),
	m_ComVIBuffer(nullptr),
	m_ComTransform(nullptr)
{

}

CObjectTool_ToolWire::CObjectTool_ToolWire(const CObjectTool_ToolWire & rhs)
	:CGameObject(rhs)
{
}

HRESULT CObjectTool_ToolWire::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CObjectTool_ToolWire::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	Set_Pos(_float3(0, 0, -10));
	m_TextureNum = 3;

	return S_OK;
}

_int CObjectTool_ToolWire::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	return _int();
}

_int CObjectTool_ToolWire::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;

	//렌더링 그룹에 넣어주는 역활
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;
	return 0;
}

_int CObjectTool_ToolWire::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	NULL_CHECK_BREAK(m_ComTransform);
	NULL_CHECK_BREAK(m_ComVIBuffer);
	NULL_CHECK_BREAK(m_ComTexture);

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	m_ComTexture->Bind_Texture(m_TextureNum);

	//렌더링 그룹에 들어가면 순서에 맞게 이 랜더가 호출되고 호출이 됬으면 버텍스 버퍼를 그려줘라
	
	RenderState_Begin();

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	RenderState_End();
	return 0;
}

_int CObjectTool_ToolWire::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;
	return 0;
}

HRESULT CObjectTool_ToolWire::Set_Pos(_float3 pos)
{
	if (m_ComTransform == nullptr)
		return E_FAIL;
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, pos);
	return S_OK;
}

HRESULT CObjectTool_ToolWire::Set_TransformMat(_Matrix m)
{
	m_ComTransform->Set_Matrix(m);
	return S_OK;
}



HRESULT CObjectTool_ToolWire::RenderState_Begin()
{
	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//	m_pGraphicDevice->SetRenderState(D3DRS_COLORVERTEX, D3DFILL_SOLID);
	return S_OK;
}

HRESULT CObjectTool_ToolWire::RenderState_End()
{
	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	return S_OK;
}

HRESULT CObjectTool_ToolWire::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Default), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture, &TransformDesc)))
		return E_FAIL;
	return S_OK;
}

CObjectTool_ToolWire * CObjectTool_ToolWire::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg)
{
	CObjectTool_ToolWire* pInstance = new CObjectTool_ToolWire(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create BackGround_ProtoType");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CObjectTool_ToolWire::Clone(void * pArg)
{
	CObjectTool_ToolWire* pInstance = new CObjectTool_ToolWire((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create BackGround_Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObjectTool_ToolWire::Free()
{
	__super::Free();	
	Safe_Release(m_ComTexture);

	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);

}