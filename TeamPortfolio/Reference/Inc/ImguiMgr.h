#pragma once
#include "Base.h"

BEGIN(Engine)

// IMGUI ���� �Լ��� ���⼭ DLL�� �����ش�.
// ����� �޶� �� �Ŵ����� ���� ��� ����.
// CGameInstace���� ���⿡ �����ϴ� ���� �´ٰ� �Ǵ���.

class ENGINE_DLL CImguiMgr final : public CBase
{
	DECLARE_SINGLETON(CImguiMgr);

// â �÷��� ����
enum ImGuiWindowFlags_Test_
{
	ImGuiWindowFlags_Test_None = 0,
	ImGuiWindowFlags_Test_ONE = 1 << 0,
	ImGuiWindowFlags_Test_TOW = 1 << 2,
	ImGuiWindowFlags_Test_3 = 1 << 3,
	ImGuiWindowFlags_Test_4 = 1 << 4,
	ImGuiWindowFlags_Test_5 = 1 << 5
};


protected:
	explicit CImguiMgr(); // ��ġ�� �Ѱ���� �����ȴ�.
	virtual ~CImguiMgr() = default;


public:
	// IMGUI �ʱ�ȭ
	HRESULT Initialize_IMGUI(HWND hwnd, LPDIRECT3DDEVICE9 device);

	// Update�� RenderBegin ~ End�� ����
	// Start UIUpdate
	HRESULT Update_IMGUI_Start();
	// End UIUpdate
	HRESULT Update_IMGUI_End();

	// IMGUI ������
	// Reder�� ȣ�� ��ġ�� ���� ������ ���⼭ �����Ѵ�.
	HRESULT Render_IMGUI();
	
	// IMGUI ���� �̺�Ʈ �ڵ鷯 
	// Ŭ���̾�Ʈ WinProc �Լ��� �����صд�.
	IMGUI_IMPL_API LRESULT  ImGui_Handle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	// ���� ���
	// �˾�â Cilcked
	HRESULT Popup(const char * maintitle, const char* clickmenu,const char* popupname);

	// �ؽ�Ʈ �߰�
	HRESULT Text(const char* str);

public:
	// ���� ���
	// ����ȭ��
	HRESULT Demo();
	HRESULT Demo_Wiget();

	HRESULT TestWindow1();
	HRESULT TestWindow2();

	HRESULT GameWindow();

	void SetViewport(HWND hwnd);

private:
	void SaveFunc();
	void SimpleLayout();
private:
	bool mDemo;

	bool mToolWindow;



//	HRESULT CastingTchar2char(const _tchar* strtchar,const char* strchar);
//	_tchar*   Castingchar2Tchar(const char* str);

public:
	void DestroyIMGUI();
	virtual void Free() override; // ��ġ�� �����ش�.
};

END
