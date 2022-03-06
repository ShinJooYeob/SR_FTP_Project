#include "..\Public\XMLMgr.h"
#include "VIBuffer_Parsed.h"

IMPLEMENT_SINGLETON(CXMLMgr);


CXMLMgr::CXMLMgr()
	:m_pGraphicDevice(nullptr)
{


}


HRESULT CXMLMgr::Initialize_XMLMgr(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	if (pGraphicDevice == nullptr)
		E_FAIL;

	m_pGraphicDevice = pGraphicDevice;

	Safe_AddRef(m_pGraphicDevice);

	return S_OK;
}

CVIBuffer_Parsed * CXMLMgr::Create_ParsedObject(wstring pPosTextFilePath, wstring pIndexTextFilePath)
{
	CVIBuffer_Parsed* pResult = nullptr;

	FAILED_CHECK_NONERETURN(Read_ObjectFiles(&pResult, pPosTextFilePath, pIndexTextFilePath));

	return pResult;
}

HRESULT CXMLMgr::Read_ObjectFiles(CVIBuffer_Parsed** pOut, wstring pPosTextFilePath, wstring pIndexTextFilePath)
{
	m_PosList.clear();
	m_UVList.clear();
	m_IndexList.clear();

	wifstream		fPosFile;

	_tchar PosFilePath[MAX_PATH] =L"../Bin/Resources/ModelData/ModelVertexData/";

	lstrcat(PosFilePath, pPosTextFilePath.c_str());

	fPosFile.open(PosFilePath);

	if (!fPosFile.fail())
	{
		_tchar	szPadding[MAX_PATH] = L"";

		_tchar	szPosX[128] = L"";
		_tchar	szPosY[128] = L"";
		_tchar	szPosZ[128] = L"";
		_tchar	szTexU[128] = L"";
		_tchar	szTexV[128] = L"";

		while (true)
		{

			// '|' 단위로 끊어서 문자열 입력 처리

			fPosFile.getline(szPadding, MAX_PATH, '"');
			fPosFile.getline(szPosX, MAX_PATH, '"');

			fPosFile.getline(szPadding, MAX_PATH, '"');
			fPosFile.getline(szPosY, MAX_PATH, '"');

			fPosFile.getline(szPadding, MAX_PATH, '"');
			fPosFile.getline(szPosZ, MAX_PATH, '"');

			fPosFile.getline(szPadding, MAX_PATH, '"');
			fPosFile.getline(szTexU, MAX_PATH, '"');

			fPosFile.getline(szPadding, MAX_PATH, '"');
			fPosFile.getline(szTexV, MAX_PATH, '"');

			if (fPosFile.eof())
				break;

			// 문자열을 정수로 변환 함수

			_float3 vPos;
			_float2 vUV;

			vPos.x = _wtof(szPosX);
			vPos.y = _wtof(szPosY);
			vPos.z = _wtof(szPosZ);

			vUV.x = _wtof(szTexU);
			vUV.y = _wtof(szTexV);


			m_PosList.push_back(vPos);
			m_UVList.push_back(vUV);


			if (fPosFile.eof())
				break;
		}

	}

	//////////////////////////////////////////////////////////////////////////

	wifstream		fIndexFile;

	_tchar IndexFilePath[MAX_PATH] = L"../Bin/Resources/ModelData/ModelIndexData/";

	lstrcat(IndexFilePath, pIndexTextFilePath.c_str());

	fIndexFile.open(IndexFilePath);

	if (!fIndexFile.fail())
	{
		_tchar	szPadding[MAX_PATH] = L"";

		_tchar	szIndex[128] = L"";

		while (true)
		{

			// '|' 단위로 끊어서 문자열 입력 처리

			fIndexFile.getline(szPadding, MAX_PATH, '>');
			fIndexFile.getline(szPadding, MAX_PATH, '>');
			fIndexFile.getline(szIndex, MAX_PATH, '<');


			if (fIndexFile.eof())
				break;

			_uint ParsedIndex;
			ParsedIndex = _wtof(szIndex);

			m_IndexList.push_back(ParsedIndex);


			if (fIndexFile.eof())
				break;
		}
	}

	CVIBuffer_Parsed::PASRSEDDESC tParseDesc;

	tParseDesc.vPos = &m_PosList;
	tParseDesc.vUV = &m_UVList;
	tParseDesc.vIndex = &m_IndexList;


	*pOut = CVIBuffer_Parsed::Create(m_pGraphicDevice, &tParseDesc);

	if (pOut == nullptr)
		return E_FAIL;

	return S_OK;
}

void CXMLMgr::Free()
{

	Safe_Release(m_pGraphicDevice);
}
