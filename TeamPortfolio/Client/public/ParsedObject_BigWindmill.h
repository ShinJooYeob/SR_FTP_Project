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

class CParsedObject_BigWindmill final : public CParsedObject
{
public:
	typedef struct tagBigWindmill {
		_float3 Transform;
		_float	Angle;
		_float3 Axis;
	}BIGWINDMILLDESC;
private:
	explicit CParsedObject_BigWindmill(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParsedObject_BigWindmill(const CParsedObject_BigWindmill& rhs);
	virtual ~CParsedObject_BigWindmill() = default;


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
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	BIGWINDMILLDESC			m_BigWindmill;
	CTransform*				m_ComTransform = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;
	CVIBuffer_Parsed*		m_ComVIBuffer = nullptr;

	CCollision*				m_pCollisionCom = nullptr;



public:
	static CParsedObject_BigWindmill* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END