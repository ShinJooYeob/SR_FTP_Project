#pragma once

#include "Component.h"


BEGIN(Engine)

class ENGINE_DLL CInventory final :public CComponent
{
public:
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CInventory(const CInventory & rhs);
	virtual ~CInventory() = default;

private:
	virtual HRESULT Initialize_Prototype(void * pArg)override;
	virtual HRESULT Initialize_Clone(void * pArg)override;
public:
	_int Get_Gold() { return m_iGold; }
	_int Get_Skill_Level(_int eSKILL);
	_int Get_MaxLevel(_int eSKILL);
	_int* Get_Skill_Index() const { return m_pSkill_Index; }
	
	void Set_Gold(_int Gold) { m_iGold += Gold; }
	void Set_Skill_LevelUP(_int eSKILL);
	void Set_Skill_MaxLevel(_int eSKILL, _int MaxLevel);


	void Initialize_Skill_Array(_int eSKILL_END);


private:
	_int m_iGold=0;
	_int Skill_Level=0;
	_int m_iMaxSkill_Index=0;
	_int* m_pSKill_LevelMax=nullptr;
	_int* m_pSkill_Index = nullptr;
public:
	static CInventory* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

	
};

END