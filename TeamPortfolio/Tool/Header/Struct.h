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



#pragma endregion

// �ؽ�ó ������Ʈ / ������Ʈ / ��θ� �����ϴ� ����ü
typedef struct tagTexturePath
{
	wstring			wstrObjKey = L"";
	wstring			wstrStateKey = L"";
	wstring			wstrPath = L"";
	int				iCount = 0;

}IMGPATH;

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

// ������Ʈ�� ���� ����
// ������Ʈ ���� ��� / �̸� / �ؽ�ó��� / Ű
enum E_OUTPUTID {OUTPUT_NONE,OUTPUT_OBJECT,OUTPUT_MAP,OUTPUT_AA,OUTPUT_END};
typedef struct tagOutputObject
{
	TCHAR strObjectName[MAX_PATH];			// ������Ʈ �̸�
	unsigned int StateIndex;				// � ������ ť������ �Ǵ� 
	TCHAR strStrTextureFullPath[MAX_PATH];	// ��� �ؽ�ó ���1
	TCHAR strStrTextureFileName[MAX_PATH];	// ��� �ؽ�ó ���2
//	D3DXMATRIX	MatrixData; // ���� ��� ���� // 
	D3DXVECTOR3 fScale;
	D3DXVECTOR3 fRot;
	D3DXVECTOR3 fPos;

}OUTPUT_OBJECTINFO;


typedef struct tagOutputMap
{
	vector<OUTPUT_OBJECTINFO> m_Vec_Map;
	unsigned int iCount;

}OUTPUT_MAPINFO;


