#pragma once

#pragma region 예제데이터


typedef struct tagInfo
{
	D3DXVECTOR3	vPos;
	D3DXVECTOR3	vDir;
	D3DXVECTOR3 vLook;
	D3DXMATRIX	matWorld;
}INFO;

typedef struct tagTexture
{
	// 이미지 한 장을 제어하는 com객체
	LPDIRECT3DTEXTURE9		pTexture;

	// 이미지 정보를 보관하기 위한 구조체
	D3DXIMAGE_INFO			tImgInfo;
}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3	vPos;
	D3DXVECTOR3 vSize;

	BYTE		byOption;
	BYTE		byTileIdx;

	// 현재 타일의 인덱스
	int			iIndex;
	// 부모 타일의 인덱스
	int			iParentIndex;
}TILE;

typedef struct tagUnitData
{
#ifndef _AFX
	wstring			strName;
#else
	CString		strName;
#endif

	int			iAttack;
	int			iDef;
	BYTE		byJobIndex;
	BYTE		byItem;
}UNITDATA;



#pragma endregion
