#pragma once

namespace Engine
{

	typedef struct tagArg
	{
		void* pArg;
		CRITICAL_SECTION*	CriSec;
		_bool* IsClientQuit;

	}THREADARG;


	////VertexBuffer///////////////////////////////////////
	typedef struct tagVertex_Texture
	{
		D3DXVECTOR3			vPosition;
		D3DXVECTOR2			vTexUV;
	}VTXTEX;

	/////법선 벡터 포함
	typedef struct tagVertex_Nomal_Texture
	{
		D3DXVECTOR3			vPosition;
		D3DXVECTOR3			vNormal;
		D3DXVECTOR2			vTexUV;
	}VTXNORMTEX;
	// 법선 벡터 포함 큐브
	typedef struct tagVertex_Nomal_CubeTexture
	{
		D3DXVECTOR3			vPosition;
		D3DXVECTOR3			vNormal;
		D3DXVECTOR3			vTexUV;
	}VTXCUBENORMTEX;

	typedef struct tagVertex_CubeTexture
	{
		D3DXVECTOR3			vPosition;
		D3DXVECTOR3			vTexUV;
	}VTXCUBETEX;


	typedef struct tagLineIndices32
	{
		unsigned long	_0, _1;
	}LINEINDICES32;


	typedef struct tagLineIndices16
	{
		unsigned short	_0, _1;
	}LINEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long	_0, _1, _2;
	}FACEINDICES32;


	typedef struct tagFaceIndices16
	{
		unsigned short	_0, _1, _2;
	}FACEINDICES16;



	// 저장 불러오기 관련 데이터 타입
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
		explicit tagOutputTetureDESC()
		{
			eTextureType = 1;
			lstrcpy(szTextFilePath, L"");
			lstrcpy(szStateKey, L"");
			StateIndex = 0;
		}
		explicit tagOutputTetureDESC(const tagOutputTetureDESC& rhs)
		{
			eTextureType = rhs.eTextureType;
			StateIndex = rhs.StateIndex;
			lstrcpy(szTextFilePath, rhs.szTextFilePath);
			lstrcpy(szStateKey, rhs.szStateKey);
		}
		_uint				eTextureType;
		TCHAR				szTextFilePath[STR_MAX];	// 텍스처 경로 / txt파일
		TCHAR				szStateKey[STR_MAX];		// 텍스처 스테이트키 
		unsigned int		StateIndex;					// 텍스처 번호


	}OUTPUT_TETURE;

	typedef struct tagOutputObject
	{
		explicit tagOutputObject()
		{
			CubeID = 0;
			D3DXMatrixIdentity(&WorldMatData);
			TexDesc = tagOutputTetureDESC();
		}
		explicit tagOutputObject(const tagOutputObject& rhs)
		{
			CubeID = rhs.CubeID;
			WorldMatData = rhs.WorldMatData;
			TexDesc = tagOutputTetureDESC(rhs.TexDesc);
		}


		tagOutputObject&  operator=(const tagOutputObject& rhs)
		{
			CubeID = rhs.CubeID;
			WorldMatData = rhs.WorldMatData;
			TexDesc = tagOutputTetureDESC(rhs.TexDesc);
			return *this;
		}

		unsigned int		CubeID;					// 어떤 종류의 큐브인지 판단 
		OUTPUT_TETURE		TexDesc;				// 텍스처 정보
		D3DXMATRIX			WorldMatData;			// 월드 행렬 정보 // 

	}OUTPUT_OBJECTINFO;


	typedef struct tag_Sphere
	{
		tag_Sphere()
		{
			mRadius = 0; mCenterPosition = _float2(0, 0);
		}

		_float2 mCenterPosition; // 중점
		_float mRadius; // 범위
	}MYSPHERE;
}