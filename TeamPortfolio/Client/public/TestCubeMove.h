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

class CTestCubeMove final : public CGameObject
{
private:
	explicit CTestCubeMove(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTestCubeMove(const CTestCubeMove& rhs);
	virtual ~CTestCubeMove() = default;
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
	HRESULT SetUp_OnTerrain(_float fDeltaTime);

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:

	/* �� */
	CVIBuffer_Cube*		m_ComVIBuffer = nullptr;

	/* ���� (��ġ, ũ��, ȸ��) */
	CTransform*			m_ComTransform = nullptr;

	/* �׷�����. */
	CRenderer*			m_ComRenderer = nullptr;

	/* �浹ó�� */
	CCollision*		m_pCollisionCom = nullptr;

	_bool					m_bIsJumped = false;
	_float					m_fJumpPower = 5.f;
	_float					m_fNowJumpPower = 0.f;


public:
	static CTestCubeMove* Create(LPDIRECT3DDEVICE9 pGraphic_Device , void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END