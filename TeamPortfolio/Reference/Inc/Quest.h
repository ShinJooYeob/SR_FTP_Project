#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CQuest final: public CComponent
{
private:
	explicit CQuest(LPDIRECT3DDEVICE9 pGraphicDevice );
	explicit CQuest(const CQuest &rhs);
	virtual ~CQuest() = default;
public:
	virtual HRESULT Initialize_Prototype(void * pArg)override;
	virtual HRESULT Initialize_Clone(void * pArg)override;
public:
	HRESULT Initialize_Quest(_int eQuest);
	bool	Get_QuestResult (_int eQuest);
private:
	_int*		m_pQuestIndex;
	_int		m_MaxQuestIndex;
private:
	static CQuest* Create(LPDIRECT3DDEVICE9 pGraphicDevice,void* pArg=nullptr);
	virtual void	Free() override;
	virtual CQuest* Clone(void* pArg) override;
};

END