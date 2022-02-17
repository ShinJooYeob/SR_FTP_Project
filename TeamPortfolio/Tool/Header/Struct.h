#pragma once

#pragma region ����������


typedef struct tagInfo
{
	D3DXVECTOR3	vPos;
	D3DXVECTOR3	vDir;
	D3DXVECTOR3 vLook;
	D3DXMATRIX	matWorld;
}INFO;

typedef struct tagTexture
{
	// �̹��� �� ���� �����ϴ� com��ü
	LPDIRECT3DTEXTURE9		pTexture;

	// �̹��� ������ �����ϱ� ���� ����ü
	D3DXIMAGE_INFO			tImgInfo;
}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3	vPos;
	D3DXVECTOR3 vSize;

	BYTE		byOption;
	BYTE		byTileIdx;

	// ���� Ÿ���� �ε���
	int			iIndex;
	// �θ� Ÿ���� �ε���
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


// �ؽ�ó�� ��ġ������ �����ϴ� ����ü
typedef struct tagTexturePath
{
	// ���ϱ��� 1 
	wstring			wstrObjKey = L"";
	// ���ϱ��� 2
	wstring			wstrStateKey = L"";
	// ��ü ���
	wstring			wstrPath = L"";
	int				iCount = 0;
}IMGPATH;
#pragma endregion


// �ؽ�ó�� ��ġ������ �����ϴ� ����ü
typedef struct tagMyTexturePath
{
	// ���ϱ��� 1 
	wstring			wFileName = L"";	
	// ��ü ��� ���
	wstring			wstrFullPath = L"";	
	// �ؽ�ó ����
	int				iCount = 0;

}MYFILEPATH;

typedef struct tagOutputObject
{
	D3DXVECTOR3 fScale;
	D3DXVECTOR3 fPos;
	TCHAR strObjectName[MAX_PATH];
	TCHAR strTextureName[MAX_PATH];
	TCHAR strTexturePath[MAX_PATH];

}OUTPUT_OBJECTINFO;



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
