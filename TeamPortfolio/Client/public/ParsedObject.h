#pragma once

#include "GameObject.h"

BEGIN(Client)

class CParsedObject : public CGameObject
{
protected:
	explicit CParsedObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParsedObject(const CParsedObject& rhs);
	virtual ~CParsedObject() = default;


public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fTimeDelta)override;



public:
	static CParsedObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END