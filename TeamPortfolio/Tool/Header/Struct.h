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

// 텍스처 오브젝트 / 스테이트 / 경로를 저장하는 구조체
typedef struct tagTexturePath
{
	wstring			wstrObjKey = L"";
	wstring			wstrStateKey = L"";
	wstring			wstrPath = L"";
	int				iCount = 0;

}IMGPATH;

// 텍스처의 위치정보를 저장하는 구조체
typedef struct tagMyTexturePath
{
	// 파일구분 1 
	wstring			wFileName = L"";	
	// 전체 상대 경로
	wstring			wstrFullPath = L"";	
	// 텍스처 개수
	int				iCount = 0;

}MYFILEPATH;

// 오브젝트에 대한 정보
// 오브젝트 월드 행렬 / 이름 / 텍스처경로 / 키
enum E_OUTPUTID {OUTPUT_NONE,OUTPUT_OBJECT,OUTPUT_MAP,OUTPUT_AA,OUTPUT_END};
#define  STR_MAX 64

typedef struct tagOutputTetureDESC
{
	_uint		eTextureType;
	TCHAR		szTextFilePath[STR_MAX];

}OUTPUT_TETURE;

typedef struct tagOutputObject
{
	explicit tagOutputObject()
		:strObjectName(L"")
	{
		D3DXMatrixIdentity(&WorldMatData);
		StateIndex = 0;
		TexDesc.eTextureType = CTexture::TYPE_CUBEMAP;
		lstrcpy(TexDesc.szTextFilePath, L"");
	}
	explicit tagOutputObject(const tagOutputObject& rhs)
	{
		WorldMatData = rhs.WorldMatData;

		StateIndex = rhs.StateIndex;
		lstrcpy(TexDesc.szTextFilePath, rhs.strObjectName);
		TexDesc = rhs.TexDesc;
		lstrcpy(TexDesc.szTextFilePath,rhs.TexDesc.szTextFilePath);
	}


	tagOutputObject&  operator=(const tagOutputObject& rhs)
	{
		WorldMatData = rhs.WorldMatData;

		StateIndex = rhs.StateIndex;

		lstrcpy(TexDesc.szTextFilePath, rhs.strObjectName);

		TexDesc = rhs.TexDesc;

		lstrcpy(TexDesc.szTextFilePath, rhs.TexDesc.szTextFilePath);
		return *this;
	}


	TCHAR			strObjectName[STR_MAX];	// 오브젝트 이름 / 파일 이름
	unsigned int	StateIndex;		// 어떤 종류의 큐브인지 판단 
	OUTPUT_TETURE	TexDesc;			// 텍스처 정보
	D3DXMATRIX		WorldMatData;		// 월드 행렬 정보 // 

}OUTPUT_OBJECTINFO;


typedef struct tagOutputMap
{
	vector<OUTPUT_OBJECTINFO> m_Vec_Map;
	unsigned int iCount;

}OUTPUT_MAPINFO;


//typedef struct tagTestString
//{
//	wstring a1;
//	wstring a2;
//	wstring a3;
//
//
//}OUTPUT_WSTRING;
//
