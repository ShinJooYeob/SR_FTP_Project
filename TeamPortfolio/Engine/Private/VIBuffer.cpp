#include "..\Public\VIBuffer.h"
#include "Picking.h"



CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice), m_pVertices(nullptr), m_pIndices(nullptr)
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
	m_pVertices(rhs.m_pVertices),
	m_pIndices(rhs.m_pIndices)

{

	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


		
	return S_OK;
}

HRESULT CVIBuffer::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

void * CVIBuffer::Get_Vtxtex()
{
	return (VTXTEX*)m_pVertices;
}

void * CVIBuffer::Get_CubeVtxtex()
{
	return (VTXCUBETEX*)m_pVertices;
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
	// 1 드로우 타입 2: 버텍스 버퍼의 시작위치 3:인덱스 버퍼의 시작 위치*/


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

_bool CVIBuffer::Pick(const _Matrix& WorldMatrixInverse, _float3 * pOut,_float2* pOutUV,_float3* ppout)
{
	CPicking*		pPicking = GET_INSTANCE(CPicking);

	pPicking->Transform_ToLocalSpace(WorldMatrixInverse);

	_uint		iIndexByte = 0;

	if (m_eIndexFormat == D3DFMT_INDEX16)
		iIndexByte = 2;
	else
		iIndexByte = 4;

	_bool		isPick = false;

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		_uint	iIndices[3] = { 0 };

		/* void*를 _byte*로 캐스팅한 이유 : 주소에 상수를 더했을때 그 상수바이트크기만큼 이동할 수 있도록 만들어주기위해. */
		/* _short*포인터형변수에 1을더하면 2byte씩 이동한다.  */
		/* _uint*포인터형변수에 1을더하면 4byte씩 이동한다.  */
		/* _byte*포인터형변수에 1을더하면 1byte씩 이동한다.  */

		for (_uint j = 0; j < 3; ++j)
			memcpy(&iIndices[j], (((_byte*)m_pIndices + m_iIndicesSize * i) + iIndexByte * j), iIndexByte);

		_float3		vPoint[3] = {
			*(_float3*)(((_byte*)m_pVertices) + m_iStride * iIndices[0]),
			*(_float3*)(((_byte*)m_pVertices) + m_iStride * iIndices[1]),
			*(_float3*)(((_byte*)m_pVertices) + m_iStride * iIndices[2])
		};

		if (isPick = pPicking->isPick_UV(vPoint, pOut, pOutUV))
		{
			_Matrix		WorldMatrix;
			D3DXMatrixInverse(&WorldMatrix, nullptr, &WorldMatrixInverse);
			D3DXVec3TransformCoord(pOut, pOut, &WorldMatrix);
			ppout[0] = vPoint[0];
			ppout[1] = vPoint[1];
			ppout[2] = vPoint[2];

			break;
		}
	}

	RELEASE_INSTANCE(CPicking);

	return isPick;
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
	{
		Safe_Delete_Array(m_pVertices);
		Safe_Delete_Array(m_pIndices);
	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

}
