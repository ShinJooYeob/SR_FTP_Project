#pragma once
#include "Component.h"

BEGIN(Engine)
class CGameObject;
END


BEGIN(Client)

// #TODO 뷰포트 충돌 / 구 충돌로 처리

class CCom_CollisionViewPort final: public CComponent
{
public:
	typedef struct tag_CollisionView_Sphere
	{
		tag_CollisionView_Sphere()
		{ mRadius = 0; mCenterRadius = _float2(0, 0); }

		_float2 mCenterRadius; // 중점
		_float mRadius; // 범위
	}COLLVIEW_SPHERE;

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
	
public:
	HRESULT AddCollisionView(COLLISION_VIEW_TYPE type, CGameObject* object);

	// 월드 -> view 좌표

	// 충돌 체크
	void Update_ViewPortCollision();

private:
	list<CGameObject*>				m_List_CollisionViewObjects[COLL_END];
	typedef list<CGameObject*>		COLLISIONOBJECTS;


public:
	static CCom_CollisionViewPort* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;

};
END
