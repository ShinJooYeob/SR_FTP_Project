#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Camera.h"



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
	D3DXMatrixOrthoLH(&m_ProjMatrix, (_float)iWinCX, (_float)iWinCY, 0.0f, 1.f);


	// 그림자 텍스처 생성
	if (FAILED(m_pGraphicDevice->CreateTexture(iWinCX, iWinCY, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, &m_pMinmapTex, NULL)))
		return E_FAIL;
	if (FAILED(m_pMinmapTex->GetSurfaceLevel(0, &m_pMinmapSurf)))
		return E_FAIL;
	if (FAILED(m_pGraphicDevice->CreateDepthStencilSurface(
		iWinCX, iWinCY, D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0, TRUE,
		&m_pMinmapTexZ, NULL)))
		return E_FAIL;



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

	if (eRenderID == CRenderer::RENDER_ALPHA)
		pGameObject->Compute_CamDistance((CTransform*)(pGameObject->Get_Component(TEXT("Com_Transform"))));

	m_RenderObjectList[eRenderID].emplace_back(pGameObject);
	
	Safe_AddRef(pGameObject);


	return S_OK;
}

HRESULT CRenderer::Render_RenderGroup()
{
	if (m_MainCamera) 
	{
		if (FAILED(Update_MinmapTexture()))
			return E_FAIL;
	}

	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonAlpha()))
		return E_FAIL;

	if (FAILED(Render_Alpha()))
		return E_FAIL;

	FAILED_CHECK(Render_AfterObj())

	if (FAILED(Render_UI()))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderer::Add_MainCamemra(CCamera * pCCamera)
{
	Safe_Release(m_MainCamera);

	m_MainCamera = pCCamera;
	Safe_AddRef(m_MainCamera);

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& RenderObject : m_RenderObjectList[RENDER_PRIORITY])
	{
		if (RenderObject != nullptr)
		{
			if (FAILED(RenderObject->Render()))
				return E_FAIL;
		}
		Safe_Release(RenderObject);
	}
	m_RenderObjectList[RENDER_PRIORITY].clear();
	return S_OK;
}

HRESULT CRenderer::Render_NonAlpha()
{

	for (auto& RenderObject : m_RenderObjectList[RENDER_NONALPHA])
	{
		if (RenderObject != nullptr)
		{
			if (FAILED(RenderObject->Render()))
				return E_FAIL;
		}
		Safe_Release(RenderObject);
	}

	m_RenderObjectList[RENDER_NONALPHA].clear();
	return S_OK;
}

HRESULT CRenderer::Render_Alpha()
{
	m_RenderObjectList[RENDER_ALPHA].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
	{
		return pSour->Get_CamDistance() > pDest->Get_CamDistance();
	});

	for (auto& RenderObject : m_RenderObjectList[RENDER_ALPHA])
	{
		if (RenderObject != nullptr)
		{
			if (FAILED(RenderObject->Render()))
				return E_FAIL;
		}
		Safe_Release(RenderObject);
	}
	m_RenderObjectList[RENDER_ALPHA].clear();
	return S_OK;
}

HRESULT CRenderer::Render_AfterObj()
{
	m_RenderObjectList[RENDER_AFTEROBJ].reverse();
	for (auto& RenderObject : m_RenderObjectList[RENDER_AFTEROBJ])
	{
		if (RenderObject != nullptr)
		{
			if (FAILED(RenderObject->Render()))
				return E_FAIL;
		}
		Safe_Release(RenderObject);
	}
	m_RenderObjectList[RENDER_AFTEROBJ].clear();

	return S_OK;
}

HRESULT CRenderer::Update_MinmapTexture()
{

	LPDIRECT3DSURFACE9 pOldBackBuffer = nullptr, pOldZBuffer = nullptr;


	if (m_MainCamera && FAILED(m_MainCamera->Set_ProjectMatrix(false)))
		return E_FAIL;

	m_pGraphicDevice->GetRenderTarget(0, &pOldBackBuffer);
	m_pGraphicDevice->GetDepthStencilSurface(&pOldZBuffer);

	m_pGraphicDevice->SetRenderTarget(0, m_pMinmapSurf);				
	m_pGraphicDevice->SetDepthStencilSurface(m_pMinmapTexZ);

	m_pGraphicDevice->Clear(0L, NULL
		, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, 0x00000000, 1.0f, 0L);


	for (auto& RenderObject : m_RenderObjectList[RENDER_PRIORITY])
	{
		if (RenderObject != nullptr)
		{
			if (FAILED(RenderObject->Render()))
				return E_FAIL;
		}
	}
	if (m_MainCamera && FAILED(m_MainCamera->Set_ProjectMatrix(false)))
		return E_FAIL;

	for (auto& RenderObject : m_RenderObjectList[RENDER_NONALPHA])
	{
		if (RenderObject != nullptr)
		{
			if (FAILED(RenderObject->Render()))
				return E_FAIL;
		}
	}
	for (auto& RenderObject : m_RenderObjectList[RENDER_ALPHA])
	{
		if (RenderObject != nullptr)
		{
			if (FAILED(RenderObject->Render()))
				return E_FAIL;
		}
	}
	for (auto& RenderObject : m_RenderObjectList[RENDER_AFTEROBJ])
	{
		if (RenderObject != nullptr)
		{
			if (FAILED(RenderObject->Render()))
				return E_FAIL;
		}
	}


	m_pGraphicDevice->SetRenderTarget(0, pOldBackBuffer);
	m_pGraphicDevice->SetDepthStencilSurface(pOldZBuffer);

	pOldBackBuffer->Release();
	pOldZBuffer->Release();



	if (m_MainCamera && FAILED(m_MainCamera->Set_ProjectMatrix(true)))
		return E_FAIL;



	Safe_Release(m_MainCamera);




	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	if (m_pGraphicDevice == nullptr)
		return E_FAIL;

	_Matrix BeforeViewMatrix, BeforeProjectmat;
	m_pGraphicDevice->GetTransform(D3DTS_VIEW, &BeforeViewMatrix);
	m_pGraphicDevice->GetTransform(D3DTS_PROJECTION, &BeforeProjectmat);


	_Matrix ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);
	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);


	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_RenderObjectList[RENDER_UI].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
	{
		return pSour->Get_CamDistance() > pDest->Get_CamDistance();
	});

	for (auto& RenderObject : m_RenderObjectList[RENDER_UI])
	{
		if (RenderObject != nullptr)
		{
			if (FAILED(RenderObject->Render()))
				return E_FAIL;
		}
		Safe_Release(RenderObject);
	}
	m_RenderObjectList[RENDER_UI].clear();


	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &BeforeViewMatrix);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &BeforeProjectmat);
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

	Safe_Release(m_pMinmapTex);
	Safe_Release(m_pMinmapSurf);
	Safe_Release(m_pMinmapTexZ);

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& RenderObject : m_RenderObjectList[i])
		{
			Safe_Release(RenderObject);
		}
		m_RenderObjectList[i].clear();
	}
}
