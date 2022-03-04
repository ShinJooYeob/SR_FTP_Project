#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CCollision;
class CVIBuffer;
class CRenderer;
END


BEGIN(Client)
class CCamera_Main;

class CNpc : public CGameObject
{
private:
explicit CNpc(LPDIRECT3DDEVICE9	pGraphicDevice);
explicit CNpc(const CNpc& rhs);
virtual	~CNpc() = default;

private:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;
	virtual HRESULT ReInitialize(void* pArg)override;
	
public:
private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComRectVIBuffer = nullptr;
	CVIBuffer_Cube*			m_ComCubeVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CInventory*				m_ComInventory = nullptr;


	/* 충돌처리 */
	CCollision*		m_pCollisionCom = nullptr;
	CCamera_Main*		m_pCamera_Main = nullptr;
private:
	_bool					m_bTextureReverse = false;
	_bool					m_bIsShdow = false;

	_bool					m_bMove = false;
	_float					m_fMoveTime = 0.f;
	_float					m_fStartMoveCount = 0.f;

	_float					m_fFrame;
private:
	

	HRESULT	SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	
	HRESULT Move(_float DeltaTime);
public:
	static CNpc* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg=nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free()override;
};
END
