#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollision : public CComponent
{
public:
	enum COLLISIONGROUP { COLLISION_FLEXIBLE, COLLISION_FIX, COLLISION_END };
private:
	CCollision(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCollision() = default;

public:
	virtual HRESULT Initialize_Prototype(void * pArg) override;
	virtual HRESULT Initialize_Clone(void * pArg) override;


	HRESULT Collision_Obsever(_float fDeltaTime);
	HRESULT Collision_Obsever_Release();


public:
	HRESULT Add_CollisionGroup(COLLISIONGROUP eCollisionGroup, class CGameObject* pCollisionObject);
	


private:
	list<class CGameObject*>		m_CollisionObjects[COLLISION_END];

	//VTXTEX*	m_CCollision_SourceVertices;
	//VTXTEX*	m_CCollision_DestVertices;

public:
	static CCollision* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END