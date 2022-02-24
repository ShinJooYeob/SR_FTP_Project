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

class CObject_ButtonCube final : public CGameObject
{
private:
	explicit CObject_ButtonCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CObject_ButtonCube(const CObject_ButtonCube& rhs);
	virtual ~CObject_ButtonCube() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)override;

	_int	Collision_Interactive(_float fDeltaTime);

private:
	HRESULT SetUp_Components();

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	CTransform*				m_ComTransform = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;

	CCollision*				m_pCollisionCom = nullptr;


	_float					m_RotAngle = 0;
	_float					seconds = 0;

	_float					Angle = 0;

	bool					m_bSwitch = false;

	_int					m_RotaionCount = 0;

public:
	static CObject_ButtonCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END