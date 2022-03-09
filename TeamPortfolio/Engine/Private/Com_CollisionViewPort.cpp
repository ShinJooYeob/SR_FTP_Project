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


	_float xx = b.mCenterPosition.x - a.mCenterPosition.x;
	_float yy = b.mCenterPosition.y - a.mCenterPosition.y;

	_float distance = xx * xx + yy * yy;

	_float Radius = a.mRadius + b.mRadius;
	Radius *= Radius;


	return distance < Radius;
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

_float2 CCom_CollisionViewPort::WorldToView(_float3 worldPos)
{
	// 월드 -> 뷰 -> 투영 -> 뷰포트
	_Matrix ViewMatrix;
	_Matrix ProjMatrix;
	_float3 WorldPos = worldPos;

	m_pGraphicDevice->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDevice->GetTransform(D3DTS_PROJECTION, &ProjMatrix);
	D3DXVec3TransformCoord(&WorldPos, &WorldPos, &ViewMatrix);
	D3DXVec3TransformCoord(&WorldPos, &WorldPos, &ProjMatrix);
	
	D3DVIEWPORT9	ViewPortDesc;
	m_pGraphicDevice->GetViewport(&ViewPortDesc);
	
	// -1 ~ 1 = 0 ~ 1280
	WorldPos.x = (WorldPos.x + 1) * (ViewPortDesc.Width  * 0.5f);
	WorldPos.y = (1- WorldPos.y ) * (ViewPortDesc.Height * 0.5f);

	_float2 newPos = _float2(WorldPos.x, WorldPos.y);
	return newPos;
}

void CCom_CollisionViewPort::Update_ViewPortCollision()
{
	// 좌표 업데이트
	for (int i = 0; i < COLL_END - 1; i++)
	{
		for (int j = 1; j < COLL_END; j++)
		{
			if (i == j) continue;

			for (auto& obj1 : m_List_CollisionViewObjects[i])
			{
				for (auto& obj2 : m_List_CollisionViewObjects[j])
				{
					if (isCollisionSphere(obj1->GetSphere(), obj2->GetSphere()))
					{
						obj1->ViewPortHit(obj2);
						obj2->ViewPortHit(obj1);
					}
				}
			}
		}

	}
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
