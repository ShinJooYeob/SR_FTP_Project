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
	// ������ ������ ���� ���� ������ ���
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
	// ������ ���Ȱ� ����� ������ �ʾ��� ���
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
