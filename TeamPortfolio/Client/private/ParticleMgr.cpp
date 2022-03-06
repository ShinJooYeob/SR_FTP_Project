#include "stdafx.h"
#include "..\public\ParticleMgr.h"


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

HRESULT CParticleMgr::Create_ParticleObject(SCENEID eSceneID, PARTICLEDESC tParticleDesc)
{

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


