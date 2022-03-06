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
	HRESULT Create_ParticleObject(SCENEID eSceneID, PARTICLEDESC tParticleDesc);

private:
	LPDIRECT3DDEVICE9 m_pGraphicDevice = nullptr;
	
public:
	virtual void Free() override;
};

END	