#pragma once

namespace Engine {

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



} 



