#pragma once
#include "Base.h"

BEGIN(Engine)

// IMGUI 제공 함수는 여기서 DLL로 열어준다.
// 기능이 달라서 이 매니저는 따로 열어서 관리.
// CGameInstace보단 여기에 정의하는 것이 맞다고 판단함.

class ENGINE_DLL CImguiMgr final : public CBase
{
	DECLARE_SINGLETON(CImguiMgr);

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
	// 쓸만한 기능 함수화
	// 메인 프레임 추가
	HRESULT MainFrame(const char* str);	 
	
	// 팝업 추가
	HRESULT Popup(const char* str);

	// 텍스트 추가
	HRESULT Text(const char* str);



private:

//	HRESULT CastingTchar2char(const _tchar* strtchar,const char* strchar);
//	_tchar*   Castingchar2Tchar(const char* str);

public:
	virtual void Free() override; // 장치를 지워준다.
};

END
