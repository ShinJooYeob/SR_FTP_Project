#pragma once


static const int g_iWinCX = 1280;
static const int g_iWinCY = 720;
extern HINSTANCE g_hInst;
extern HWND g_hWnd;

static const _tchar* g_Path_Data = L"../Bin/Resources/Textures/TexturePathData/";
static const _tchar* g_Path_Data_MapTXT = L"../Bin/Resources/Textures/TexturePathData/map.txt";

static const _tchar* g_Path_TextureData = L"../Bin/Resources/Textures/TexturePathData/";


#define FPS		1.f/60.f

namespace Client {};
using namespace Client;

#include "Client_Enum.h"
#include "Client_Struct.h"


#define GETIMGUI GetSingle(CGameInstance)->GetIMGui()

#define NOT_EXIST_BLOCK		_float3(9999.f,9999.f,9999.f)
