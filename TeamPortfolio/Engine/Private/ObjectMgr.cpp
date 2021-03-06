#include "..\Public\ObjectMgr.h"
#include "GameObject.h"
#include "Layer.h"



IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{
}

HRESULT CObjectMgr::Reserve_Container(_uint _eSceneNum)
{
	m_iMaxSceneNum = _eSceneNum;

	m_mapLayer = new LAYERS[m_iMaxSceneNum];

	return S_OK;
}

HRESULT CObjectMgr::Add_GameObject_Prototype(const _tchar * tagPrototype, CGameObject * pPrototype)
{
	if (pPrototype == nullptr)
		return E_FAIL;

	if (Find_Prototype(tagPrototype) != nullptr)
	{
		OutputDebugString(L"Prototypes already exist,So won't add more And did Release Parameter GameObject");
		Safe_Release(pPrototype);
		return S_FALSE;
	}
	else {

		m_mapPrototypes.emplace(tagPrototype, pPrototype);
	}


	return S_OK;
}

HRESULT CObjectMgr::Add_GameObject_To_Layer(_uint eSceneNum, const _tchar * tagLayer, const _tchar * tagPrototype, void * pArg)
{
	if (eSceneNum >= m_iMaxSceneNum)
		return E_FAIL;

	CGameObject* pPrototype = Find_Prototype(tagPrototype);

	if (pPrototype == nullptr)
		return E_FAIL;

	CGameObject* pInstance =  pPrototype->Clone(pArg);

	if (pInstance == nullptr)
		return E_FAIL;

	pInstance->Set_NowSceneNum(eSceneNum);
	if (pInstance->Get_Layer_Tag() == nullptr)
		pInstance->Set_Layer_Tag(tagLayer);

	CLayer* pLayer = Find_Layer(eSceneNum,tagLayer);

	if (pLayer == nullptr)
	{
		pLayer = CLayer::Create();

		if (pLayer == nullptr)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(pInstance)))
			return E_FAIL;


		m_mapLayer[eSceneNum].emplace(tagLayer, pLayer);
	}
	else 
	{
		if (FAILED(pLayer->Add_GameObject(pInstance)))
			return E_FAIL;

		m_mapLayer[eSceneNum].emplace(tagLayer, pLayer);
	}



	return S_OK;
}

HRESULT CObjectMgr::Delete_GameObject_To_Layer_Index(_uint eSceneNum, const _tchar * tagLayer, int index)
{
	if (eSceneNum >= m_iMaxSceneNum || m_mapLayer == nullptr)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(eSceneNum, tagLayer);

	if (pLayer == nullptr)
		return E_FAIL;
	
	FAILED_CHECK(pLayer->Delete_GameObject_By_LayerIndex(index));

	return S_OK;
}


HRESULT CObjectMgr::Delete_GameObject_To_Layer_Object(_uint eSceneNum, const _tchar * tagLayer, CGameObject* obj)
{
	if (eSceneNum >= m_iMaxSceneNum || m_mapLayer == nullptr)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(eSceneNum, tagLayer);

	if (pLayer == nullptr)
		return E_FAIL;

	FAILED_CHECK(pLayer->Delete_GameObject_By_LayerObject(obj));


	return S_OK;
}

HRESULT CObjectMgr::Clear_Scene_GameObjects(_uint eSceneNum)
{

	if (eSceneNum >= m_iMaxSceneNum)
		return E_FAIL;

	for (auto& pair : m_mapLayer[eSceneNum])
		Safe_Release(pair.second);


	m_mapLayer[eSceneNum].clear();

	
	return S_OK;
}

HRESULT CObjectMgr::Set_StaticGameObject_NowSceneNum(_uint eSceneNum)
{
	for (auto& pair : m_mapLayer[0])
		pair.second->Set_NowSceneNum(eSceneNum);

	return S_OK;
}

