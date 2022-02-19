#pragma once

#include "Component.h"

BEGIN(Engine)

class CTransform;

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

public:
	HRESULT Collision_Pushed(CTransform* pMyTansform, _float3 fCollision_Distance, _float fDeltaTime);
	HRESULT Collision_Suck_In(CTransform* pMyTansform, _float3 fCollision_Distance, _float fDeltaTime);
	


private:
	list<class CGameObject*>		m_CollisionObjects[COLLISION_END];

	_float3 DefaultCubeVerties[8] = {};

	//VTXCUBETEX*	m_CCollision_SourceVertices;
	//VTXCUBETEX*	m_CCollision_DestVertices;

public:
	static CCollision* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END