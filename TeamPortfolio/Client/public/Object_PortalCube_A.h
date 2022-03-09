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

class CObject_PortalCube_A final : public CGameObject
{
public:
	typedef struct tagPotalCubeDesc{
		_float3 vPos_A_Cube;
		_float3 vPos_B_Cube;
		_uint  iNowScene;
	}POTALDESC;

private:
	explicit CObject_PortalCube_A(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CObject_PortalCube_A(const CObject_PortalCube_A& rhs);
	virtual ~CObject_PortalCube_A() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fTimeDelta)override;

	virtual _int Object_Transfer(CGameObject * pDestObjects, _float fDeltaTime);

	void UsedPotal(_float fTimeDelta) { m_Seconds += fTimeDelta; }

	void	Set_Particle();

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

	class CPlayer*			m_pPlayer = nullptr;

	_float					m_Seconds = 0.f;
	_float					m_fTimeDelta;

	class CObject_PortalCube_B*		m_pTargetCube = nullptr;

public:
	static CObject_PortalCube_A* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END