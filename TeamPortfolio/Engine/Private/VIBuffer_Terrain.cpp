#include "..\Public\VIBuffer_Terrain.h"




CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CVIBuffer(pGraphicDevice)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs),
	m_TerrainDesc(rhs.m_TerrainDesc)
{
}

HRESULT CVIBuffer_Terrain::Initialize_Protoype(void * pArg)
{
	ZeroMemory(&m_TerrainDesc, sizeof(TERRAINDESC));
	
	if (pArg != nullptr)
		memcpy(&m_TerrainDesc, pArg, sizeof(TERRAINDESC));


	_ulong* pPixel = nullptr;

	if (m_TerrainDesc.szHeightFilePath == nullptr)
	{
		if (!m_TerrainDesc.iRow)m_TerrainDesc.iRow = 1;
		if (!m_TerrainDesc.iCol)m_TerrainDesc.iCol = 1;
	}
	else
	{
		HANDLE hFile = CreateFile(m_TerrainDesc.szHeightFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (hFile == nullptr)
			return E_FAIL;


		DWORD dwReadedByte = 0;
		BITMAPFILEHEADER FileHeader;
		BITMAPINFOHEADER InfoHeader;

		ReadFile(hFile, &FileHeader, sizeof(BITMAPFILEHEADER), &dwReadedByte, nullptr);
		ReadFile(hFile, &InfoHeader, sizeof(BITMAPINFOHEADER), &dwReadedByte, nullptr);
		if (dwReadedByte <= 0) {
			CloseHandle(hFile);
			return E_FAIL;
		}

		m_TerrainDesc.iRow = InfoHeader.biWidth - 1;
		m_TerrainDesc.iCol = InfoHeader.biHeight- 1;

		pPixel = new _ulong[InfoHeader.biWidth * InfoHeader.biHeight];


		ReadFile(hFile, pPixel, sizeof(_ulong)*InfoHeader.biWidth * InfoHeader.biHeight, &dwReadedByte, nullptr);

		CloseHandle(hFile);
		if (dwReadedByte <= 0) 
		{
			Safe_Delete_Array(pPixel);
			return E_FAIL;
		}

	}



	//////////vertex¹öÆÛ///////////////
	m_iStride = sizeof(VTXNORMTEX);
	m_iNumVertices = (m_TerrainDesc.iRow + 1) * (m_TerrainDesc.iCol + 1);
	m_dwFVF = D3DFVF_XYZ |D3DFVF_NORMAL| D3DFVF_TEX1;

	m_eDrawPrimitive = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = m_TerrainDesc.iRow * m_TerrainDesc.iCol * 2;


	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;


	if (m_pVB == nullptr)
		return E_FAIL;

	VTXNORMTEX* pVertices = nullptr;
	m_pVertices = new VTXNORMTEX[m_iNumVertices];

	m_pVB->Lock(0, 0, (void**)(&pVertices), 0);

	_uint iScopeU = _uint(m_TerrainDesc.iRow * 0.25f);
	_uint iScopeV = _uint(m_TerrainDesc.iCol * 0.25f);

	_float3 vMidPoint = _float3(m_TerrainDesc.iRow * 0.5f, (m_TerrainDesc.iRow + m_TerrainDesc.iCol)* -0.5f, m_TerrainDesc.iCol* 0.5f);

	for (_uint i = 0 ; i < m_iNumVertices ; i++)
	{
		int iX = i % (m_TerrainDesc.iRow + 1);
		int iZ = i / (m_TerrainDesc.iRow + 1);

		if (pPixel)
			pVertices[i].vPosition = _float3((float)iX, (pPixel[i] & 0x000000ff) * 0.1f, (float)iZ);
		else
			pVertices[i].vPosition = _float3((float)iX, 0, (float)iZ);

		pVertices[i].vTexUV		= _float2((iX /(float)m_TerrainDesc.iRow) * iScopeU, (1 - iZ/ (float)m_TerrainDesc.iCol)  * iScopeV);
		pVertices[i].vNormal = _float3(0, 1.f, 0);

		//pVertices[i].vNormal = pVertices[i].vPosition - vMidPoint;

		//D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);
		int t = 0;
				
	}

	memcpy(m_pVertices, pVertices, sizeof(VTXNORMTEX)*m_iNumVertices);

	m_pVB->Unlock();

	Safe_Delete_Array(pPixel);

	//////////Index¹öÆÛ///////////////
	m_iIndicesSize = sizeof(FACEINDICES32);
	m_eIndexFormat = D3DFMT_INDEX32;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES32* pIndices = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (_uint i = 0;i < m_iNumPrimitive; i++)
	{
		_uint iX = i % m_TerrainDesc.iRow;
		_uint iZ = i / m_TerrainDesc.iRow;
		_uint iVertexZ = iZ / 2;

		if (iZ % 2)
		//È¦¼ö ÃþÀÌ¶ó¸é
		{
			iVertexZ++;

			pIndices[i]._0 = (iVertexZ * (m_TerrainDesc.iRow + 1)) + (iX);
			pIndices[i]._1 = (iVertexZ * (m_TerrainDesc.iRow + 1)) + (iX + 1);
			pIndices[i]._2 = ((iVertexZ - 1) * (m_TerrainDesc.iRow + 1)) + (iX + 1 );
		}
		else 
		//Â¦¼ö ÃþÀÌ¶ó¸é
		{
			pIndices[i]._0 = (iVertexZ * (m_TerrainDesc.iRow+1)) + (iX + 1);
			pIndices[i]._1 = (iVertexZ * (m_TerrainDesc.iRow +1 )) + (iX);
			pIndices[i]._2 = ((iVertexZ + 1) * (m_TerrainDesc.iRow + 1)) + (iX);
		}
	}
	m_pIB->Unlock();



	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize_Clone(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Terrain::PointInTerrain(_float3 * pOut, _float3 vPos,_Matrix TerrainInversWorldMatrix)
{
	_uint iIndex[4];

	vPos = vPos.PosVector_Matrix(TerrainInversWorldMatrix);

	if (vPos.x < 0 || vPos.x >(m_TerrainDesc.iRow + 1) ||
		vPos.z < 0 || vPos.z >(m_TerrainDesc.iCol + 1))
		return E_FAIL;

	iIndex[0] = int(vPos.z) * (m_TerrainDesc.iRow + 1) + int(vPos.x);
	iIndex[1] = iIndex[0] + 1;
	iIndex[2] = iIndex[0] + (m_TerrainDesc.iRow + 1);
	iIndex[3] = iIndex[2] + 1;


	D3DXPLANE tPlane;

	VTXNORMTEX*			pVertices = (VTXNORMTEX*)m_pVertices;

	if (pVertices[iIndex[2]].vPosition.z - vPos.z > vPos.x - pVertices[iIndex[2]].vPosition.x)
	{

		D3DXPlaneFromPoints(&tPlane, &(pVertices[iIndex[0]].vPosition), 
			&(pVertices[iIndex[2]].vPosition), &(pVertices[iIndex[1]].vPosition));
	}
	else
	{
		D3DXPlaneFromPoints(&tPlane, &(pVertices[iIndex[2]].vPosition),
			&(pVertices[iIndex[3]].vPosition), &(pVertices[iIndex[1]].vPosition));
	}


	*pOut = _float3(vPos.x, (tPlane.a * vPos.x + tPlane.c * vPos.z + tPlane.d) / -tPlane.b, vPos.z).PosVector_Matrix(TerrainInversWorldMatrix.InverseMatrix());



	return S_OK;
}


CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphicDevice);


	if (FAILED(pInstance->Initialize_Protoype(pArg))) {

		MSGBOX("Failed to Creating CVIBuffer_Terrain Prototype");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg))) {

		MSGBOX("Failed to Creating CVIBuffer_Terrain clone");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
}
