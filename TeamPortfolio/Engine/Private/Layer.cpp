#include "..\Public\Layer.h"

#include "GameObject.h"


CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	m_ObjectList.push_back(pGameObject);
	return S_OK;
}

_int CLayer::Update(_float fDeltaTime)
{

	for (auto& pGameObject : m_ObjectList) 
	{

		if (pGameObject->Update(fDeltaTime) < 0) 
			return -1;

	}

	return _int();
}

_int CLayer::LateUpdate(_float fDeltaTime)
{
	for (auto& pGameObject : m_ObjectList) {

		if (pGameObject->LateUpdate(fDeltaTime) < 0)
			return -1;

	}
	return _int();
}

CComponent * CLayer::Get_Commponent_By_LayerIndex(const _tchar* tagComponet, _uint iLayerIndex)
{
	if (iLayerIndex >= m_ObjectList.size())
		return nullptr;

	auto iter = m_ObjectList.begin();

	for (_uint i = 0; i<iLayerIndex;i++)
		iter++;

	
	return (*iter)->Find_Components(tagComponet);
}

CGameObject * CLayer::Get_GameObject_By_LayerIndex(_uint iLayerIndex)
{
	if (iLayerIndex >= m_ObjectList.size())
		return nullptr;

	auto iter = m_ObjectList.begin();

	for (_uint i = 0; i < iLayerIndex; i++)
		iter++;

	return (*iter);
}



CLayer * CLayer::Create()
{
	CLayer* pLayer = new CLayer();
	return pLayer;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_ObjectList)
	{
		Safe_Release(pGameObject);
	}
	m_ObjectList.clear();

}
