
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

class CLobyCube final : public CGameObject
{
private:
	explicit CLobyCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CLobyCube(const CLobyCube& rhs);
	virtual ~CLobyCube() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	virtual _int Render()override;
	virtual _int LateRender()override;



private:
	HRESULT SetUp_Components();

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:

	CVIBuffer_Cube*		m_ComVIBuffer = nullptr;
	CTransform*			m_ComTransform = nullptr;
	CRenderer*			m_ComRenderer = nullptr;
	CTexture*			m_ComTexture = nullptr;



public:
	static CLobyCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};



END