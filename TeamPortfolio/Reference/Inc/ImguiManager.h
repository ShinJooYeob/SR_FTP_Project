#pragma once
#include "Base.h"

BEGIN(Engine)

// IMGUI ���� �Լ��� ���⼭ DLL�� �����ش�.
// ����� �޶� CGameInstace���� ���⿡ �����ϴ� ���� �´ٰ� �Ǵ���.

class CImguiManager final : public CBase
{
	DECLARE_SINGLETON(CImguiManager);

protected:
	explicit CImguiManager(); // ��ġ�� �Ѱ���� �����ȴ�.
	virtual ~CImguiManager() = default;


public:
	HRESULT InitIMGUI(HWND hwnd, LPDIRECT3DDEVICE9 device);

	// Ŭ���̾�Ʈ���� ������Ʈ�� �ٸ��� ������ ���� ���� �ʿ�
	//	HRESULT Tick_IMGUI();

	// Reder�� ȣ�� ��ġ�� ���� ������ ���⼭ �����Ѵ�.
	HRESULT Render_IMGUI();
	
// �ڵ鷯 �ʼ� 
	IMGUI_IMPL_API LRESULT  ImGui_Handle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	// ������ ��� �Լ�ȭ
	// ����� �������� ������

	HRESULT Text(const char* str);
private:


private:
	bool show_demo_window;
	bool show_another_window;
	bool mPopup;

public:
	virtual void Free() override; // ��ġ�� �����ش�.
};

END
