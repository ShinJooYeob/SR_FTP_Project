#pragma once
#pragma warning(disable : 4251)



#include <d3d9.h>
#include <d3dx9.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#include <typeinfo>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <process.h>
#include <time.h>

#include <string>
#include <fstream>
#include <tchar.h>

#include <math.h>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>

using namespace std;

/////////비디오 포함헤더
#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

////////사운드 포함 헤더/////////////
#include <io.h>
#include "fmod.h"
//#include "fmod.hpp"
#pragma comment(lib, "fmod_vc.lib")


#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG


#include "Engine_Typedef.h"
#include "Engine_Struct.h"
#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Easing.h"
#include "Engine_Enum.h"


using namespace Engine;
