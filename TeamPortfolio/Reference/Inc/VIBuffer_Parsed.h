#pragma once
#include "VIBuffer.h"


BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Parsed :public CVIBuffer
{
public:
	typedef struct tagParsedBufferDesc {

		list<_float3>*	vPos;
		list<_float2>*	vUV;
		list<_uint>*	vIndex;

	}PASRSEDDESC;

public:
	HRESULT Fix_Vertex_By_Postion(_float3 vPos);

private:
	explicit CVIBuffer_Parsed(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CVIBuffer_Parsed(const CVIBuffer_Parsed& rhs);
	virtual ~CVIBuffer_Parsed() = default;

private:
	virtual HRESULT Initialize_Prototype(void * pArg);
	virtual HRESULT Initialize_Clone(void * pArg);


public:
	static CVIBuffer_Parsed* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;
};

END