#pragma once

#define TOOL_WINCX		1280
#define TOOL_WINCY		800

#define		TILECX	130
#define		TILECY	68

#define		TILEX	20
#define		TILEY	30

#define		RUBY		0x01		// -> 0001
#define		DIAMOND		0X02		// -> 0010
#define		SAPPHIRE	0x04		// -> 0100

#define		MIN_STR		64
#define		MAX_STR		256

#define	MSG_BOX(message)			MessageBoxW(g_hWnd, message, L"System Message", MB_OK)
#define FAILED_TOOL {AfxMessageBox(L"Fail", MB_OK);__debugbreak();}
#define FAILED_TOOL_MSG(message) {AfxMessageBox(message, MB_OK);__debugbreak();}

enum SCENEID
{
	SCENE_STATIC,
	SCENE_LOBY,
	SCENE_IMGUISCENE,
	SCENE_END
};



extern HWND g_hWnd;
extern HWND g_hWnd2;

extern HINSTANCE g_hInstance;
// 저장 파일 이름

static const wstring g_FilePath_TexturePathData = L"../bin/Resources/textures/TexturePathData/";
// static const wstring g_FilePath_ObjectPathData = L"../bin/Resources/mapdata/";

// 오브젝트 세이브
static const wstring g_FilePath_MapPathData_Save = L"\\bin\\Resources\\mapdata\\map\\";
static const wstring g_FilePath_ObjectPathData_Save = L"\\bin\\Resources\\mapdata\\object\\";

// 오브젝트 로드용
static const wstring g_FilePath_ObjectsPathData_Load = L"../bin/Resources/mapdata/object/";


static const TCHAR* g_FileName_Blocktxt = TEXT("Cube_Texture.txt");
static const TCHAR* g_Filename_Cubetxt = TEXT("Cube_Texture.txt");

#define  UPVEC _float3(0,1,0)
#define  DOWNVEC _float3(0,-1,0)
#define  LEFTVEC _float3(-1,0,0)
#define  RIGHTVEC _float3(1,0,0)
#define  FRONTVEC _float3(0,0,1)
#define  BACKVEC _float3(0,0,-1)
#define  ZEROVEC _float3(0,0,0)


#define COLOR_BACK_TOOLA		D3DCOLOR_ARGB(255, 0, 0, 0)
#define COLOR_BACK_TOOLB		D3DCOLOR_ARGB(255, 0, 0, 255)
#define COLOR_BACK_TOOLC		D3DCOLOR_ARGB(255, 0, 255, 0)

namespace Tool {};

using namespace Engine;
using namespace Tool;
