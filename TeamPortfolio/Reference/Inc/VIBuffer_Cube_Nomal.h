#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cube_Normal final : public CVIBuffer
{
protected:
	explicit CVIBuffer_Cube_Normal(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer_Cube_Normal(const CVIBuffer_Cube_Normal& rhs);
	virtual ~CVIBuffer_Cube_Normal() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	static CVIBuffer_Cube_Normal* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END