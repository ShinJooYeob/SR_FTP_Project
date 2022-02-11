#include "..\Public\VIBuffer_Rect.h"





CVIBuffer_Rect::CVIBuffer_Rect(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CVIBuffer(pGraphicDevice)
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect & rhs)
	: CVIBuffer(rhs)
{
}


HRESULT CVIBuffer_Rect::Initialize_Protoype(void * pArg)
{

	//////////vertex버퍼///////////////
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	m_eDrawPrimitive = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;


	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	/*
	만들어진 컴객체를 통해 Lock이라는 함수를 호출하면 
	만들어진 해당 컴객체 안의 맴버 변수에 다른 쓰래드가 접근 하는 것을 막을 뿐만아니라
	해당 맴버 변수에 접근할 수 있는 Pointer를 받을 수 있다.
	*/

	if (m_pVB == nullptr)
		return E_FAIL;

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)(&pVertices), 0);

	pVertices[0].vPosition = _float3(-0.5f,0.5f,0.f);
	pVertices[0].vTexUV =		_float2(0.0f, 0.0f);


	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(1.0f, 0.0f);


	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(1.0f, 1.0f);


	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = _float2(0.0f, 1.0f);


	m_pVB->Unlock();	

	//////////Index버퍼///////////////
	m_iIndicesSize = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;
	
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16* pIndices = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);
	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;
	m_pIB->Unlock();



	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize_Clone(void * pArg)
{
	return S_OK;
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Protoype(pArg))) 
	{

		MSGBOX("Failed to Creating VIBuffer_Rect Prototype");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CVIBuffer_Rect::Clone(void * pArg)
{
	CComponent* pInstance = new CVIBuffer_Rect((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{

		MSGBOX("Failed to Creating CVIBuffer_Rect");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();
}
