#pragma once
#include "Include.h"
#include "Base.h"

// �̱������� �����
#define VK_MAX 255

class CKeyMgr : public CBase
{
private:
	bool mbKeyState[VK_MAX];

private:
	CKeyMgr();
	~CKeyMgr() = default;

public:
// �̱��� ����
	DECLARE_SINGLETON(CKeyMgr)

public:
	bool Init();
	// Ű ������ ����
	// Ű�Ŵ������� �ڱⰡ ����Ű�� ����ó���� �ؾ��Ѵ�.
	bool Key_Pressing(int key);
	bool Key_Down(int key);
	bool Key_Up(int key);
	

	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;

};

