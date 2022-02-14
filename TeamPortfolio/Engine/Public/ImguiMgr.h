#pragma once
#include "Base.h"

BEGIN(Engine)

// IMGUI 제공 함수는 여기서 DLL로 열어준다.
// 기능이 달라서 이 매니저는 따로 열어서 관리.
// CGameInstace보단 여기에 정의하는 것이 맞다고 판단함.

class ENGINE_DLL CImguiMgr final : public CBase
{
	DECLARE_SINGLETON(CImguiMgr);

// 창 플래그 설정
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
	explicit CImguiMgr(); // 장치를 넘겨줘야 생성된다.
	virtual ~CImguiMgr() = default;


public:
	// IMGUI 초기화
	HRESULT Initialize_IMGUI(HWND hwnd, LPDIRECT3DDEVICE9 device);

	// Update도 RenderBegin ~ End의 개념
	// Start UIUpdate
	HRESULT Update_IMGUI_Start();
	// End UIUpdate
	HRESULT Update_IMGUI_End();

	// IMGUI 렌더링
	// Reder는 호출 위치가 같기 때문에 여기서 수행한다.
	HRESULT Render_IMGUI();
	
	// IMGUI 전용 이벤트 핸들러 
	// 클라이언트 WinProc 함수에 정의해둔다.
	IMGUI_IMPL_API LRESULT  ImGui_Handle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	// 개별 기능
	// 팝업창 Cilcked
	HRESULT Popup(const char * maintitle, const char* clickmenu,const char* popupname);

	// 텍스트 추가
	HRESULT Text(const char* str);

public:
	// 모음 기능
	// 데모화면
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
	virtual void Free() override; // 장치를 지워준다.
};

END
