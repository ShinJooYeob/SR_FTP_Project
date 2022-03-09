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

_bool CCom_CollisionViewPort::isCollisionSphere(const MYSPHERE & a, const MYSPHERE & b)
{
	// 충돌 체크

	return true;
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
	// 좌표 업데이트
	for (int i = 0; i < COLL_END-1; i++)
	{
		for (auto& obj1 : m_List_CollisionViewObjects[i])
		{
			for (auto& obj2 : m_List_CollisionViewObjects[i + 1])
			{
				if (isCollisionSphere(obj1->GetSphere(), obj2->GetSphere()))
				{
					obj1->ViewPortHit(obj2);
					obj2->ViewPortHit(obj1);
				}
			}
		}
	}
	

	// MYSPHERE s;

	// 1. 뷰포트 좌표로 변경

	// 2. 뷰포트 좌표에서 충돌검사

	// 3. 충돌시 함수 실행
}

void CCom_CollisionViewPort::Release_ViewPortCollision()
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
