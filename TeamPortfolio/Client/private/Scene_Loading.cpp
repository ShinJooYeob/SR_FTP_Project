#include "stdafx.h"
#include "..\Public\Scene_Loading.h"

#include "Loader.h"
#include "Scene_Loby.h"
#include "Scene_StageSelect.h"





CScene_Loading::CScene_Loading(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
}

HRESULT CScene_Loading::Initialize(SCENEID eSceneID)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eNextSceneIndex = eSceneID;
	m_pLoader = CLoader::Create(m_pGraphicDevice,eSceneID);


	return S_OK;
}

_int CScene_Loading::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;


	return 0;
}

_int CScene_Loading::LateUpdate(_float fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;


	//로딩이 끝낫을 경우
	if ( m_pLoader->IsLoadFinished())
	{
		switch (m_eNextSceneIndex)
		{
		case SCENEID::SCENE_LOBY:
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loby::Create(m_pGraphicDevice), m_eNextSceneIndex)))
				return E_FAIL;
			break;

		case SCENEID::SCENE_STAGESELECT:
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_StageSelect::Create(m_pGraphicDevice), m_eNextSceneIndex)))
				return E_FAIL;
			break;

		case SCENEID::SCENE_STAGE1:
				return E_FAIL;
			break;

		case SCENEID::SCENE_STAGE2:
				return E_FAIL;
			break;

		case SCENEID::SCENE_STAGE3:
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


#ifdef _DEBUG

	if (m_pLoader != nullptr) 
	{
		_int iPercent = int((m_pLoader->Get_ProgressCount() / (float)m_pLoader->Get_MaxCount()) * 100);
		_tchar  szTempString[MAX_PATH];
		_itow_s(iPercent, szTempString, MAX_PATH , 10);
		lstrcat(szTempString,L"% 로딩 중 (Loading Scene)");

		SetWindowText(g_hWnd, szTempString);
	}
#endif // _DEBUG


	return 0;
}

_int CScene_Loading::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
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
