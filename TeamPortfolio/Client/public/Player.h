#pragma once

#include "GameObject.h"

BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END

BEGIN(Client)

class CCamera_Main;

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
	CRenderer*				m_ComRenderer = nullptr;
	CInventory*				m_ComInventory = nullptr;


	/* 충돌처리 */
	CCollision*		m_pCollisionCom = nullptr;

private:
	_float					m_fFrame = 0.f;

	_float					m_fJumpPower = 5.f;
	_float					m_fNowJumpPower = 0.f;
	_uint					m_bIsJumped = 0;

	_bool					m_bIsCliming = false;
	_bool					m_bIsShdow = false;

	_bool					m_bTextureReverse = false;

	_bool					m_bIsRunning = false;

	_float3					m_vDownstairsNear;
	_float3					m_vClimingBlock;
	_float3					m_vReturnStair = _float3(0,0,0);


	CCamera_Main*			m_pCamera_Main = nullptr;

private:
	HRESULT			SetUp_Components();

	HRESULT			Input_Keyboard(_float fDeltaTime);
	HRESULT			Animation_Change(_float fDeltaTime);

	HRESULT			Find_FootHold_Object(_float fDeltaTime);
	HRESULT			Set_PosOnFootHoldObject(_float fDeltaTime);
	HRESULT			Set_CamY(_float fDeltaTime);


	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

END	