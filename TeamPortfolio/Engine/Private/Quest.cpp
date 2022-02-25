#include "..\Public\Quest.h"

IMPLEMENT_SINGLETON(CQuest)

CQuest::CQuest()
{
}

HRESULT CQuest::Initialize_Quest(_int eQuest_END)
{
	m_iQuestIndex = new _int[eQuest_END];
	m_iQuestGoalIndex = new _int[eQuest_END];
	for (int i = 0; i < eQuest_END; ++i)
	{
		m_iQuestIndex[i] = 0;
		
	}
	m_iMaxQuest = eQuest_END;


	return S_OK;
}

HRESULT CQuest::Set_QuestGoal(_int eQuest, _int iQuestGoal)
{
	if (iQuestGoal < 1)
	{
		MSGBOX("iQuestGoal은 1이상만 가능");
		return E_FAIL;
	}
	
	m_iQuestGoalIndex[eQuest] = iQuestGoal;
	return S_OK;
}

HRESULT CQuest::Set_QuestIndexIncrease(_int eQuest, _int iCount)
{
	if (eQuest > m_iMaxQuest)
		return E_FAIL;
	m_iQuestIndex[eQuest] += iCount;

	return S_OK;
}



_int CQuest::Get_QuestNeed(_int eQuest)
{
	if (eQuest > m_iMaxQuest)
		return -1;

	else if (m_iQuestIndex[eQuest] >= m_iQuestGoalIndex[eQuest])
		return 0;

	else if (m_iQuestIndex[eQuest] < m_iQuestGoalIndex[eQuest])
	{
		_int iCountNeed;
		iCountNeed = m_iQuestGoalIndex[eQuest] - m_iQuestIndex[eQuest];
		return iCountNeed;
	}

	return -1;
}


void CQuest::Free()
{
	Safe_Delete_Array(m_iQuestIndex);
	Safe_Delete_Array(m_iQuestGoalIndex);
}

