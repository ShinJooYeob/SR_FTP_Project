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
enum E_OUTPUTID {OUTPUT_NONE,OUTPUT_OBJECT,OUTPUT_MAP,OUTPUT_END};
#define  STR_MAX 64

typedef struct tagOutputFileidDESC
{
	explicit tagOutputFileidDESC(E_OUTPUTID id,const _tchar* str)
	{
		FILEID = id;
		lstrcpy(strObjectName, str);

	}
	explicit tagOutputFileidDESC()
	{
		FILEID = OUTPUT_OBJECT;
		lstrcpy(strObjectName, L"");

	}

	explicit tagOutputFileidDESC(const tagOutputFileidDESC& rhs)
	{
		FILEID = rhs.FILEID;
		lstrcpy(strObjectName, rhs.strObjectName);
	}


	tagOutputFileidDESC&  operator=(const tagOutputFileidDESC& rhs)
	{
		FILEID = rhs.FILEID;
		lstrcpy(strObjectName, rhs.strObjectName);
		return *this;
	}

	E_OUTPUTID		FILEID = OUTPUT_NONE;
	TCHAR			strObjectName[STR_MAX];	// �����̸� / ��ü �̸�

}OUTPUT_ID;

typedef struct tagOutputTetureDESC
{
	_uint		eTextureType;
	TCHAR		szTextFilePath[STR_MAX];

}OUTPUT_TETURE;

typedef struct tagOutputObject
{
	explicit tagOutputObject()
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
		TexDesc = rhs.TexDesc;
		lstrcpy(TexDesc.szTextFilePath,rhs.TexDesc.szTextFilePath);
	}


	tagOutputObject&  operator=(const tagOutputObject& rhs)
	{
		WorldMatData = rhs.WorldMatData;

		StateIndex = rhs.StateIndex;


		TexDesc = rhs.TexDesc;

		lstrcpy(TexDesc.szTextFilePath, rhs.TexDesc.szTextFilePath);
		return *this;
	}

	unsigned int		StateIndex;				// ���° �ؽ�ó 
	unsigned int		CubeID;					// � ������ ť������ �Ǵ� 
	OUTPUT_TETURE		TexDesc;				// �ؽ�ó ����
	D3DXMATRIX			WorldMatData;			// ���� ��� ���� // 

}OUTPUT_OBJECTINFO;

//typedef struct tagOutputMap
//{
//	vector<OUTPUT_OBJECTINFO*> m_Vec_Map;
//	unsigned int iCount;
//
//}OUTPUT_MAPINFO;


