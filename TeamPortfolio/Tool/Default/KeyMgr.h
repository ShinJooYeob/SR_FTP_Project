#pragma once
#include "Include.h"
#include "Base.h"

// 싱글턴으로 만들기
#define VK_MAX 255

class CKeyMgr : public CBase
{
private:
	bool mbKeyState[VK_MAX];

private:
	CKeyMgr();
	~CKeyMgr() = default;

public:
// 싱글턴 구현
	DECLARE_SINGLETON(CKeyMgr)

public:
	bool Init();
	// 키 누르기 상태
	// 키매니저에서 자기가 만들키의 예외처리를 해야한다.
	bool Key_Pressing(int key);
	bool Key_Down(int key);
	bool Key_Up(int key);
	

	// CBase을(를) 통해 상속됨
	virtual void Free() override;

};

