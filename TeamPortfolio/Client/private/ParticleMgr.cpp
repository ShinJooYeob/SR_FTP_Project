#include "stdafx.h"
#include "..\public\ParticleMgr.h"
#include "ParticleObejct.h"


IMPLEMENT_SINGLETON(CParticleMgr);

CParticleMgr::CParticleMgr()
{
}

HRESULT CParticleMgr::Initialize_ParticleMgr(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	if (pGraphicDevice == nullptr)
		return E_FAIL;

	m_pGraphicDevice = pGraphicDevice;
	Safe_AddRef(m_pGraphicDevice);

	return S_OK;
}

HRESULT CParticleMgr::Create_ParticleObject(_uint eSceneID, PARTICLEDESC tParticleDesc)
{
	tParticleDesc.bSubPraticle = false;
	switch (tParticleDesc.eParticleID)
	{
	case Client::Particle_Straight:

		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Straight"), &tParticleDesc));

		break;
	case Client::Particle_Ball:
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Ball"), &tParticleDesc));

		break;
	case Client::Particle_Cone:
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Cone"), &tParticleDesc));

		break;
	case Client::Particle_Fountain:
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Fountain"), &tParticleDesc));

		break;

	case Client::Particle_Spread:
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Spread"), &tParticleDesc));

		break;

	case Client::Particle_Fixed:
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Fixed"), &tParticleDesc));

		break;
	case Client::Particle_Suck:
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Suck"), &tParticleDesc));

		break;

	default:
		return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CParticleMgr::Create_ParticleObject_AddSub(_uint eSceneID, PARTICLEDESC tParticleDesc, PARTICLEDESC tSubParticleDesc)
{
	list<CGameObject*>* ParticleList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(eSceneID, L"Layer_Particle");
	CParticleObject* ParticleObj = nullptr;

	tParticleDesc.bSubPraticle = true;
	tSubParticleDesc.bSubPraticle = false;

	switch (tParticleDesc.eParticleID)
	{
	case Client::Particle_Straight:

		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Straight"), &tParticleDesc));

		ParticleObj = (CParticleObject*)(ParticleList->back());
		ParticleObj->Set_SubParticleDesc(&tSubParticleDesc);

		break;
	case Client::Particle_Ball:
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Ball"), &tParticleDesc));

		ParticleObj = (CParticleObject*)(ParticleList->back());
		ParticleObj->Set_SubParticleDesc(&tSubParticleDesc);
		break;
	case Client::Particle_Cone:
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Cone"), &tParticleDesc));

		ParticleObj = (CParticleObject*)(ParticleList->back());
		ParticleObj->Set_SubParticleDesc(&tSubParticleDesc);
		break;
	case Client::Particle_Fountain:
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Fountain"), &tParticleDesc));

		ParticleObj = (CParticleObject*)(ParticleList->back());
		ParticleObj->Set_SubParticleDesc(&tSubParticleDesc);
		break;

	case Client::Particle_Spread:
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Spread"), &tParticleDesc));

		ParticleObj = (CParticleObject*)(ParticleList->back());
		ParticleObj->Set_SubParticleDesc(&tSubParticleDesc);
		break;

	case Client::Particle_Fixed:
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Fixed"), &tParticleDesc));

		ParticleObj = (CParticleObject*)(ParticleList->back());
		ParticleObj->Set_SubParticleDesc(&tSubParticleDesc);
		break;

	case Client::Particle_Suck:

		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(eSceneID, TEXT("Layer_Particle"), TEXT("ProtoType_GameObject_Object_particle_Suck"), &tParticleDesc));

		ParticleObj = (CParticleObject*)(ParticleList->back());
		ParticleObj->Set_SubParticleDesc(&tSubParticleDesc);

		break;

	default:
		return E_FAIL;
		break;
	}




	return S_OK;
}

void CParticleMgr::Free()
{
	Safe_Release(m_pGraphicDevice);
}


