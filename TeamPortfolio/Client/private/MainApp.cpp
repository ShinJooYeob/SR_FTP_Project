#include "stdafx.h"
#include "..\public\MainApp.h"

#include "Scene_Loading.h"

#include "Camera_Main.h"

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

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, GraphicDesc, SCENEID::SCENE_END, &m_pGraphicDevice)))
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
	return m_pGameInstance->Update_Engine(fDeltaTime);
}

HRESULT CMainApp::Render()
{
	if (m_pGameInstance == nullptr)
		return -1;

	m_pGameInstance->Render_Begin();



	m_pComRenderer->Render_RenderGroup();

	HRESULT tResult = m_pGameInstance->Render_Scene();

	m_pGameInstance->Render_End();

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
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), m_pComRenderer = CRenderer::Create(m_pGraphicDevice))))
		return E_FAIL;
	Safe_AddRef(m_pComRenderer);


	//�����ε��� ������Ÿ�� ����
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphicDevice))))
		return E_FAIL;



	//Transform ������Ÿ�� ����
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphicDevice))))
		return E_FAIL;

	/* ����Ʈ �ؽ�ó ������Ÿ�� ���� */
	CTexture::TEXTUREDESC TextureDesc{};
	TextureDesc.szFilePath = TEXT("../Bin/Resources/Textures/Default.jpg");

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Texture_Default"), CTexture::Create(m_pGraphicDevice,&TextureDesc))))
		return E_FAIL;



	return S_OK;
}

HRESULT CMainApp::Ready_Static_GameObject_Prototype()
{	//Camera_Main ������Ÿ�� ����
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.vEye = _float3(0.f, 7.f, -5.f);
	CameraDesc.vAt = _float3(0, 0, 0);
	CameraDesc.vAxisY = _float3(0, 1, 0);

	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fMovePerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Camera_Main"), CCamera_Main::Create(m_pGraphicDevice, &CameraDesc))))
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
	Safe_Release(m_pComRenderer);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pGameInstance);
}

