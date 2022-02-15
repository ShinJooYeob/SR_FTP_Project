#pragma once

#include "GameObject.h"

BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CPlayer final :public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)override;

private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CVIBuffer_Cube*			m_ComColiisionBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CInventory*				m_ComInventory = nullptr;


private:
	_float			m_fFrame = 0.f;

	_bool					m_bIsJumped = false;
	_float					m_fJumpPower = 5.f;
	_float					m_fNowJumpPower = 0.f;
	_bool					m_bIsCliming = false;

	CGameObject*	m_FootHoldObject = nullptr;
	CGameObject*	m_BackWardObject = nullptr;


private:
	HRESULT			SetUp_Components();

	HRESULT			Find_FootHold_Object();

	HRESULT			Set_PosOnFootHoldObject(_float fDeltaTime);
	HRESULT			Set_PosOnTerrain(_float fDeltaTime);


	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

