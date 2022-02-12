#pragma once
#include "Base.h"

BEGIN(Engine)

// IMGUI 제공 함수는 여기서 DLL로 열어준다.
// 기능이 달라서 CGameInstace보단 여기에 정의하는 것이 맞다고 판단함.

class CImguiManager final : public CBase
{
	DECLARE_SINGLETON(CImguiManager);

protected:
	explicit CImguiManager(); // 장치를 넘겨줘야 생성된다.
	virtual ~CImguiManager() = default;


public:
	HRESULT InitIMGUI(HWND hwnd, LPDIRECT3DDEVICE9 device);

	// 클라이언트마다 업데이트가 다르기 때문에 개별 구현 필요
	//	HRESULT Tick_IMGUI();

	// Reder는 호출 위치가 같기 때문에 여기서 수행한다.
	HRESULT Render_IMGUI();
	
// 핸들러 필수 
	IMGUI_IMPL_API LRESULT  ImGui_Handle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	// 쓸만한 기능 함수화
	// 기능좀 본다음에 생각함

	HRESULT Text(const char* str);
private:


private:
	bool show_demo_window;
	bool show_another_window;
	bool mPopup;

public:
	virtual void Free() override; // 장치를 지워준다.
};

END
