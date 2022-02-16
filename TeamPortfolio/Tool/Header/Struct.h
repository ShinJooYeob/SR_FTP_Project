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

// �ؽ�ó�� ��ġ������ �����ϴ� ����ü
typedef struct tagMyTexturePath
{
	// ���ϱ��� 1 
	wstring			wFolderName1 = L"";
	// ���ϱ��� 2
	wstring			wFolderName2 = L"";
	// ���ϱ��� 3 
	wstring			wFolderName3 = L"";
	// ���ϱ��� 4
	wstring			wFolderName4 = L"";
	// ���ϱ��� 5 
	wstring			wFolderName5 = L"";
	
	// ��ü ��� ���
	wstring			wstrFullPath = L"";
	
	// �ؽ�ó ����
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

	float   fFrame;	// ���� ��µǴ� �̹��� �ε���
	float	fMax;	// �ִ� �̹��� ���
}FRAME;