#pragma once
#include "Component.h"


BEGIN(Engine)
class CGameObject;

// 뷰포트 충돌 / 구 충돌로 처리

class ENGINE_DLL CCom_CollisionViewPort final: public CComponent
{
public:
	enum COLLISION_VIEW_TYPE
	{
		COLL_PLAYER, COLL_MONSTER, COLL_BULLET, COLL_END
	};


private:
	explicit CCom_CollisionViewPort(LPDIRECT3DDEVICE9	pGraphicDevice);
	explicit CCom_CollisionViewPort(const CCom_CollisionViewPort& rhs);
	virtual	~CCom_CollisionViewPort() = default;

public:
	HRESULT Initialize_Prototype(void * pArg);
	HRESULT Initialize_Clone(void * pArg);
	
private:
	_bool isCollisionSphere(const MYSPHERE& a, const MYSPHERE& b);

public:
	HRESULT AddCollisionView(COLLISION_VIEW_TYPE type, CGameObject* object);
	_bool isScreenOutPos(_float2 Pos, _float offset);

	// 월드 -> view 좌표
	_float2 WorldToView(_float3 pos);
	// _float3 ViewToWorldTo(_float2 pos);

	// 충돌 체크
	void Update_ViewPortCollision();
	void Release_ViewPortCollision();

private:
	D3DVIEWPORT9	mViewPortDesc;

	list<CGameObject*>				m_List_CollisionViewObjects[COLL_END];
	typedef list<CGameObject*>		COLLISIONOBJECTS;


public:
	static CCom_CollisionViewPort* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;

};
END
