#pragma once
#include "VIBuffer.h"


BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain : public CVIBuffer
{
public:
	typedef struct tagTerrainDesc {	
		//���� �� �簢���� ���� (������ ���� �ƴ�)
		_uint  iRow = 0;
		//���� �� �簢���� ���� (������ ���� �ƴ�)
		_uint  iCol = 0;
		//Height Map ���� ���
		const _tchar* szHeightFilePath = nullptr;
	}TERRAINDESC;

public:
	HRESULT PointInTerrain(_float3* pOut, _float3 vPos,_Matrix TerrainInversWorldMatrix);

private:
	explicit CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

private:
	virtual HRESULT Initialize_Protoype(void * pArg);
	virtual HRESULT Initialize_Clone(void * pArg);


	TERRAINDESC		m_TerrainDesc;

public:
	static CVIBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;
};

END