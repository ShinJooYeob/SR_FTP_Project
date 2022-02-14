#include "..\Public\Quest.h"





CQuest::CQuest(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

CQuest::CQuest(const CQuest & rhs)
	:CComponent(rhs)
{
}

HRESULT CQuest::Initialize_Protoype(void * pArg)
{
	return S_OK;
}

HRESULT CQuest::Initialize_Clone(void * pArg)
{
	if (FAILED(Initialize_Quest(*(_int*)pArg)))
		return E_FAIL;
	return S_OK;
}

HRESULT CQuest::Initialize_Quest(_int eQuest)
{
	m_pQuestIndex = new _int[eQuest];
	for (int i = 0; i < eQuest; ++i)
	{
		m_pQuestIndex[i] = 0;
	}
	m_MaxQuestIndex = eQuest;


	return S_OK;
}

bool CQuest::Get_QuestResult(_int eQuest)
{
	if (m_pQuestIndex[eQuest] = 1)
		return true;

	if (m_pQuestIndex[eQuest] = 0)
		return false;
}

CQuest * CQuest::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CQuest* pInstance = new CQuest(pGraphicDevice);
	if (FAILED(pInstance->Initialize_Protoype(pArg)))
	{
		MSGBOX("Failed to Create Quest Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CQuest::Free()
{
	__super::Free();
}

CQuest * CQuest::Clone(void * pArg)
{
	CQuest* pInstance = new CQuest(*this);
	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Create Quest");
		Safe_Release(pInstance);
	}

	return pInstance;
}
