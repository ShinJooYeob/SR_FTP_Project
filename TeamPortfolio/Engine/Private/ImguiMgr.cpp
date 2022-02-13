#include "..\Public\ImguiMgr.h"



IMPLEMENT_SINGLETON(CImguiMgr);

/*

struct IDirect3DDevice9;

IMGUI_IMPL_API bool     ImGui_ImplDX9_Init(IDirect3DDevice9* device);
IMGUI_IMPL_API void     ImGui_ImplDX9_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplDX9_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplDX9_RenderDrawData(ImDrawData* draw_data);

// Use if you want to reset your rendering device without losing Dear ImGui state.
IMGUI_IMPL_API bool     ImGui_ImplDX9_CreateDeviceObjects();
IMGUI_IMPL_API void     ImGui_ImplDX9_InvalidateDeviceObjects();

*/

CImguiMgr::CImguiMgr()
{
}

HRESULT CImguiMgr::Initialize_IMGUI(HWND hwnd, LPDIRECT3DDEVICE9 device)
{
	// IMGUI �ʱ�ȭ
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// IO ������ �׼���(���콺/Ű����/�����е� �Է�, �ð�, �پ��� ���� �ɼ�/�÷���
	// IMGUI_API ImGuiIO& GetIO(); 

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	// ���� �÷����� ���� �ٸ�
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(device);
	return S_OK;
}

HRESULT CImguiMgr::Update_IMGUI_Start()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	return S_OK;
}

HRESULT CImguiMgr::Update_IMGUI_End()
{
	ImGui::EndFrame();
	return S_OK;
}




//HRESULT CImguiMgr::Tick_IMGUI()
//{
//	// IMGUI ������ �ʱ�ȭ
//	ImGui_ImplDX9_NewFrame();
//	ImGui_ImplWin32_NewFrame();
//	ImGui::NewFrame();
//
//	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//	// ���� ���� ���� ����
//
//	if (ImGui::BeginMainMenuBar())
//	{
//		const _tchar* c = TEXT("Menu");
//		const char* cast = CastingTchar2char((c));
//		if (ImGui::BeginMenu(cast))
//		{
//			if (ImGui::MenuItem("menu item"))
//			{
//				mPopup = true;
//			}
//			ImGui::EndMenu();
//		}
//		ImGui::EndMainMenuBar();
//	}
//
//	if (mPopup)
//	{
//		ImGui::OpenPopup("popup");
//	}
//
//	if (ImGui::BeginPopupModal("popup"))
//	{
//		ImGui::Text("Lorem ipsum");
//		ImGui::EndPopup();
//	}
//
//
//	/*if (show_demo_window)
//	ImGui::ShowDemoWindow(&show_demo_window);*/
//
//	// ������ ����â
//
//		// �α� â 
//		// ���� ���̾ƿ�â (���� ������Ʈ�� �������)
//		// �Ӽ� ����â
//		// ��������â
//		// Ŀ���� ������ â
//
//	// ������ ����
//		// ���� �������� �ɵ�
//
//	/*
//	
//	// 2. ������ ��� ���� 
//	{
//		
//		static float f = 1.0f;
//		static int counter = 0;
//		static ImVec4 clear_color = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
//
//		// ���ο� ġ�� �����.
//		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//		// �ؽ�Ʈ �Է�
//		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//		
//		// üũ�ڽ�
//		ImGui::Checkbox("CheckBox", &show_another_window);      // Edit bools storing our window open/close state
//		
//		// �����̴� �ڽ�
//		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//		// ��ư
//		if (ImGui::Button("Button"))                       
//			counter++;
//
//		// ���� ���־� ����
//		ImGui::SameLine(counter);
//		
//		// �ؽ�Ʈ�� ���� �ֱ� 
//		ImGui::Text("counter = %d", counter);
////		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//
//		ImGui::End();
//	}
//	// �ٸ�â ����
//	if (show_another_window)
//	{
//		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
//		ImGui::Text("Hello from another window!");
//		if (ImGui::Button("Close Me"))
//			show_another_window = false;
//		ImGui::End();
//
//	}
//	*/
//
//
//	ImGui::EndFrame();
//
//	return S_OK;
//
//}

HRESULT CImguiMgr::Render_IMGUI()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	return S_OK;
}


HRESULT CImguiMgr::MainFrame(const char * str)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(str))
		{

		}
		ImGui::EndMainMenuBar();
	}
	return S_OK;
}

HRESULT CImguiMgr::Popup(const char * str)
{
	static bool mPopup1 = false;
	mPopup1 = !mPopup1;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::MenuItem("menu item"))
		{
			mPopup1 = true;
		}
		ImGui::EndMenu();
	}

	if (mPopup1)
	{
		ImGui::OpenPopup(str);
	}

	if (ImGui::BeginPopupModal("popup"))
	{
		ImGui::Text("lorem ipsum");
		ImGui::EndPopup();
	}

	return S_OK;
}

HRESULT CImguiMgr::Text(const char* str)
{

	ImGui::Text(str);
	return S_OK;

}


//HRESULT CastingTchar2char(const _tchar* strtchar, const char* strchar)
//{
//
//	// TCHAR -> char
//	const int len = 256;
//	char ctemp[len] = "";
//
//	// WideCharToMultiByte
//	WideCharToMultiByte(CP_ACP, 0, str, len, ctemp, len, NULL, NULL);
//	return S_OK;
//}

//_tchar * CImguiMgr::Castingchar2Tchar(const char * str)
//{
//
//	//char->TCHAR
//	const int len = 256;
//	TCHAR szUniCode[len] = L"";
//
//	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, len, szUniCode, 256);
//	return szUniCode;
//}

void CImguiMgr::Free()
{
	// ������ �����Լ��� ����.
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

IMGUI_IMPL_API LRESULT  CImguiMgr::ImGui_Handle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

