
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


	HRESULT Rot_N_SceneChange(_float fDeltaTime);
	void Strat_Turning(_uint RotDir);

private:
	HRESULT SetUp_Components();

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	HRESULT Revolution_Turn_AxisY_CW(_float fDeltaTime);

private:
	CVIBuffer_Cube*		m_ComVIBuffer = nullptr;
	CTransform*			m_ComTransform = nullptr;
	CRenderer*			m_ComRenderer = nullptr;
	CTexture*			m_ComTexture = nullptr;


	_float						m_fPassedTime = 0;
	_float						m_fStartAngle = 0;
	_float						m_fTargetAngle = 0;
	_float						m_fTotalTime = 1;
	_uint						m_eEasingType = 0;
	_bool						m_IsTurning = false;

public:
	static CLobyCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};



END