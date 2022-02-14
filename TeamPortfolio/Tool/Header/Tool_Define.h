#pragma once

#define WINCX		1280
#define WINCY		800

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
// #define GetGameInstance				GetSingle(CGameInstance)	

enum SCENE_ID
{
	SCENE_ID_STATIC,
	SCENE_ID_A,
	SCENE_ID_B,
	SCENE_ID_END,
};

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

