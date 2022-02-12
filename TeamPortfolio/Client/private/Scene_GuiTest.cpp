#include "stdafx.h"
#include "..\Public\Scene_GuiTest.h"




CScene_GuiTest::CScene_GuiTest(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
	mPopUp = false;
}

HRESULT CScene_GuiTest::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	

	return S_OK;
}

_int CScene_GuiTest::Update(_float fDeltaTime)
{
	FAILED_UPDATE(__super::Update(fDeltaTime));


	TestGUI();

	return 0;
}

_int CScene_GuiTest::LateUpdate(_float fDeltaTime)
{
	FAILED_UPDATE(__super::LateUpdate(fDeltaTime));


	return 0;
}

_int CScene_GuiTest::Render()
{

	FAILED_UPDATE(__super::Render());


#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("IMGUISCENE"));
#endif // _DEBUG


	return 0;
}

_int CScene_GuiTest::LateRender()
{
	FAILED_UPDATE(__super::LateRender());

	return 0;
}

HRESULT CScene_GuiTest::TestGUI()
{
	// IMGUI 프레임 초기화
//	::ImGui_ImplDX9_NewFrame();
//	ImGui_ImplDX9_NewFrame();
//	ImGui_ImplWin32_NewFrame();
//	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	// 데모 예제 포럼 참고

	/*if (ImGui::BeginMainMenuBar())
	{
		const _tchar* c = TEXT("Menu");
		if (ImGui::BeginMenu("AAA"))
		{
			if (ImGui::MenuItem("menu item"))
			{
				mPopUp = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (mPopUp)
	{
		ImGui::OpenPopup("POPUP");
	}

	if (ImGui::BeginPopupModal("POPUP"))
	{
		ImGui::Text("Lorem ipsum");
		ImGui::EndPopup();
	}*/

	/*
	if (show_demo_window)
	ImGui::ShowDemoWindow(&show_demo_window);

	// 쓸만한 데모창

		// 로그 창 
		// 심플 레이아웃창 (더미 오브젝트가 들어있음)
		// 속성 수정창
		// 오버레이창
		// 커스텀 렌더링 창

	// 툴예제 참고
		// 위젯 예제보면 될듯

	
	
	// 2. 간단한 기능 데모 
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


//	ImGui::EndFrame();

	return S_OK;
}

CScene_GuiTest * CScene_GuiTest::Create(LPDIRECT3DDEVICE9 GraphicDevice)
{
	CScene_GuiTest* pTempGame = new CScene_GuiTest(GraphicDevice);

	if (FAILED(pTempGame->Initialize()))
	{
		Safe_Release(pTempGame);
		MSGBOX("Failed to Creating CScene_GuiTest");
		return nullptr;
	}

	return pTempGame;
}

void CScene_GuiTest::Free()
{
	__super::Free();
}
