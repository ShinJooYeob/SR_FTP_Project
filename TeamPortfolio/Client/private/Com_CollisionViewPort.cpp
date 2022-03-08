#include "stdafx.h"
#include "..\public\Com_CollisionViewPort.h"
#include "GameObject.h"

CCom_CollisionViewPort::CCom_CollisionViewPort(LPDIRECT3DDEVICE9 pGraphicDevice)
 : CComponent(pGraphicDevice)
{
}

CCom_CollisionViewPort::CCom_CollisionViewPort(const CCom_CollisionViewPort & rhs)
	: CComponent(rhs)
{
	

}

HRESULT CCom_CollisionViewPort::Initialize_Prototype(void * pArg)
{
	return S_OK;
}

HRESULT CCom_CollisionViewPort::Initialize_Clone(void * pArg)
{

	return S_OK;
}

HRESULT CCom_CollisionViewPort::AddCollisionView(COLLISION_VIEW_TYPE type, CGameObject * object)
{
	if (type >= COLL_END ||
		object == nullptr)
		return E_FAIL;


	m_List_CollisionViewObjects[type].push_back(object);
	// object->Update_ViewPos();
	Safe_AddRef(object);
	return S_OK;
}

void CCom_CollisionViewPort::Update_ViewPortCollision()
{

	// if (a, b, )
	// {
	// 	a.ViewPortHit(b);
	// }

	// 1. 뷰포트 좌표로 변경

	// 2. 뷰포트 좌표에서 충돌검사

	// 3. 충돌시 함수 실행
}

CCom_CollisionViewPort * CCom_CollisionViewPort::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CCom_CollisionViewPort* pInstance = new CCom_CollisionViewPort(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Create CCom_CollisionViewPort Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCom_CollisionViewPort::Clone(void * pArg)
{
	
	return this;
}

void CCom_CollisionViewPort::Free()
{
	for (_uint i = 0; i < COLL_END; ++i)
	{
		for (auto& RenderObject : m_List_CollisionViewObjects[i])
		{
			Safe_Release(RenderObject);
		}
		m_List_CollisionViewObjects[i].clear();
	}
}
