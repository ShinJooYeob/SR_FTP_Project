#pragma once


// ���� �ҷ����� ������ Ÿ��
enum E_OUTPUTID { OUTPUT_NONE, OUTPUT_OBJECT, OUTPUT_MAP, OUTPUT_END };
#define  STR_MAX 64

typedef struct tagOutputFileidDESC
{
	explicit tagOutputFileidDESC(E_OUTPUTID id, const _tchar* str)
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
		lstrcpy(TexDesc.szTextFilePath, rhs.TexDesc.szTextFilePath);
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