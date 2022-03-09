
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

class CTerrainCube final : public CGameObject
{
private:
	explicit CTerrainCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrainCube(const CTerrainCube& rhs);
	virtual ~CTerrainCube() = default;
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

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:

	CVIBuffer_Cube*		m_ComVIBuffer = nullptr;
	CTransform*			m_ComTransform = nullptr;
	CRenderer*			m_ComRenderer = nullptr;
	CTexture*			m_ComTexture = nullptr;
	CShader*			m_ComShader = nullptr;

	/* 충돌처리 */
	CCollision*		m_pCollisionCom = nullptr;
	CVIBuffer_Cube*		m_ComColiisionBuffer = nullptr;

	CTransform*			m_PlayerTransform = nullptr;
	_bool				m_bIsArticle = false;


public:
	static CTerrainCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};



END