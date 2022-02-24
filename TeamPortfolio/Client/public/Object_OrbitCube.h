#pragma once

#include "GameObject.h"


BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CCollision;
class CVIBuffer_Cube;
END

class CObject_OrbitCube final : public CGameObject
{
public:
	typedef struct tagOrbitCubeDesc{
		_float3 fTransform;
		_float3 fRotAxis;
	}ORBITCUBEDESC;
private:
	explicit CObject_OrbitCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CObject_OrbitCube(const CObject_OrbitCube& rhs);
	virtual ~CObject_OrbitCube() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)override;
	void	Set_ButtonCube(class CObject_OrbitButton* pButtonCube) {m_pButtonCube = pButtonCube	;}

private:
	HRESULT SetUp_Components();

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	ORBITCUBEDESC			m_OrbitCubeDesc;
	CTransform*				m_ComTransform = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;
	CVIBuffer_Cube*			m_ComVIBuffer = nullptr;

	CCollision*				m_pCollisionCom = nullptr;

	_Matrix					parentMatrix;

	_float					seconds = 0;
	_float					m_RotAngle = 0;



	_float				m_StartAngle;
	_float				m_TargetAngle;
	_float				m_PassedTime;
	_bool				m_bIsTurning = false;

	class CObject_OrbitButton* m_pButtonCube = nullptr;

public:
	static CObject_OrbitCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

