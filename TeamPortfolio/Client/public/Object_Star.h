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


class CObject_Star final : public CGameObject
{
public:
	typedef struct tagObjectStarDesc {
		_float3 fTransform;
	}STARDESC;
private:
	explicit CObject_Star(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CObject_Star(const CObject_Star& rhs);
	virtual ~CObject_Star() = default;
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

	void	Set_Particle();

private:
	STARDESC				m_StarDesc;
	CTransform*				m_ComTransform = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;
	CVIBuffer_Rect*			m_ComRectVIBuffer = nullptr;
	CVIBuffer_Cube*			m_ComCubeVIBuffer = nullptr;
	CCollision*				m_pCollisionCom = nullptr;

	_float3					TempPos;


public:
	static CObject_Star* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


END