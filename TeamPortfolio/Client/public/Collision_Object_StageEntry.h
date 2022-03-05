
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

class CCollision_Object_StageEntry final : public CGameObject
{
public:
	typedef struct tagStgeEntryDesc {

		_float3 vPos;
		SCENEID eTargetScene;

	}STAGEENTERYDESC;

private:
	explicit CCollision_Object_StageEntry(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollision_Object_StageEntry(const CCollision_Object_StageEntry& rhs);
	virtual ~CCollision_Object_StageEntry() = default;
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

private:
	CTransform*			m_ComTransform = nullptr;
	CCollision*			m_pCollisionCom = nullptr;

	CTransform*			m_PlayerTransform = nullptr;



	_bool				m_bIsStageChage = false;
	_float				m_fChangeSceneTime = 0;

	STAGEENTERYDESC		m_tStageDesc;

public:
	static CCollision_Object_StageEntry* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};



END