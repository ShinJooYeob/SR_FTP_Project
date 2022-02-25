#include "..\Public\Inventory.h"



CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{
	
}
CInventory::CInventory(const CInventory & rhs)
	: CComponent(rhs)
{
	m_iGold = 0;
}

HRESULT CInventory::Initialize_Prototype(void * pArg)
{
	return S_OK;
}

HRESULT CInventory::Initialize_Clone(void * pArg)
{
	Initialize_Skill_Array(*(_int*)pArg);
	return S_OK;
}

_int CInventory::Get_Skill_Level(_int eSKILL)
{
	if (eSKILL > m_iMaxSkill_Index)
	{
		MSGBOX("index段引");
		return -1;
	}
	return m_pSkill_Index[eSKILL];
}




void CInventory::Set_Skill_LevelUP(_int eSKILL)
{
	if (eSKILL > m_iMaxSkill_Index)
	{
		MSGBOX("index段引");
		return;
	}
	
	if (m_pSkill_Index[eSKILL] > 1)
	{
		MSGBOX("Max level 段引");
		return;
	}
	++m_pSkill_Index[eSKILL];
}





void CInventory::Initialize_Skill_Array(_int eSkill_Index)
{
	m_pSkill_Index = new _int[eSkill_Index];
	for (int i = 0;i < eSkill_Index;++i)
		m_pSkill_Index[i] = 0;

	m_iMaxSkill_Index = eSkill_Index;
}

CInventory * CInventory::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CInventory*	pInstance = new CInventory(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Creating CInventory");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CInventory::Clone(void * pArg)
{
	CInventory* pInstance = new CInventory((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Create CInventory");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CInventory::Free()
{
	__super::Free();
	if (m_bIsClone)
		Safe_Delete_Array(m_pSkill_Index);
}
