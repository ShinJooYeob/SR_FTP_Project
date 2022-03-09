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
class CObject_PortalCube_B final : public CGameObject
{
private:
	explicit CObject_PortalCube_B(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CObject_PortalCube_B(const CObject_PortalCube_B& rhs);
	virtual ~CObject_PortalCube_B() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	HRESULT Set_TargetCube(class CObject_PortalCube_A* pCube) { 	m_pTargetCube = pCube;		return S_OK; }

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

	_float					m_Seconds = 0.f;

	_float					m_fTimeDelta;
	class CPlayer*			m_pPlayer = nullptr;

	class CObject_PortalCube_A*		m_pTargetCube = nullptr;
	
public:
	static CObject_PortalCube_B* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END