
#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollision;
END

BEGIN(Client)

class CCollision_Object final : public CGameObject
{


private:
	explicit CCollision_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollision_Object(const CCollision_Object& rhs);
	virtual ~CCollision_Object() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)override;

private:
	HRESULT SetUp_Components();

private:
	CTransform*			m_ComTransform = nullptr;
	CCollision*			m_pCollisionCom = nullptr;

	CTransform*			m_PlayerTransform = nullptr;

public:
	static CCollision_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};



END