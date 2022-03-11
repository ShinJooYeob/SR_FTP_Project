#include "..\Public\VIBuffer_Parsed.h"






CVIBuffer_Parsed::CVIBuffer_Parsed(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CVIBuffer(pGraphicDevice)
{
}

CVIBuffer_Parsed::CVIBuffer_Parsed(const CVIBuffer_Parsed & rhs)
	: CVIBuffer(rhs)
{
}


HRESULT CVIBuffer_Parsed::Initialize_Prototype(void * pArg)
{

	if (pArg == nullptr)
		return E_FAIL;

	PASRSEDDESC tDesc;

	memcpy(&tDesc, pArg, sizeof(tDesc));

	//list<_float3>*	vPos;
	//list<_float2>*	vUV;
	//list<_float>*	vIndex;


	//////////vertex버퍼///////////////
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = tDesc.vPos->size();
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	m_eDrawPrimitive = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = tDesc.vIndex->size();


	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXTEX) * m_iNumVertices);

	/*
	만들어진 컴객체를 통해 Lock이라는 함수를 호출하면 
	만들어진 해당 컴객체 안의 맴버 변수에 다른 쓰래드가 접근 하는 것을 막을 뿐만아니라
	해당 맴버 변수에 접근할 수 있는 Pointer를 받을 수 있다.
	*/

	if (m_pVB == nullptr)
		return E_FAIL;

	VTXTEX* pVertices = nullptr;

	auto PosListIter = tDesc.vPos->begin();
	auto UVListIter = tDesc.vUV->begin();

	m_pVB->Lock(0, 0, (void**)(&pVertices), 0);


	for (_uint i = 0; i < m_iNumVertices; i++)
	{
		pVertices[i].vPosition = (*PosListIter);
		pVertices[i].vTexUV = (*UVListIter);

		PosListIter++;
		UVListIter++;
	}

	memcpy(m_pVertices, pVertices, sizeof(VTXTEX) * m_iNumVertices);
	m_pVB->Unlock();	


	//////////Index버퍼///////////////


	m_pIndices = new unsigned long[tDesc.vIndex->size()];

	ZeroMemory(m_pIndices, sizeof(unsigned long) * (tDesc.vIndex->size()));

	m_iIndicesSize = sizeof(FACEINDICES32);
	m_eIndexFormat = D3DFMT_INDEX32;
	
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	unsigned long* pIndices = nullptr;
	auto IndexListIter = tDesc.vIndex->begin();

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (_uint i = 0; i < tDesc.vIndex->size(); i++)
	{
		pIndices[i] = (*IndexListIter);
		IndexListIter++;
	}


	m_pIB->Unlock();

	memcpy(m_pIndices, pIndices, sizeof(unsigned long) * (tDesc.vIndex->size()));

	return S_OK;
}

HRESULT CVIBuffer_Parsed::Initialize_Clone(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Parsed::Fix_Vertex_By_Postion(_float3 vPos)
{

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)(&pVertices), 0);


	for (_uint i = 0; i < m_iNumVertices; i++)
	{
		pVertices[i].vPosition += vPos;
	}

	m_pVB->Unlock();
	return S_OK;
}


CVIBuffer_Parsed * CVIBuffer_Parsed::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CVIBuffer_Parsed* pInstance = new CVIBuffer_Parsed(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg))) 
	{

		MSGBOX("Failed to Creating VIBuffer_Parsed Prototype");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CVIBuffer_Parsed::Clone(void * pArg)
{
	CComponent* pInstance = new CVIBuffer_Parsed((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{

		MSGBOX("Failed to Creating CVIBuffer_Parsed");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVIBuffer_Parsed::Free()
{
	__super::Free();
}
