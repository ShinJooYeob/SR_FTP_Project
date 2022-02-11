#pragma once
#pragma warning(disable : 4251)


#include <d3d9.h>
#include <d3dx9.h>
#include <typeinfo>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <process.h>
#include <time.h>

#include <math.h>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>

using namespace std;

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


using namespace Engine;