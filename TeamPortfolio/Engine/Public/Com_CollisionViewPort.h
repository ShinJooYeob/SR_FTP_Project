#pragma once
#include "Component.h"


BEGIN(Engine)
class CGameObject;

// #TODO ����Ʈ �浹 / �� �浹�� ó��

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

	// ���� -> view ��ǥ

	// �浹 üũ
	void Update_ViewPortCollision();
	void Release_ViewPortCollision();

private:
	list<CGameObject*>				m_List_CollisionViewObjects[COLL_END];
	typedef list<CGameObject*>		COLLISIONOBJECTS;


public:
	static CCom_CollisionViewPort* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;

};
END
