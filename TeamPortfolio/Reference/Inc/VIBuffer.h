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
	virtual HRESULT Initialize_Protoype(void * pArg);
	virtual HRESULT Initialize_Clone(void * pArg);


protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
	//커스텀 버텍스 포멧의 크기
	_uint							m_iStride = 0; 
	//정점의 갯수
	_uint							m_iNumVertices = 0;
	//버텍스 포멧
	_ulong							m_dwFVF = 0;

	//그리기 설정 (삼각형 3개씩 꺼내서 그리기)
	_D3DPRIMITIVETYPE				m_eDrawPrimitive;
	//삼각형의 갯수
	_uint							m_iNumPrimitive = 0;


	//삼각형을 그리는 인덱스 구조체의 사이즈
	_uint							m_iIndicesSize = 0;
	//인덱스 하나의 byte
	D3DFORMAT						m_eIndexFormat;

	void*							m_pVertices = nullptr;

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	virtual CComponent* Clone(void* pArg)PURE;
	virtual void Free() override;
};

END	