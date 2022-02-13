#include "stdafx.h"
#include "..\Public\Scene_IMGUI.h"



CScene_IMGUI::CScene_IMGUI(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
	mbIsFrame = false;
}

HRESULT CScene_IMGUI::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	FAILED_CHECK(Ready_Layer_BackGround(StrLayer(LAYER_DEFAULT)));
	FAILED_CHECK(Ready_Layer_BackGround(StrLayer(LAYER_BACKGROUND)));

	return S_OK;
}

_int CScene_IMGUI::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	GETIMGUI->Update_IMGUI_Start();
	mbIsFrame = true;


//	GETIMGUI->Demo();
//	GETIMGUI->Demo_Test();
//	GETIMGUI->TestWindow1();
	GETIMGUI->TestWindow2();

	
	
//	GETIMGUI->Text("AAA");
//	GETIMGUI->Text("Go String");
//	GETIMGUI->Text("aaa");
	return 0;

}

_int CScene_IMGUI::LateUpdate(_float fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	GETIMGUI->Update_IMGUI_End();

	return 0;
}

_int CScene_IMGUI::Render()
{
	if (__super::Render() < 0)
		return -1;

	// 씬이 체인지 될때 Render가 먼저 실행되는 경우가 있어 체크를 해줘야한다.
	if (mbIsFrame)
	{
		GETIMGUI->Render_IMGUI();
		mbIsFrame = false;
	}
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("CScene_IMGUI"));
#endif // _DEBUG

	return 0;
}

_int CScene_IMGUI::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

HRESULT CScene_IMGUI::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
//	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_TerrainGround")))
//		return E_FAIL;

	return S_OK;
}

CScene_IMGUI * CScene_IMGUI::Create(LPDIRECT3DDEVICE9 GraphicDevice)
{
	CScene_IMGUI* pTempGame = new CScene_IMGUI(GraphicDevice);

	if (FAILED(pTempGame->Initialize()))
	{
		Safe_Release(pTempGame);
		MSGBOX("Failed to Creating CScene_IMGUI");
		return nullptr;
	}

	return pTempGame;
}

void CScene_IMGUI::Free()
{
	__super::Free();
}
