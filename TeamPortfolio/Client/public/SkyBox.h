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
class CCamera_Main;

class CSkyBox final : public CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox() = default;
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

	CTransform*				m_ComTransform = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;
	CVIBuffer_Cube*			m_ComVIBuffer = nullptr;

	CCamera_Main*			m_MainCamera = nullptr;

public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device , void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END