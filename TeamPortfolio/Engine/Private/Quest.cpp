#include "..\Public\Quest.h"

IMPLEMENT_SINGLETON(CQuest)

CQuest::CQuest()
{
}

HRESULT CQuest::Initialize_Quest(_int eQuest_END)
{
	
	m_iQuestIndex = new _int[eQuest_END];
	m_iQuestGoalIndex = new _int[eQuest_END];
	/*ZeroMemory(m_iQuestIndex, sizeof m_iQuestIndex[eQuest_END]);
	ZeroMemory(m_iQuestGoalIndex, sizeof m_iQuestGoalIndex[eQuest_END]);*/
	for (int i = 0; i < eQuest_END; ++i)
	{
		m_iQuestIndex[i] = 0;
		m_iQuestGoalIndex[i] = 1;
		
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
		return S_OK;
	if (iCount > m_iQuestGoalIndex[eQuest])
		return S_OK;
	m_iQuestIndex[eQuest] += iCount;

	return S_OK;
}



_int CQuest::Get_QuestNeedPercent(_int eQuest)
{
	if (eQuest > m_iMaxQuest)
		return -1;

	else if (m_iQuestIndex[eQuest] >= m_iQuestGoalIndex[eQuest])
		return 100;

	else if (m_iQuestIndex[eQuest] < m_iQuestGoalIndex[eQuest])
	{
		_float fCountNeedPercent;
		fCountNeedPercent =  (((_float)m_iQuestIndex[eQuest]/ m_iQuestGoalIndex[eQuest]*100.f));
	
		_int iCountNeedPercent = (_int)fCountNeedPercent;
		return iCountNeedPercent;
	}
	else
		return -1;
}

_bool CQuest::Get_QuestComplete(void)
{
	int Quest[4] = {};
	for (int i = 0; i < m_iMaxQuest; ++i)
	{
		if (m_iQuestGoalIndex[i] <= m_iQuestIndex[i])
			Quest[i] = 1;
		else
			Quest[i] = 0;
	}
	m_iQuestComplete = Quest[0] + Quest[1] + Quest[2] + Quest[3];
	if (m_iQuestComplete == 4)
		return true;
	else
		return false;
}


void CQuest::Free()
{
	Safe_Delete_Array(m_iQuestIndex);
	Safe_Delete_Array(m_iQuestGoalIndex);
}

