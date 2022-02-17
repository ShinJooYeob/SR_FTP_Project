#include "..\Public\Renderer.h"
#include "GameObject.h"


CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

HRESULT CRenderer::Initialize_Prototype(void * pArg)
{
	D3DVIEWPORT9		ViewPortDesc;
	m_pGraphicDevice->GetViewport(&ViewPortDesc);
	_int iWinCX = ViewPortDesc.Width;
	_int iWinCY = ViewPortDesc.Height;
	D3DXMatrixOrthoLH(&m_ProjMatrix, iWinCX, iWinCY, 0.0f, 1.f);
	return S_OK;
}

HRESULT CRenderer::Initialize_Clone(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderID, CGameObject * pGameObject)
{
	if (eRenderID >= RENDER_END ||
		pGameObject == nullptr)
		return E_FAIL;

	m_RenderObjectList[eRenderID].emplace_back(pGameObject);
	
	Safe_AddRef(pGameObject);


	return S_OK;
}

HRESULT CRenderer::Render_RenderGroup()
{

	for (_uint i =0 ; i<RENDER_END ; ++i)
	{
		if (i == RENDER_UI)
		{
			_Matrix ViewMatrix;
			D3DXMatrixIdentity(&ViewMatrix);
			m_pGraphicDevice->SetTransform(D3DTS_VIEW, &ViewMatrix);
			m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
		}
		for (auto& RenderObject : m_RenderObjectList[i])
		{
			
				if (RenderObject != nullptr)
				{
		
					if (FAILED(RenderObject->Render()))
						return E_FAIL;
					Safe_Release(RenderObject);
				}

		}
	
		m_RenderObjectList[i].clear();
	}
	return S_OK;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CRenderer* pInstance = new CRenderer(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg))) 
	{
		MSGBOX("Failed to Creating CRenderer");
		Safe_Release(pInstance);
	}


	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{

	this->AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& RenderObject : m_RenderObjectList[i])
		{
			Safe_Release(RenderObject);
		}
		m_RenderObjectList[i].clear();
	}
}
