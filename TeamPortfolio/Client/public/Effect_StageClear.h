
#pragma once

#include "GameObject.h"


BEGIN(Client)

class CEffect_StageClear final : public CGameObject
{
private:
	typedef struct tagStarObjectAttribute
	{
		_float3 vPos;
		_float fAge = 0;
		_float LifeTime;
		_float fDegreeAngle;
		_float	fDist;
		_bool bIsDead;

		_float3	vPlayerPos;
		_float3 vTargetPos;

	}STAROBJATT;

private:
	explicit CEffect_StageClear(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEffect_StageClear(const CEffect_StageClear& rhs);
	virtual ~CEffect_StageClear() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)override;


private:

	CVIBuffer_Rect*		m_ComVIBuffer = nullptr;
	CTransform*			m_ComTransform = nullptr;
	CRenderer*			m_ComRenderer = nullptr;
	CTexture*			m_ComTexture = nullptr;


	

private:
	_uint				m_iAcheiveStarNum = 0;
	_uint					m_iChecker = 0;
	list<STAROBJATT>				m_StarObjectList;

	PARTICLEDESC			m_ParticleDesc;
private:
	HRESULT SetUp_Components();

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();



	_float3			Caculate_TargetPos(_float3 vPlayerPos, _float fDegree, _float fDistance);
	_float3			EaseingFloat3(EasingTypeID id, _float3 StartPos, _float3 EndPos, _float curTime, _float maxTime);

public:
	static CEffect_StageClear* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};



END