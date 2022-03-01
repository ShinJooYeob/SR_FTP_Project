#pragma once
#include "Base.h"

BEGIN(Engine)
class CVIBuffer_Parsed;

class CXMLMgr :public CBase
{
	DECLARE_SINGLETON(CXMLMgr);

private:
	explicit CXMLMgr();
	virtual ~CXMLMgr() = default;

public:
	HRESULT Initialize_XMLMgr(LPDIRECT3DDEVICE9 pGraphicDevice);

public:
	CVIBuffer_Parsed* Create_ParsedObject(wstring pPosTextFilePath, wstring pIndexTextFilePath);


private:
	HRESULT Read_ObjectFiles(CVIBuffer_Parsed** pOut, wstring pPosTextFilePath, wstring pIndexTextFilePath);

private:
	list<_float3>		m_PosList;
	list<_float2>		m_UVList;
	list<_uint>		m_IndexList;

private:
	LPDIRECT3DDEVICE9 m_pGraphicDevice;

public:
	virtual void Free() override;

};
END

