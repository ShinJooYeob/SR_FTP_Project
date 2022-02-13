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
	// IMGUI ������ �ʱ�ȭ
//	::ImGui_ImplDX9_NewFrame();
//	ImGui_ImplDX9_NewFrame();
//	ImGui_ImplWin32_NewFrame();
//	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	// ���� ���� ���� ����

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

	// ������ ����â

		// �α� â 
		// ���� ���̾ƿ�â (���� ������Ʈ�� �������)
		// �Ӽ� ����â
		// ��������â
		// Ŀ���� ������ â

	// ������ ����
		// ���� �������� �ɵ�

	
	
	// 2. ������ ��� ���� 
	{
		
		static float f = 1.0f;
		static int counter = 0;
		static ImVec4 clear_color = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);

		// ���ο� ġ�� �����.
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		// �ؽ�Ʈ �Է�
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		
		// üũ�ڽ�
		ImGui::Checkbox("CheckBox", &show_another_window);      // Edit bools storing our window open/close state
		
		// �����̴� �ڽ�
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		// ��ư
		if (ImGui::Button("Button"))                       
			counter++;

		// ���� ���־� ����
		ImGui::SameLine(counter);
		
		// �ؽ�Ʈ�� ���� �ֱ� 
		ImGui::Text("counter = %d", counter);
//		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
	}
	// �ٸ�â ����
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
