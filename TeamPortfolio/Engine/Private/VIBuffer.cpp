#include "..\Public\VIBuffer.h"




CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice), m_pVertices(nullptr)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	:CComponent(rhs),
	m_pVB(rhs.m_pVB),
	m_pIB(rhs.m_pIB),
	m_iStride(rhs.m_iStride),
	m_iNumVertices(rhs.m_iNumVertices),
	m_dwFVF(rhs.m_dwFVF),
	m_eDrawPrimitive(rhs.m_eDrawPrimitive),
	m_iNumPrimitive(rhs.m_iNumPrimitive),
	m_iIndicesSize(rhs.m_iIndicesSize),
	m_eIndexFormat(rhs.m_eIndexFormat),
	m_pVertices(rhs.m_pVertices)

{

	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_Protoype(void * pArg)
{
	if (FAILED(__super::Initialize_Protoype(pArg)))
		return E_FAIL;


		
	return S_OK;
}

HRESULT CVIBuffer::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Render()
{

	if (m_pGraphicDevice == nullptr)
		return E_FAIL;
	//몇번째 스트림이냐, 어떤 버퍼를 바인딩할까, 몇번째 바이트부터 쓸까, 한번에 크기를 얼마나 빼서쓸까
	m_pGraphicDevice->SetStreamSource(0, m_pVB, 0, m_iStride);
	m_pGraphicDevice->SetIndices(m_pIB);
	m_pGraphicDevice->SetFVF(m_dwFVF);

	// m_pGraphic_Device->DrawPrimitive(m_ePrimitiveType, 0, m_iNumPrimitive);
	m_pGraphicDevice->DrawIndexedPrimitive(m_eDrawPrimitive,0,0,m_iNumVertices, 0, m_iNumPrimitive);
	//?/*2: 버텍스 버퍼의 시작위치 3:인덱스 버퍼의 시작 위치*/


	return S_OK;
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;



	if (FAILED(m_pGraphicDevice->CreateVertexBuffer(m_iStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;
	

	/*
	CreateVertexBuffer 호출하면
	사용할 커스텀 버텍스 버퍼의 크기만큼 메모리를 할당하고
	해당 메모리주소 및 관리를 하는 컴포넌트에 접근할 수 있는 포인터는 LPDIRECT3DVERTEXBUFFER9에 저장한다.

	2인자
	0 => 정적 버텍스 배열로 만들겠다 (읽기만 가능)
	D3DUSAGE_DYNAMIC => 동적 배열로 만들겠다 (읽기 쓰기 둘다 가능) ->속도는 살짝 느리지만 수정이 가능하다

	4인자
	버텍스 버퍼를 할당할 위치가 어디냐
	D3DPOOL_DEFAULT 는 비디오 메모리 상에 리소스를 배치
	D3DPOOL_MANAGED 는 비디오 메모리의 리소스를 시스템메모리에 백업
	D3DPOOL_SYSTEMMEM 는 시스템 메모리에 배치
	*/


	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	if (FAILED(m_pGraphicDevice->CreateIndexBuffer(m_iIndicesSize * m_iNumPrimitive, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	/*
	1인자 인덱스 버퍼의 총 사이즈
	3인자 인덱스 하나의 크기
	5인자 인덱스 버퍼를 담을 공간
	*/

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	if (!m_bIsClone)
		Safe_Delete_Array(m_pVertices);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

}
