#include "..\Public\SceneMgr.h"
#include "Scene.h"
#include "GameInstance.h"



IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	:m_pScene(nullptr), m_iCurrentSceneIdx(0), m_iNowSceneIndx_ExceptLoad(0)
{
}

HRESULT CSceneMgr::Scene_Chage(CScene * _pScene , _int iNextSceneIndex, _bool bIsNotLoadingScene )
{
	if (_pScene == nullptr)
		return E_FAIL;


	if (m_iCurrentSceneIdx && FAILED(GetSingle(CGameInstance)->Clear_Scene_Resource(m_iCurrentSceneIdx)))
		return E_FAIL;
	


	Safe_Release(m_pScene);
	m_pScene = _pScene;
	m_iCurrentSceneIdx = iNextSceneIndex;
	if (bIsNotLoadingScene)
		m_iNowSceneIndx_ExceptLoad = m_iCurrentSceneIdx;

	return S_OK;
}

_int CSceneMgr::Update(_float fDeltaTime)
{
	if (m_pScene == nullptr)
		return -1;
	return m_pScene->Update(fDeltaTime);
}

_int CSceneMgr::LateUpdate(_float fDeltaTime)
{
	if (m_pScene == nullptr)
		return -1;
	return m_pScene->LateUpdate(fDeltaTime);
}

_int CSceneMgr::Render()
{
	if (m_pScene == nullptr)
		return -1;
	return m_pScene->Render();
}

_int CSceneMgr::LateRender()
{
	if (m_pScene == nullptr)
		return -1;
	return m_pScene->LateRender();
}

CScene * CSceneMgr::Get_Scene()
{
	return m_pScene;
}


void CSceneMgr::Free()
{
	Safe_Release(m_pScene);
}
