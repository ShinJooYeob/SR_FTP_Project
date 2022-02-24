#include "stdafx.h"
#include "ObjectTool_Terrain.h"
#include "VIBuffer.h"
#include "Picking.h"

CObjectTool_Terrain::CObjectTool_Terrain(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice),
	m_ComRenderer(nullptr),
	m_ComVIBuffer(nullptr),
	m_ComTransform(nullptr)
{
//	m_isVisble = true;
}

CObjectTool_Terrain::CObjectTool_Terrain(const CObjectTool_Terrain & rhs)
	:CGameObject(rhs)
{
}

HRESULT CObjectTool_Terrain::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CObjectTool_Terrain::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CObjectTool_Terrain::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	return 0;
}

_int CObjectTool_Terrain::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;

	//렌더링 그룹에 넣어주는 역활
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_PRIORITY, this)))
		return E_FAIL;

	return 0;
}

_int CObjectTool_Terrain::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	NULL_CHECK_BREAK(m_ComTransform);
	NULL_CHECK_BREAK(m_ComVIBuffer);

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	RenderState_Begin();
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;
	RenderState_End();

	return 0;
}

_int CObjectTool_Terrain::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;
	return 0;
}

HRESULT CObjectTool_Terrain::RenderState_Begin()
{

	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	
	return S_OK;
}

HRESULT CObjectTool_Terrain::RenderState_End()
{
	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	return S_OK;
}

HRESULT CObjectTool_Terrain::SetUp_Components()
{

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Terrain128x128), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	//if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
	//	return E_FAIL;	
	//if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
	//	return E_FAIL;
	return S_OK;
}

CObjectTool_Terrain * CObjectTool_Terrain::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg)
{
	CObjectTool_Terrain* pInstance = new CObjectTool_Terrain(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create ObjectTool_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CObjectTool_Terrain::Clone(void * pArg)
{
	CObjectTool_Terrain* pInstance = new CObjectTool_Terrain((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create ObjectTool_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObjectTool_Terrain::Free()
{
	__super::Free();

	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);

}