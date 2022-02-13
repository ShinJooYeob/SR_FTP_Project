#pragma once
#include "Base.h"

BEGIN(Engine)

// IMGUI ���� �Լ��� ���⼭ DLL�� �����ش�.
// ����� �޶� �� �Ŵ����� ���� ��� ����.
// CGameInstace���� ���⿡ �����ϴ� ���� �´ٰ� �Ǵ���.

class ENGINE_DLL CImguiMgr final : public CBase
{
	DECLARE_SINGLETON(CImguiMgr);

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
	// ������ ��� �Լ�ȭ
	// ���� ������ �߰�
	HRESULT MainFrame(const char* str);	 
	
	// �˾� �߰�
	HRESULT Popup(const char* str);

	// �ؽ�Ʈ �߰�
	HRESULT Text(const char* str);



private:

//	HRESULT CastingTchar2char(const _tchar* strtchar,const char* strchar);
//	_tchar*   Castingchar2Tchar(const char* str);

public:
	virtual void Free() override; // ��ġ�� �����ش�.
};

END
