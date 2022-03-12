#pragma once


static const int g_iWinCX = 1280;
static const int g_iWinCY = 720;
extern HINSTANCE g_hInst;
extern HWND g_hWnd;
extern _float g_fDeltaTime;

static const _tchar* g_Path_Data = L"../Bin/Resources/Textures/TexturePathData/";
static const _tchar* g_Path_Data_MapTXT = L"../Bin/Resources/Textures/TexturePathData/map.txt";

static const _tchar* g_Path_TextureData = L"../Bin/Resources/Textures/TexturePathData/";


#define FPS		1.f/60.f

namespace Client {};
using namespace Client;

#include "Client_Enum.h"
#include "Client_Struct.h"
#include "Quest.h"

#define GETIMGUI GetSingle(CGameInstance)->GetIMGui()

#define NOT_EXIST_BLOCK		_float3(9999.f,9999.f,9999.f)

#define  ZEROVEC	_float3(0,0,0)
#define  LEFTVEC	_float3(-1,0,0)	
#define  RIGHTVEC	_float3(1,0,0)
#define  UPVEC		_float3(0,1,0)
#define  DOWNVEC	_float3(0,-1,0)
#define  FRONTVEC	_float3(0,0,1)
#define  BACKVEC	_float3(0,0,-1)


