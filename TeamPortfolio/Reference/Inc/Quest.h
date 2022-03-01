#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CQuest final: public CBase
{
	DECLARE_SINGLETON(CQuest)
private:
	explicit CQuest();
	virtual ~CQuest() = default;

public:
	HRESULT Initialize_Quest(_int eQuest_END);
	
	HRESULT Set_QuestGoal(_int eQuest, _int iQuestGoal);
	HRESULT Set_QuestIndexIncrease(_int eQuest, _int iCount);

	_int	Get_QuestGoal(_int eQuest) { return m_iQuestGoalIndex[eQuest]; }
	_int	Get_QuestCurrent(_int eQuest) { return m_iQuestIndex[eQuest]; }
	_int	Get_QuestNeedPercent (_int eQuest);
private:
	_int*		m_iQuestGoalIndex=nullptr;

	_int*		m_iQuestIndex=nullptr;
	_int		m_iMaxQuest;
private:
	virtual void	Free() override;
};

END