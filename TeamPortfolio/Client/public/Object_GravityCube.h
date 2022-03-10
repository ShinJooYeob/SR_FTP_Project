#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollision;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CObject_GravityCube final : public CGameObject
{
private:
	explicit CObject_GravityCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CObject_GravityCube(const CObject_GravityCube& rhs);
	virtual ~CObject_GravityCube() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fTimeDelta)override;
	virtual _int Collision_Gravity(_float fDeltaTime);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(_float fDeltaTime);

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	void	Set_Particle_BlackDust();
	void	Set_Particle_DustSmall();

private:
	CTransform*				m_ComTransform = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;
	CVIBuffer_Cube*			m_ComVIBuffer = nullptr;

	CCollision*				m_pCollisionCom = nullptr;

	_float					m_fFrame = 0;

	_float					ParticleTime = 0;
public:
	static CObject_GravityCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END