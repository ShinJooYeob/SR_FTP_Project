#include "..\Public\Inventory.h"



CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

HRESULT CInventory::Initialize_Protoype(void * pArg)
{
	return S_OK;
}

HRESULT CInventory::Initialize_Clone(void * pArg)
{
	return S_OK;
}

_int CInventory::Get_Skill_Level(_int eSKILL)
{
	return m_pSkill_Index[eSKILL];
}

void CInventory::Set_Skill_Level(_int eSKILL, _int Skill_level)
{
	m_pSkill_Index[eSKILL] += Skill_level;
}

void CInventory::Initialize_Skill_Array(_int pSkill_Index)
{
	m_pSkill_Index = new _int[pSkill_Index];
	m_MaxSkillLevel = pSkill_Index;
}

CInventory * CInventory::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CInventory*	pInstance = new CInventory(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Protoype(pArg)))
	{
		MSGBOX("Failed to Creating CInventory");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CInventory::Clone(void * pArg)
{
	return nullptr;
}

void CInventory::Free()
{
}
