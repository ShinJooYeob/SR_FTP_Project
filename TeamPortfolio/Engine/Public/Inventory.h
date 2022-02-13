#pragma once

#include "Component.h"
static int a;
BEGIN(Engine)

class ENGINE_DLL CInventory final :public CComponent
{
public:
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual ~CInventory() = default;

private:
	virtual HRESULT Initialize_Protoype(void * pArg)override;
	virtual HRESULT Initialize_Clone(void * pArg)override;
public:
	_int Get_Gold() { return m_iGold; }
	_int Get_Skill_Level(_int eSKILL);

	void Set_Gold(_int Gold) { m_iGold = Gold; }
	void Set_Skill_Level(_int eSKILL, _int Skill_level);

	void Initialize_Skill_Array(_int pSkill_Index);


private:
	_int m_iGold;
	_int m_MaxSkillLevel;
	_int* m_pSkill_Index;
public:
	static CInventory* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

	
};

END