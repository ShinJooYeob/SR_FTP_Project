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

class CParsedObject_Alien final : public CParsedObject
{
private:
	explicit CParsedObject_Alien(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParsedObject_Alien(const CParsedObject_Alien& rhs);
	virtual ~CParsedObject_Alien() = default;


public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fTimeDelta)override;


private:
	HRESULT SetUp_Components();
	HRESULT	SetUp_ParticleDesc();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	PARTICLEDESC			m_ParticleDesc;



	CTransform*				m_ComTransform = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;
	CVIBuffer_Parsed*		m_ComVIBuffer = nullptr;

	CCollision*				m_pCollisionCom = nullptr;



	_float3					m_DefaultPos;
	_float					m_ChangingY = 0;
	_float					m_fFrameTime = 0;
	_float					m_fAngle = 0;

	_float					m_fStartAngle = 0;
	_float					m_fTargetAngle = 0;

	_bool					m_bIsDiag = false;


public:
	static CParsedObject_Alien* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END