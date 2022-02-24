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
	//���° ��Ʈ���̳�, � ���۸� ���ε��ұ�, ���° ����Ʈ���� ����, �ѹ��� ũ�⸦ �󸶳� ��������
	m_pGraphicDevice->SetStreamSource(0, m_pVB, 0, m_iStride);
	m_pGraphicDevice->SetIndices(m_pIB);
	m_pGraphicDevice->SetFVF(m_dwFVF);

	// m_pGraphic_Device->DrawPrimitive(m_ePrimitiveType, 0, m_iNumPrimitive);
	m_pGraphicDevice->DrawIndexedPrimitive(m_eDrawPrimitive,0,0,m_iNumVertices, 0, m_iNumPrimitive);
	// 1 ��ο� Ÿ�� 2: ���ؽ� ������ ������ġ 3:�ε��� ������ ���� ��ġ*/


	return S_OK;
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;



	if (FAILED(m_pGraphicDevice->CreateVertexBuffer(m_iStride * m_iNumVertices, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;
	

	/*
	CreateVertexBuffer ȣ���ϸ�
	����� Ŀ���� ���ؽ� ������ ũ�⸸ŭ �޸𸮸� �Ҵ��ϰ�
	�ش� �޸��ּ� �� ������ �ϴ� ������Ʈ�� ������ �� �ִ� �����ʹ� LPDIRECT3DVERTEXBUFFER9�� �����Ѵ�.

	2����
	0 => ���� ���ؽ� �迭�� ����ڴ� (�б⸸ ����)
	D3DUSAGE_DYNAMIC => ���� �迭�� ����ڴ� (�б� ���� �Ѵ� ����) ->�ӵ��� ��¦ �������� ������ �����ϴ�

	4����
	���ؽ� ���۸� �Ҵ��� ��ġ�� ����
	D3DPOOL_DEFAULT �� ���� �޸� �� ���ҽ��� ��ġ
	D3DPOOL_MANAGED �� ���� �޸��� ���ҽ��� �ý��۸޸𸮿� ���
	D3DPOOL_SYSTEMMEM �� �ý��� �޸𸮿� ��ġ
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

		/* void*�� _byte*�� ĳ������ ���� : �ּҿ� ����� �������� �� �������Ʈũ�⸸ŭ �̵��� �� �ֵ��� ������ֱ�����. */
		/* _short*�������������� 1�����ϸ� 2byte�� �̵��Ѵ�.  */
		/* _uint*�������������� 1�����ϸ� 4byte�� �̵��Ѵ�.  */
		/* _byte*�������������� 1�����ϸ� 1byte�� �̵��Ѵ�.  */

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
	1���� �ε��� ������ �� ������
	3���� �ε��� �ϳ��� ũ��
	5���� �ε��� ���۸� ���� ����
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