CComponent * CObjectMgr::Get_Commponent_By_LayerIndex(_uint eSceneNum, const _tchar * tagLayer, const _tchar* tagComponet,  _uint iLayerIndex)
{
	if (eSceneNum >= m_iMaxSceneNum || m_mapLayer == nullptr)
		return nullptr;

	CLayer* pLayer = Find_Layer(eSceneNum, tagLayer);

	if (pLayer == nullptr)
		return nullptr;

	return pLayer->Get_Commponent_By_LayerIndex(tagComponet, iLayerIndex);
}

CGameObject * CObjectMgr::Get_GameObject_By_LayerIndex(_uint eSceneNum, const _tchar * tagLayer, _uint iLayerIndex)
{
	if (eSceneNum >= m_iMaxSceneNum || m_mapLayer == nullptr)
		return nullptr;

	CLayer* pLayer = Find_Layer(eSceneNum, tagLayer);

	if (pLayer == nullptr)
		return nullptr;

	return pLayer->Get_GameObject_By_LayerIndex(iLayerIndex);
}

CGameObject * CObjectMgr::Change_Camera_Ortho_By_LayerIndex(_uint eSceneNum, const _tchar * tagLayer, _uint iLayerIndex)
{
	if (eSceneNum >= m_iMaxSceneNum || m_mapLayer == nullptr)
		return nullptr;

	CLayer* pLayer = Find_Layer(eSceneNum, tagLayer);

	if (pLayer == nullptr)
		return nullptr;
	CGameObject * pTemp = pLayer->Get_GameObject_By_LayerIndex(iLayerIndex);;
	

	return nullptr;
}


list<CGameObject*>* CObjectMgr::Get_ObjectList_from_Layer(_uint iSceneNum, const _tchar * tagLayer)
{
	if (iSceneNum >= m_iMaxSceneNum || m_mapLayer == nullptr)
		return nullptr;
	CLayer* layer = Find_Layer(iSceneNum, tagLayer);
	if (!layer)
		return nullptr;
	else
		return layer->Get_ObjectList();
}

_int CObjectMgr::Update(_float fDeltaTime)
{
	for (_uint eSceneNym = 0 ; eSceneNym < m_iMaxSceneNum; eSceneNym++)
	{

		for (auto& pair : m_mapLayer[eSceneNym]) 
		{

			if (pair.second->Update(fDeltaTime) < 0)
				return -1;

		}
	}


	return 0;
}

_int CObjectMgr::LateUpdate(_float fDeltaTime)
{
	for (_uint eSceneNym = 0; eSceneNym < m_iMaxSceneNum; eSceneNym++)
	{
		for (auto& pair : m_mapLayer[eSceneNym])
		{
			if (pair.second->LateUpdate(fDeltaTime) < 0)
				return -1;

		}
	}
	return 0;
}

CGameObject * CObjectMgr::Find_Prototype(const _tchar * tagPrototype)
{
	auto iter = find_if(m_mapPrototypes.begin(), m_mapPrototypes.end(), CTagFinder(tagPrototype));

	if (iter == m_mapPrototypes.end())
		return nullptr;

	return iter->second;
}

CLayer * CObjectMgr::Find_Layer(_uint iSceneNum, const _tchar * tagLayer)
{

	auto iter = find_if(m_mapLayer[iSceneNum].begin(), m_mapLayer[iSceneNum].end(), CTagFinder(tagLayer));

	if (iter == m_mapLayer[iSceneNum].end())
		return nullptr;

	return iter->second;
}



void CObjectMgr::Free()
{

	for (_uint iSceneNum = 0; iSceneNum < m_iMaxSceneNum; ++iSceneNum)
	{
		for (auto& pair : m_mapLayer[iSceneNum])
			Safe_Release(pair.second);


		m_mapLayer[iSceneNum].clear();
	}

	Safe_Delete_Array(m_mapLayer);

	for (auto& pair : m_mapPrototypes) 
	{
		Safe_Release(pair.second);
	}
	m_mapPrototypes.clear();


}
