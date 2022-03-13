#include "stdafx.h"
#include "Loby_UI.h"
#include "..\Public\Scene_Loby.h"
#include "Scene_Loading.h"

#include "BackGround.h"
#include "Camera_Main.h"



CScene_Loby::CScene_Loby(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
}





HRESULT CScene_Loby::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;


	if (FAILED(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main))))
		return E_FAIL;
	
	FAILED_CHECK(Ready_Layer_SkyBox(TEXT("Layer_LobySkyBox")));

	FAILED_CHECK(Ready_Layer_LobyCube(TEXT("Layer_LobyCube")));
	FAILED_CHECK(Ready_Layer_LobyPlayer(TEXT("Layer_LobyPlayer")));
	FAILED_CHECK(Ready_Layer_LobyScroll(TEXT("Layer_LobyScroll")));
	FAILED_CHECK(Ready_Layer_LobyUI(TEXT("Layer_LobyUI")));


	FAILED_CHECK(GetSingle(CGameInstance)->PlayBGM((L"JW_LobyBGM.wav")));

	return S_OK;
}

_int CScene_Loby::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (!ChangeTime &&m_pUI->Get_IsSceneChange() )
	{
		ChangeTime += fDeltaTime;

	}

	if (ChangeTime)
	{
		ChangeTime += fDeltaTime;
		if (ChangeTime > 1.4f && m_pMainCam->Get_EffectID() == CCamera_Main::CAM_EFT_END)
		{
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGESELECT), SCENEID::SCENE_LOADING)))
				return E_FAIL;
		}
	}
	// IMGUI / ¸Ê Åø Å×½ºÆ®¾ÀÀ¸·Î »ç¿ë
	if (GetKeyState(VK_F1) & 0x8000)
	{
		FAILED_CHECK(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGESELECT), SCENEID::SCENE_LOADING));
	}
	if (GetKeyState(VK_F2) & 0x8000)
	{
		FAILED_CHECK(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE1), SCENEID::SCENE_LOADING));
	}

	if (GetKeyState(VK_F3) & 0x8000)
	{
		FAILED_CHECK(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE2), SCENEID::SCENE_LOADING));
	}

	if (GetKeyState(VK_F4) & 0x8000)
	{
		FAILED_CHECK(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE3), SCENEID::SCENE_LOADING));
	}

	if (GetKeyState(VK_F5) & 0x8000)
	{
		FAILED_CHECK(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_TUTORIAL), SCENEID::SCENE_LOADING));
	}
	if (GetKeyState(VK_F6) & 0x8000)
	{
		FAILED_CHECK(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_BOSS), SCENEID::SCENE_LOADING));
	}

	return 0;
}

_int CScene_Loby::LateUpdate(_float fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	return 0;
}

_int CScene_Loby::Render()
{
	if (__super::Render() < 0)
		return -1;

//#ifdef _DEBUG
//	SetWindowText(g_hWnd, TEXT("LobyScene"));
//#endif // _DEBUG
//
	return 0;
}

_int CScene_Loby::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}



HRESULT CScene_Loby::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_LOBY, pLayerTag, TEXT("Prototype_GameObject_LobySkyBox")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Loby::Ready_Layer_LobyCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_LOBY, pLayerTag, TEXT("Prototype_GameObject_LobyCube")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Loby::Ready_Layer_LobyPlayer(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_LOBY, pLayerTag, TEXT("Prototype_GameObject_LobyPlayer")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Loby::Ready_Layer_LobyScroll(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_LOBY, pLayerTag, TEXT("Prototype_GameObject_LobyScroll")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Loby::Ready_Layer_LobyUI(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_LOBY, pLayerTag, TEXT("Prototype_GameObject_LobyUI")))
		return E_FAIL;
	m_pUI = (CLoby_UI*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENEID::SCENE_LOBY, pLayerTag));

	if (m_pUI == nullptr)
		return E_FAIL;


	return S_OK;
}


HRESULT CScene_Loby::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.bIsOrtho = true;
	CameraDesc.vEye = _float3(0.f, 0.f, -10.f);
	CameraDesc.vWorldRotAxis = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0, 1, 0);
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fMovePerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	m_pMainCam = (CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));

	if (m_pMainCam == nullptr)
	{
		if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, pLayerTag, TAG_OP(Prototype_Camera_Main), &CameraDesc))
			return E_FAIL;

		m_pMainCam = (CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
		if (m_pMainCam == nullptr)
			return E_FAIL;
	}
	else 
	{

		if (FAILED(m_pMainCam->Reset_LookAtAxis(&CameraDesc)))
			return E_FAIL;


		m_pMainCam->Set_NowSceneNum(SCENE_LOBY);


	}
	
	return S_OK;
}

CScene_Loby * CScene_Loby::Create(LPDIRECT3DDEVICE9 GraphicDevice)
{
	CScene_Loby* pTempLoby = new CScene_Loby(GraphicDevice);

	if (FAILED(pTempLoby->Initialize())) 
	{
		Safe_Release(pTempLoby); 
		MSGBOX("Failed to Creating CScene_Loby");
		return nullptr;
	}

	return pTempLoby;

}

void CScene_Loby::Free()
{
	__super::Free();
}
