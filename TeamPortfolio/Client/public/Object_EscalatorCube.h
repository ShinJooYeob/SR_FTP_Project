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

class CObject_EscalatorCube final : public CGameObject
{
public:
	typedef struct tagEscalatorCubeDesc{
		_float3 EscalatorCubePos;
		_float3 DestPos;
	}ESCALATORCUBEDESC;
private:
	explicit CObject_EscalatorCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CObject_EscalatorCube(const CObject_EscalatorCube& rhs);
	virtual ~CObject_EscalatorCube() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fTimeDelta)override;
	virtual _int Collision_Escalator(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime);

private:
	HRESULT SetUp_Components();

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	CTransform*				m_ComTransform = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;
	CVIBuffer_Cube*			m_ComVIBuffer = nullptr;

	CCollision*				m_pCollisionCom = nullptr;

	ESCALATORCUBEDESC		m_EscalatorCubeDesc;
	_float					m_fTimer = 0.f;
	_float					Seconds = 0.f;
	_float					Distance = 0.f;
	_float3					m_fTempPos;
	_bool					m_bCollisionSwitch = false;
	_float3					EscalatorPos;


public:
	static CObject_EscalatorCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END