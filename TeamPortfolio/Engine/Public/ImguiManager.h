#pragma once
#include "Base.h"

BEGIN(Engine)

// IMGUI ���� �Լ��� ���⼭ DLL�� �����ش�.
// ����� �޶� �� �Ŵ����� ���� ��� ����.
// CGameInstace���� ���⿡ �����ϴ� ���� �´ٰ� �Ǵ���.

class ENGINE_DLL CImguiManager final : public CBase
{
	DECLARE_SINGLETON(CImguiManager);

protected:
	explicit CImguiManager(); // ��ġ�� �Ѱ���� �����ȴ�.
	virtual ~CImguiManager() = default;


public:
	// IMGUI �ʱ�ȭ
	HRESULT InitIMGUI(HWND hwnd, LPDIRECT3DDEVICE9 device);

	// Ŭ���̾�Ʈ���� ������Ʈ�� �ٸ��� ������ ���� ���� �ʿ�
	//	HRESULT Tick_IMGUI();

	// IMGUI ������
	// Reder�� ȣ�� ��ġ�� ���� ������ ���⼭ �����Ѵ�.
	HRESULT Render_IMGUI();
	
	// IMGUI ���� �̺�Ʈ �ڵ鷯 
	// Ŭ���̾�Ʈ WinProc �Լ��� �����صд�.
	IMGUI_IMPL_API LRESULT  ImGui_Handle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	// ������ ��� �Լ�ȭ
	HRESULT Text(const char* str);	// �ؽ�Ʈ ��� / 
private:

//	HRESULT CastingTchar2char(const _tchar* strtchar,const char* strchar);
//	_tchar*   Castingchar2Tchar(const char* str);

public:
	virtual void Free() override; // ��ġ�� �����ش�.
};

END
