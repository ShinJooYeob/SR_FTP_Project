#pragma once

#include "Base.h"


BEGIN(Engine)
class ENGINE_DLL CComponent abstract :public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Protoype(void * pArg);
	virtual HRESULT Initialize_Clone(void * pArg);


protected:
	LPDIRECT3DDEVICE9 m_pGraphicDevice = nullptr;
	_bool				m_bIsClone = false;



public:
	virtual CComponent* Clone(void* pArg = nullptr)PURE;
	virtual void  Free() override;
};

END	