#pragma once

#include "Base.h"

BEGIN(Client)


class CParticleMgr :public CBase
{
	DECLARE_SINGLETON(CParticleMgr);

private:
	explicit CParticleMgr();
	virtual ~CParticleMgr() = default;


public:
	HRESULT Initialize_ParticleMgr(LPDIRECT3DDEVICE9 pGraphicDevice);

public:
	HRESULT Create_ParticleObject(_uint eSceneID, PARTICLEDESC tParticleDesc);
	HRESULT Create_ParticleObject_AddSub(_uint eSceneID, PARTICLEDESC tParticleDesc, PARTICLEDESC tSubParticleDesc);

private:
	LPDIRECT3DDEVICE9 m_pGraphicDevice = nullptr;
	
public:
	virtual void Free() override;
};

END	