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
extern HINSTANCE g_hInstance;
// 저장 파일 이름

static const wstring g_FilePath_TexturePathData = L"../bin/Resources/textures/TexturePathData/";
static const wstring g_FilePath_ObjectPathData = L"../bin/Resources/Data/";
static const wstring g_FilePath_ObjectPathData_Save = L"\\bin\\Resources\\Data\\";


static const TCHAR* g_FileName_Blocktxt = TEXT("Blocks.txt");
static const TCHAR* g_Filename_Cubetxt = TEXT("Cube.txt");

namespace Tool {};

using namespace Tool;
