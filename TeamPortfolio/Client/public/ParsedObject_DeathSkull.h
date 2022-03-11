#pragma once

#include "GameObject.h"
#include "ParsedObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollision;
class CVIBuffer_Parsed;
END

BEGIN(Client)

class CParsedObject_DeathSkull final : public CParsedObject
{
private:
	explicit CParsedObject_DeathSkull(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParsedObject_DeathSkull(const CParsedObject_DeathSkull& rhs);
	virtual ~CParsedObject_DeathSkull() = default;


public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;

	virtual _int Render()override;
	virtual _int LateRender()override;
	��

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	CTransform*				m_ComTransform = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;
	CVIBuffer_Parsed*		m_ComVIBuffer = nullptr;

	CCollision*				m_pCollisionCom = nullptr;



public:
	static CParsedObject_DeathSkull* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END