#include "stdafx.h"
#include "..\public\MainApp.h"

#include "Scene_Loading.h"

#include "Camera_Main.h"
#include "UI_Mouse.h"
CMainApp::CMainApp()
	:m_pGameInstance(GetSingle(CGameInstance))
{
	m_pGameInstance->AddRef();
}

HRESULT CMainApp::Initialize()
{
	if (m_pGameInstance == nullptr)
		return E_FAIL;

	CGraphic_Device::GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(CGraphic_Device::GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.eScreenMode = CGraphic_Device::TYPE_SCREEN;

	ShowCursor(false);
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, GraphicDesc, SCENEID::SCENE_END, &m_pGraphicDevice)))
		return E_FAIL;

	//// IMGUI �ʱ�ȭ
	//	// IMGUI �ʱ�ȭ
	
	if (GETIMGUI->Initialize_IMGUI(g_hWnd, m_pGraphicDevice))
		return E_FAIL;

	if (FAILED(Default_Setting()))
		return E_FAIL;

	if (FAILED(Ready_Static_Component_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_Static_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Scene_Change(SCENEID::SCENE_LOBY)))
		return E_FAIL;




	return S_OK;
}

_int CMainApp::Update(_float fDeltaTime)
{
	if (m_pGameInstance == nullptr)
		return -1;

	if (FAILED(m_pGameInstance->Update_Engine(fDeltaTime)))
	{
		MSGBOX("Failed to Update_Engine ");
		return E_FAIL;
	}

	//�ݸ��� ���� Ž����
	m_pCollision->Collision_Obsever(fDeltaTime);

	if (FAILED(m_pGameInstance->LateUpdate_Engine(fDeltaTime)))
	{
		MSGBOX("Failed to LateUpdate_Engine ");
		return E_FAIL;
	}

	return 0;
}

HRESULT CMainApp::Render()
{
	if (m_pGameInstance == nullptr)
		return -1;

	m_pGameInstance->Render_Begin();

	m_pComRenderer->Render_RenderGroup();

	HRESULT tResult = m_pGameInstance->Render_Scene();

	m_pGameInstance->Render_End();

	//�ݸ��� ���� ����ִ���
	m_pCollision->Collision_Obsever_Release(); 

	return tResult;

}

HRESULT CMainApp::Scene_Change(SCENEID eSceneID)
{

	if (m_pGameInstance == nullptr)
		return E_FAIL;


	switch (eSceneID)
	{
	case SCENEID::SCENE_LOBY:
	case SCENEID::SCENE_STAGESELECT:
	case SCENEID::SCENE_STAGE1:
	case SCENEID::SCENE_STAGE2:
	case SCENEID::SCENE_STAGE3:
	case SCENEID::SCENE_IMGUISCENE:

		m_pGameInstance->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, eSceneID), SCENEID::SCENE_LOADING);

		break;

	default:
		return E_FAIL;
		break;
	}



	return S_OK;
}

HRESULT CMainApp::Default_Setting()
{
	if (m_pGraphicDevice == nullptr)
		return E_FAIL;

	//���̾� ������
	//m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//��� �������� ������
	m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, false);

	m_pGraphicDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphicDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//�ؽ����� ����� Ȯ��ǰų� ��ҵɶ� ���� �� ����� �߻��� ������ ���� �����ؼ� ä��

	m_pGraphicDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	//�ؽ��ĸ� �ΰ��� ������ �����ϰ� �Ǹ� ���ϰ� �����ؼ� ������ ���� ���� ��찡 �߻�

	m_pGraphicDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphicDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	//UV��ǥ�� 1�� �Ѿ�� �ۼ��� ��� �ٽ� 1�� ���� 0���� ġȯ�Ͽ� ��ǥ�� ����


	return S_OK;
}

HRESULT CMainApp::Ready_Static_Component_Prototype()
{
	if (m_pGameInstance == nullptr)
		return E_FAIL;


	//������ �İ�ü ������Ÿ�� ����
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), m_pComRenderer = CRenderer::Create(m_pGraphicDevice))))
		return E_FAIL;
	Safe_AddRef(m_pComRenderer);

	//�����ε��� ������Ÿ�� ����
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), CVIBuffer_Rect::Create(m_pGraphicDevice))))
		return E_FAIL;
	//Transform ������Ÿ�� ����
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform), CTransform::Create(m_pGraphicDevice))))
		return E_FAIL;

	/* ����Ʈ �ؽ�ó ������Ÿ�� ���� */
	CTexture::TEXTUREDESC TextureDesc{};
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("Player.txt");

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Default), CTexture::Create(m_pGraphicDevice,&TextureDesc))))
		return E_FAIL;

	//�÷��̾� �ؽ�ó ����
	//Player Texture
	TextureDesc.szTextFilePath = TEXT("Player.txt");

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Player), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;


	TextureDesc.szTextFilePath = TEXT("Mouse.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Texture_Mouse"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;
	//��ũ �ؽ�ó
	TextureDesc.szTextFilePath = TEXT("Cam_Effect.txt");
	
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Blank), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//�̴ϸ� �ؽ���
	TextureDesc.szTextFilePath = TEXT("MiniMap.txt");
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_MiniMap), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.szTextFilePath = TEXT("SkyBoxTexture.txt");
	TextureDesc.eTextureType = CTexture::TYPE_CUBEMAP;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Texture_SkyBox"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;


	TextureDesc.szTextFilePath = TEXT("Cubetest.txt");
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Texture_Cube_Default"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;



	// �ݸ��� ������Ÿ�� ����
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Collision"), m_pCollision = CCollision::Create(m_pGraphicDevice))))
		return E_FAIL;
	Safe_AddRef(m_pCollision);

	//�����ε��� ť�� ������Ÿ�� ����
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), CVIBuffer_Cube::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Inventory"), CInventory::Create(m_pGraphicDevice))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Static_GameObject_Prototype()
{	//Camera_Main ������Ÿ�� ����
	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vWorldRotAxis = _float3(0, 0, 0);
	CameraDesc.vEye = _float3(0, 5.f, -10.f);
	CameraDesc.vAt = _float3(0, 5.f, 0);
	CameraDesc.vAxisY = _float3(0, 1, 0);

	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fMovePerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Camera_Main), CCamera_Main::Create(m_pGraphicDevice, &CameraDesc))))
		return E_FAIL;
	
	
	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_Mouse_UI", CUI_Mouse::Create(m_pGraphicDevice, _float4(0, 0, 0, 0)))))
		return E_FAIL;
	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;

	if (FAILED(pInstance->Initialize()))
	{
		MSGBOX("Failed to Creating CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pCollision);
	Safe_Release(m_pComRenderer);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pGameInstance);
}

