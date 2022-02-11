#include "..\Public\ComponentMgr.h"


IMPLEMENT_SINGLETON(CComponentMgr);

CComponentMgr::CComponentMgr()
{
}

HRESULT CComponentMgr::Reserve_Container(_uint eMaxSceneNum)
{

	m_iMaxScenenNum = eMaxSceneNum;

	m_mapComPrototype = new COMPROTOTYPES[m_iMaxScenenNum];

	return S_OK;
}

HRESULT CComponentMgr::Add_Component_Prototype(_uint eSceneIdx, const _tchar * tagComPrototype, CComponent * pComponenet)
{
	if (eSceneIdx >= m_iMaxScenenNum || pComponenet == nullptr)
		return E_FAIL;


	if (Find_Component(eSceneIdx, tagComPrototype) != nullptr)
		return E_FAIL;

	m_mapComPrototype[eSceneIdx].emplace(tagComPrototype, pComponenet);


	return S_OK;
}

CComponent * CComponentMgr::Clone_Component(_uint eSceneIdx, const _tchar * tagComPrototype, void * pArg)
{
	if (eSceneIdx >= m_iMaxScenenNum)
		return nullptr;


	CComponent* pPrototype = Find_Component(eSceneIdx, tagComPrototype);


	if (pPrototype == nullptr)
		return nullptr;

	CComponent* pCloneComponent = pPrototype->Clone(pArg);

	return pCloneComponent;
}

HRESULT CComponentMgr::Clear_Scene_Componenets(_uint eSceneIdx)
{

	if (eSceneIdx >= m_iMaxScenenNum)
		return E_FAIL;

	for (auto& pair : m_mapComPrototype[eSceneIdx]) 
		Safe_Release(pair.second);
	
	m_mapComPrototype[eSceneIdx].clear();

	return S_OK;
}

CComponent * CComponentMgr::Find_Component(_uint eScenenIdx,const _tchar * tagComPrototype)
{
	auto iter = find_if(m_mapComPrototype[eScenenIdx].begin(), m_mapComPrototype[eScenenIdx].end(), CTagFinder(tagComPrototype));
	
	if (iter == m_mapComPrototype[eScenenIdx].end())
		return nullptr;

	return iter->second;
}

void CComponentMgr::Free()
{
	for (_uint i = 0; i< m_iMaxScenenNum ;i++)
	{
		for (auto& pair : m_mapComPrototype[i])
			Safe_Release(pair.second);
		

		m_mapComPrototype[i].clear();
	}

	Safe_Delete_Array(m_mapComPrototype);
}
