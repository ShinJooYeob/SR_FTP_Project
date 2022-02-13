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

// #Bug: 마우스 위치가 맞지 않음 

CImguiMgr::CImguiMgr()
{
}

HRESULT CImguiMgr::Initialize_IMGUI(HWND hwnd, LPDIRECT3DDEVICE9 device)
{
	// IMGUI 초기화
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// IO 구조에 액세스(마우스/키보드/게임패드 입력, 시간, 다양한 구성 옵션/플래그
	// IMGUI_API ImGuiIO& GetIO(); 

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	// 연결 플랫폼에 따라 다름
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(device);

	mDemo = true;
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



/*
// 2. 간단한 기능 기록
{

	static float f = 1.0f;
	static int counter = 0;
	static ImVec4 clear_color = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);

	// 새로운 치을 만든다.
	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	// 텍스트 입력
	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	// 체크박스
	ImGui::Checkbox("CheckBox", &show_another_window);      // Edit bools storing our window open/close state

	// 슬라이더 박스
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	// 버튼
	if (ImGui::Button("Button"))
		counter++;

	// 라인 비주얼 제어
	ImGui::SameLine(counter);

	// 텍스트에 변수 넣기
	ImGui::Text("counter = %d", counter);
	//		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}
// 다른창 생성
if (show_another_window)
{
	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImGui::Text("Hello from another window!");
	if (ImGui::Button("Close Me"))
		show_another_window = false;
	ImGui::End();

}
*/

HRESULT CImguiMgr::Render_IMGUI()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	return S_OK;
}


HRESULT CImguiMgr::Popup(const char * maintitle, const char* clickname, const char* popupname)
{
	// 보류

	//static bool mPopup1 = false;

	//if (ImGui::BeginMainMenuBar())
	//{
	//	if (ImGui::BeginMenu(maintitle))
	//	{
	//		if (ImGui::MenuItem(clickname))
	//		{
	//			mPopup1 = !mPopup1;
	//		}
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMainMenuBar();
	//}

	//if (mPopup1)
	//{
	//	ImGui::OpenPopup(popupname);
	//}

	//if (ImGui::BeginPopupModal(popupname))
	//{
	//	ImGui::Text("Text");
	//	ImGui::EndPopup();
	//}

	return S_OK;
}

HRESULT CImguiMgr::Text(const char* str)
{

	ImGui::Text(str);
	return S_OK;

}

HRESULT CImguiMgr::Demo()
{
	//// 타이틀바 예제 목록	

	//// 메인바
	//static bool show_app_main_menu_bar = false;
	//static bool show_app_documents = false;
	//// 콘솔
	//static bool show_app_console = false;
	//// 로그
	//static bool show_app_log = false;
	//static bool show_app_layout = false;
	//static bool show_app_property_editor = false;
	//static bool show_app_long_text = false;
	//static bool show_app_auto_resize = false;
	//static bool show_app_constrained_resize = false;
	//static bool show_app_simple_overlay = false;
	//static bool show_app_fullscreen = false;
	//static bool show_app_window_titles = false;
	//static bool show_app_custom_rendering = false;


	if (mDemo)
	{
		ImGui::ShowDemoWindow(&mDemo);
	}

	return S_OK;
}

//#define IMGUI_DEMO_MARKER(section)  do { if (GImGuiDemoMarkerCallback != NULL) GImGuiDemoMarkerCallback(__FILE__, __LINE__, section, GImGuiDemoMarkerCallbackUserData); } while (0)


HRESULT CImguiMgr::Demo_Test()
{
	// 데모에서 쓸만한것 간단한 코드로 만듬

#pragma region 타이틀바
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("(demo menu)", NULL, false, false);
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}			
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..")) {}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
#pragma endregion

#pragma region 파일입출력


#pragma endregion 


#pragma region 체크박스


#pragma endregion 


	return S_OK;

}

HRESULT CImguiMgr::ShowExampleAppMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
		//	ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	return S_OK;
}

HRESULT CImguiMgr::TestWindow1()
{
	static _int iData = 0;

	// 텍스트
	ImGui::Text("Hello, world %d", iData);
	// 버튼
	if (ImGui::Button("Save"))
		SaveFunc();
	
	// 입력 텍스트
	static char buf[256] = "";
	ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));

	// 슬라이더 iData와 데이터 연동
	ImGui::SliderInt("INT", &iData, 0, 1000);
	return S_OK;

}

HRESULT CImguiMgr::TestWindow2()
{
	// Create a window called "My First Tool", with a menu bar.
	static bool my_tool_active;
	ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// 메뉴와 메뉴키 지원
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Edit a color (stored as ~4 floats)
	static _float myColor[4] = { 0,0,0,0 };

	ImGui::ColorEdit4("Color", myColor);

	// 값 프레임으로 표시
	static float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
	ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

	// Display contents in a scrolling region
	// 텍스트 컬러색
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff"); 
	// 하위창을 바로 만듬
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; n++)
		ImGui::Text("%04d: Some text", n);
	// 종료 표시 필수
	ImGui::EndChild();
	ImGui::End();
	return S_OK;
}

void CImguiMgr::SaveFunc()
{
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
	// 삭제시 제공함수로 삭제.
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

IMGUI_IMPL_API LRESULT  CImguiMgr::ImGui_Handle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

