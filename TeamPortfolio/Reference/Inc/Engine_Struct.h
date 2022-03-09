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

	/////���� ���� ����
	typedef struct tagVertex_Nomal_Texture
	{
		D3DXVECTOR3			vPosition;
		D3DXVECTOR3			vNormal;
		D3DXVECTOR2			vTexUV;
	}VTXNORMTEX;
	// ���� ���� ���� ť��
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



	// ���� �ҷ����� ���� ������ Ÿ��
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
		TCHAR				szTextFilePath[STR_MAX];	// �ؽ�ó ��� / txt����
		TCHAR				szStateKey[STR_MAX];		// �ؽ�ó ������ƮŰ 
		unsigned int		StateIndex;					// �ؽ�ó ��ȣ


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

		unsigned int		CubeID;					// � ������ ť������ �Ǵ� 
		OUTPUT_TETURE		TexDesc;				// �ؽ�ó ����
		D3DXMATRIX			WorldMatData;			// ���� ��� ���� // 

	}OUTPUT_OBJECTINFO;


	typedef struct tag_Sphere
	{
		tag_Sphere()
		{
			mRadius = 0; mCenterRadius = _float2(0, 0);
		}

		_float2 mCenterRadius; // ����
		_float mRadius; // ����
	}MYSPHERE;
}