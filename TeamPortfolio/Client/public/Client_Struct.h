#pragma once


// 저장 불러오기 데이터 타입
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
	TCHAR			strObjectName[STR_MAX];	// 파일이름 / 객체 이름

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

	unsigned int		StateIndex;				// 몇번째 텍스처 
	unsigned int		CubeID;					// 어떤 종류의 큐브인지 판단 
	OUTPUT_TETURE		TexDesc;				// 텍스처 정보
	D3DXMATRIX			WorldMatData;			// 월드 행렬 정보 // 

}OUTPUT_OBJECTINFO;