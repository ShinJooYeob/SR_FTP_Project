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
	//���° ��Ʈ���̳�, � ���۸� ���ε��ұ�, ���° ����Ʈ���� ����, �ѹ��� ũ�⸦ �󸶳� ��������
	m_pGraphicDevice->SetStreamSource(0, m_pVB, 0, m_iStride);
	m_pGraphicDevice->SetIndices(m_pIB);
	m_pGraphicDevice->SetFVF(m_dwFVF);

	// m_pGraphic_Device->DrawPrimitive(m_ePrimitiveType, 0, m_iNumPrimitive);
	m_pGraphicDevice->DrawIndexedPrimitive(m_eDrawPrimitive,0,0,m_iNumVertices, 0, m_iNumPrimitive);
	//?/*2: ���ؽ� ������ ������ġ 3:�ε��� ������ ���� ��ġ*/


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
		Safe_Delete_Array(m_pVertices);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

}
