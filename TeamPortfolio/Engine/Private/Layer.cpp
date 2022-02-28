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
		if (pGameObject->Get_IsDied())
			continue;

		if (pGameObject->Update(fDeltaTime) < 0) 
			return -1;
	}

	return _int();
}

_int CLayer::LateUpdate(_float fDeltaTime)
{
	for (auto& pGameObject : m_ObjectList) {

		if (pGameObject->Get_IsDied())
			continue;

		if (pGameObject->LateUpdate(fDeltaTime) < 0)
			return -1;

	}

	// 죽은 오브젝트 실제 삭제
	Release_DeadObejct();
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

HRESULT CLayer::Delete_GameObject_By_LayerIndex(_uint iIndex)
{
	// 특정 오브젝트 삭제 / 인덱스번호
	if (iIndex >= m_ObjectList.size())
		return E_FAIL;

	int index = 0;
	for (auto iter = m_ObjectList.begin(); iter!=m_ObjectList.end();iter++)
	{
		if (index == iIndex)
		{
			(*iter)->DIED();
			return S_OK;
		}
		index++;
	}

	return E_FAIL;
}

HRESULT CLayer::Delete_GameObject_By_LayerObject(CGameObject * obj)
{
	// 특정 오브젝트 삭제 / obj 포인터
	for (auto iter = m_ObjectList.begin(); iter != m_ObjectList.end(); iter++)
	{
		if (*iter == obj)
		{
			(*iter)->DIED();
			return S_OK;
		}
	}
	return S_OK;
}

HRESULT CLayer::Set_NowSceneNum(_uint eScene)
{
	for (auto& pGameObeject : m_ObjectList)
		pGameObeject->Set_NowSceneNum(eScene);
	
	return S_OK;
}

HRESULT CLayer::Release_DeadObejct()
{
	// 죽은 오브젝트 리스트 삭제
	OBJECTS::iterator iter_begin = m_ObjectList.begin();
	OBJECTS::iterator iter_end = m_ObjectList.end();

	for (OBJECTS::iterator iter = iter_begin; iter != m_ObjectList.end();)
	{
		if ((*iter)->Get_IsDied())
		{
			Safe_Release(*iter);
			iter = m_ObjectList.erase(iter);
		}
		else
			iter++;
	}

	return S_OK;
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
		//pGameObject->Release();
		Safe_Release(pGameObject);
	}
	m_ObjectList.clear();

}
