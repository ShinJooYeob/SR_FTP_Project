#include "..\Public\VIBuffer_Cube_Nomal.h"



CVIBuffer_Cube_Normal::CVIBuffer_Cube_Normal(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Cube_Normal::CVIBuffer_Cube_Normal(const CVIBuffer_Cube_Normal & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cube_Normal::Initialize_Prototype(void* pArg)
{
	m_iStride = sizeof(VTXCUBENORMTEX);
	m_iNumVertices = 8;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_NORMAL| D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	m_eDrawPrimitive = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_pVertices = new VTXCUBENORMTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXCUBENORMTEX) * m_iNumVertices);

	VTXCUBENORMTEX*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	_uint VertexNumber = 0;
	for (int Z = 0; Z < 2; ++Z)
	{
		for (int Y = 0; Y < 2; ++Y)
		{
			for (int X = 0; X < 2; ++X)
			{
				pVertices[VertexNumber].vPosition = _float3(X - 0.5f, Y - 0.5f, Z - 0.5f);
				pVertices[VertexNumber].vNormal = pVertices[VertexNumber].vPosition;
				pVertices[VertexNumber].vTexUV = pVertices[VertexNumber].vPosition;


				((VTXCUBENORMTEX*)m_pVertices)[VertexNumber] = pVertices[VertexNumber];

				++VertexNumber;
			}
		}
	}

	m_pVB->Unlock();




	m_iIndicesSize = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	m_pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	FACEINDICES16*		pIndices = nullptr;


	_uint IndexNumber = 0;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);


	/////+X
	pIndices[IndexNumber]._0 = 3;
	pIndices[IndexNumber]._1 = 7;
	pIndices[IndexNumber]._2 = 5;
	++IndexNumber;

	pIndices[IndexNumber]._0 = 3;
	pIndices[IndexNumber]._1 = 5;
	pIndices[IndexNumber]._2 = 1;
	++IndexNumber;


	/////// -X
	pIndices[IndexNumber]._0 = 6;
	pIndices[IndexNumber]._1 = 2;
	pIndices[IndexNumber]._2 = 0;
	++IndexNumber;

	pIndices[IndexNumber]._0 = 6;
	pIndices[IndexNumber]._1 = 0;
	pIndices[IndexNumber]._2 = 4;
	++IndexNumber;


	///// +Y
	pIndices[IndexNumber]._0 = 6;
	pIndices[IndexNumber]._1 = 7;
	pIndices[IndexNumber]._2 = 3;
	++IndexNumber;

	pIndices[IndexNumber]._0 = 6;
	pIndices[IndexNumber]._1 = 3;
	pIndices[IndexNumber]._2 = 2;
	++IndexNumber;

	////// -Y
	pIndices[IndexNumber]._0 = 0;
	pIndices[IndexNumber]._1 = 1;
	pIndices[IndexNumber]._2 = 5;
	++IndexNumber;

	pIndices[IndexNumber]._0 = 0;
	pIndices[IndexNumber]._1 = 5;
	pIndices[IndexNumber]._2 = 4;
	++IndexNumber;


	////// +Z
	pIndices[IndexNumber]._0 = 7;
	pIndices[IndexNumber]._1 = 6;
	pIndices[IndexNumber]._2 = 4;
	++IndexNumber;

	pIndices[IndexNumber]._0 = 7;
	pIndices[IndexNumber]._1 = 4;
	pIndices[IndexNumber]._2 = 5;
	++IndexNumber;


	//////-Z
	pIndices[IndexNumber]._0 = 2;
	pIndices[IndexNumber]._1 = 3;
	pIndices[IndexNumber]._2 = 1;
	++IndexNumber;

	pIndices[IndexNumber]._0 = 2;
	pIndices[IndexNumber]._1 = 1;
	pIndices[IndexNumber]._2 = 0;
	++IndexNumber;


	m_pIB->Unlock();


	memcpy(m_pIndices, pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);





	return S_OK;
}

HRESULT CVIBuffer_Cube_Normal::Initialize_Clone(void * pArg)
{
	return S_OK;
}

CVIBuffer_Cube_Normal * CVIBuffer_Cube_Normal::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CVIBuffer_Cube_Normal*	pInstance = new CVIBuffer_Cube_Normal(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Cube_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Cube_Normal::Clone(void * pArg)
{
	CVIBuffer_Cube_Normal*	pInstance = new CVIBuffer_Cube_Normal(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Creating CVIBuffer_Cube_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cube_Normal::Free()
{
	__super::Free();
}
