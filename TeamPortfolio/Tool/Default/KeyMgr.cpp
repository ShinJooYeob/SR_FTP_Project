#include "stdafx.h"
#include "KeyMgr.h"


IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
{
	ZeroMemory(mbKeyState, sizeof(mbKeyState));
}

bool CKeyMgr::Init()
{
	ZeroMemory(mbKeyState, sizeof(mbKeyState));
	return true;
}

bool CKeyMgr::Key_Pressing(int key)
{
	if (GetAsyncKeyState(key) && 0x8000)
		return true;
	return false;
}

bool CKeyMgr::Key_Down(int key)
{
	// 이전에 눌림이 없고 현재 눌렸을 경우
	if (!mbKeyState[key] && (GetAsyncKeyState(key) && 0x8000))
	{
		mbKeyState[key] = !mbKeyState[key];
		return true;
	}
	if (mbKeyState[key] && !(GetAsyncKeyState(key) && 0x8000))
		mbKeyState[key] = !mbKeyState[key];
	return false;
}

bool CKeyMgr::Key_Up(int key)
{
	// 이전에 눌렸고 현재는 눌리지 않았을 경우
	if (mbKeyState[key] && !(GetAsyncKeyState(key) && 0x8000))
	{
		mbKeyState[key] = !mbKeyState[key];
		return true;
	}
	if (!mbKeyState[key] && (GetAsyncKeyState(key) && 0x8000))
		mbKeyState[key] = !mbKeyState[key];
	return false;
}

void CKeyMgr::Free()
{
}
