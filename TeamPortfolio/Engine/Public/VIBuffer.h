#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract :public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() =default;

public:
	virtual HRESULT Render();
protected:
	virtual HRESULT Initialize_Prototype(void * pArg);
	virtual HRESULT Initialize_Clone(void * pArg);


public:
	//버텍스 갯수 반환
	_uint Get_NumVertices() { return m_iNumVertices; }

	//버텍스 정보 반환
	void* Get_Vtxtex();
	void* Get_CubeVtxtex();

//	_bool Pick(const _Matrix & WorldMatrixInverse, _float3 * pOut, _float2* pOutUV);
	_bool Pick(const _Matrix& WorldMatrixInverse, _float3 * pOut, _float2* pOutUV, _float3* ppout);

protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	_uint							m_iStride = 0; 
	_uint							m_iNumVertices = 0;
	_ulong							m_dwFVF = 0;
	void*							m_pVertices = nullptr;


	_D3DPRIMITIVETYPE				m_eDrawPrimitive;
	_uint							m_iNumPrimitive = 0;


	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
	_uint							m_iIndicesSize = 0;
	D3DFORMAT						m_eIndexFormat;
	void*							m_pIndices = nullptr;

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	virtual CComponent* Clone(void* pArg)PURE;
	virtual void Free() override;
};

END	