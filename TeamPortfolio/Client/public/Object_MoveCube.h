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

class CObject_MoveCube final : public CGameObject
{
private:
	explicit CObject_MoveCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CObject_MoveCube(const CObject_MoveCube& rhs);
	virtual ~CObject_MoveCube() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)override;
	virtual _int Collision_Not_Moving(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime);

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_OnTerrain(_float fDeltaTime);

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:

	CTransform*				m_ComTransform = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;
	//CVIBuffer_Rect*			m_ComVIBuffer = nullptr;

	CVIBuffer_Cube*			m_ComColiisionBuffer = nullptr;
	CCollision*				m_pCollisionCom = nullptr;

	_bool					m_bIsJumped = false;
	_float					m_fJumpPower = 5.f;
	_float					m_fNowJumpPower = 0.f;


public:
	static CObject_MoveCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device , void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END