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
	void	Set_QuestCompleteIndex(_int eQuest, _int iCount) { m_iQuestCompleteIndex[eQuest] = iCount; }

	_int	Get_QuestGoal(_int eQuest) { return m_iQuestGoalIndex[eQuest]; }
	_int	Get_QuestCurrent(_int eQuest) { return m_iQuestIndex[eQuest]; }
	_int	Get_QuestNeedPercent(_int eQuest);
	_bool	Get_QuestComplete(void);
	_uint	Get_QuestComplete(_int eQuest){ return m_iQuestCompleteIndex[eQuest]; }
private:
	_uint		m_iQuestCompleteIndex[4] = {0,0,0,0};
	_int*		m_iQuestGoalIndex=nullptr;
	_int		m_iQuestComplete = 0;
	_int*		m_iQuestIndex=nullptr;
	_int		m_iMaxQuest;
private:
	virtual void	Free() override;
};

END