#pragma once

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


// 텍스처의 위치정보를 저장하는 구조체
typedef struct tagTexturePath
{
	// 파일구분 1 
	wstring			wstrObjKey = L"";
	// 파일구분 2
	wstring			wstrStateKey = L"";
	// 전체 경로
	wstring			wstrPath = L"";
	int				iCount = 0;
}IMGPATH;

// 텍스처의 위치정보를 저장하는 구조체
typedef struct tagMyTexturePath
{
	// 파일구분 1 
	wstring			wFolderName1 = L"";
	// 파일구분 2
	wstring			wFolderName2 = L"";
	// 파일구분 3 
	wstring			wFolderName3 = L"";
	// 파일구분 4
	wstring			wFolderName4 = L"";
	// 파일구분 5 
	wstring			wFolderName5 = L"";
	
	// 전체 상대 경로
	wstring			wstrFullPath = L"";
	
	// 텍스처 개수
	int				iCount = 0;

}MYFILEPATH;

enum E_FILETYPE
{
	FILETYPE_PNG,
	FILETYPE_XML,
	FILETYPE_ALL,
	FILETYPE_END,

};

enum E_TRANSTYPE
{
	TRANSTYPE_POS,
	TRANSTYPE_ROT,
	TRANSTYPE_SCALE,
	TRANSTYPE_END,

};


typedef struct tagFrame
{
	tagFrame() {}
	tagFrame(float _fFrame, float _fMax)
		: fFrame(_fFrame), fMax(_fMax)
	{
	}

	float   fFrame;	// 현재 출력되는 이미지 인덱스
	float	fMax;	// 최대 이미지 장수
}FRAME;