#include "stdafx.h"
#include "..\Public\Scene_Loading.h"

#include "Loader.h"
#include "Scene_Loby.h"
#include "Scene_StageSelect.h"
#include "Scene_Stage1.h"
#include "Scene_Stage2.h"
#include "Scene_Stage3.h"
#include "Scene_IMGUI.h"
#include "Scene_Tutorial.h"
#include "Scene_Stage_Boss.h"
#include "UI_LoadingBar.h"
#include "Camera_Main.h"





CScene_Loading::CScene_Loading(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
}

HRESULT CScene_Loading::Initialize(SCENEID eSceneID)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	GetSingle(CGameInstance)->Stop_AllChannel();

	m_eNextSceneIndex = eSceneID;
	m_pLoader = CLoader::Create(m_pGraphicDevice,eSceneID);

	if (FAILED(Ready_Layer_Loading(TEXT("Layer_Loading"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_LoadingBar(TEXT("Layer_LoadingBar"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main))))
		return E_FAIL;*/


	return S_OK;
}

_int CScene_Loading::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	m_pLoadingBar->Set_Progress((_float)(m_pLoader->Get_ProgressCount()), (_float)(m_pLoader->Get_MaxCount()));

	return 0;
}

_int CScene_Loading::LateUpdate(_float fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;


	//로딩이 끝낫을 경우
	if ( m_pLoader->IsLoadFinished())
	{
		GetSingle(CGameInstance)->Set_iNowSceneIndx_ExceptLoad(m_eNextSceneIndex);
		switch (m_eNextSceneIndex)
		{
		case SCENEID::SCENE_LOBY:
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loby::Create(m_pGraphicDevice), m_eNextSceneIndex ,true )))
				return E_FAIL;
			break;

		case SCENEID::SCENE_STAGESELECT:
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_StageSelect::Create(m_pGraphicDevice), m_eNextSceneIndex, true)))
				return E_FAIL;
			break;

		case SCENEID::SCENE_STAGE1:
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Stage1::Create(m_pGraphicDevice), m_eNextSceneIndex, true)))
				return E_FAIL;
			break;

		case SCENEID::SCENE_STAGE2:
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Stage2::Create(m_pGraphicDevice), m_eNextSceneIndex, true)))
				return E_FAIL;
			break;

		case SCENEID::SCENE_STAGE3:
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Stage3::Create(m_pGraphicDevice), m_eNextSceneIndex, true)))
				return E_FAIL;
			break;

		case SCENEID::SCENE_IMGUISCENE:
			FAILED_CHECK(GetSingle(CGameInstance)->Scene_Change(CScene_IMGUI::Create(m_pGraphicDevice), m_eNextSceneIndex,true));			
			break;
		case SCENEID::SCENE_TUTORIAL:
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Tutorial::Create(m_pGraphicDevice), m_eNextSceneIndex, true)))
				return E_FAIL;
			break;
		case SCENEID::SCENE_BOSS:
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Stage_Boss::Create(m_pGraphicDevice), m_eNextSceneIndex, true)))
				return E_FAIL;
			break;
		default:
			break;
		}


	}

	return 0;
}

_int CScene_Loading::Render()
{
	if (__super::Render() < 0)
		return -1;


//#ifdef _DEBUG
//
//	if (m_pLoader != nullptr) 
//	{
//		_int iPercent = int((m_pLoader->Get_ProgressCount() / (float)m_pLoader->Get_MaxCount()) * 100);
//		_tchar  szTempString[MAX_PATH];
//		_itow_s(iPercent, szTempString, MAX_PATH , 10);
//		lstrcat(szTempString,L"% 로딩 중 (Loading Scene)");
//
//		SetWindowText(g_hWnd, szTempString);
//	}
//#endif // _DEBUG


	return 0;
}

_int CScene_Loading::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

HRESULT CScene_Loading::Ready_Layer_Loading(const _tchar * pLayerTag)
{
	//오브젝트들을 써야함
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_LOADING, pLayerTag, TEXT("Prototype_GameObject_Loading")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Loading::Ready_Layer_LoadingBar(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_LOADING, pLayerTag, TEXT("Prototype_GameObject_LoadingBar")))
		return E_FAIL;

	m_pLoadingBar = (CUI_LoadingBar*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENEID::SCENE_LOADING, pLayerTag));

	return S_OK;
}

HRESULT CScene_Loading::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.bIsOrtho = true;
	CameraDesc.vWorldRotAxis = _float3(5.f, 3.f, 5.f);
	CameraDesc.vAxisY = _float3(0, 1, 0);
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fMovePerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, pLayerTag, TAG_OP(Prototype_Camera_Main), &CameraDesc))
		return E_FAIL;
	return S_OK;
}

CScene_Loading * CScene_Loading::Create(LPDIRECT3DDEVICE9 GraphicDevice, SCENEID eSceneID)
{
	CScene_Loading* pTempLoadingScene = new CScene_Loading(GraphicDevice);

	if (FAILED(pTempLoadingScene->Initialize((eSceneID))))
	{
		Safe_Release(pTempLoadingScene);
		MSGBOX("Failed to Creating CScene_Loading");
		return nullptr;
	}


	return pTempLoadingScene;
}

void CScene_Loading::Free()
{
	__super::Free();
	
	Safe_Release(m_pLoader);
}
