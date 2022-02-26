#include "stdafx.h"
#include "..\Public\Scene_IMGUI.h"

#include "Player.h"
#include "MapLoadMgr.h"


CScene_IMGUI::CScene_IMGUI(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
	mbIsFrame = false;
#pragma region FOR IMGUI
	//GETIMGUI->Update_IMGUI_Start();
	//mbIsFrame = true;
	//GETIMGUI->Demo();
	//	GETIMGUI->Update_IMGUI_End();

	// 씬이 체인지 될때 Render가 먼저 실행되는 경우가 있어 체크를 해줘야한다.
	//if (mbIsFrame)
	//{
	//	GETIMGUI->Render_IMGUI();
	//	mbIsFrame = false;
	//}
#pragma endregion

}

HRESULT CScene_IMGUI::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;	

	// 배경 카메라 지형 생성

	// 임의 매니저 초기화
	GetSingle(CMapLoadMgr)->GetInstance();

	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_Player)));
	FAILED_CHECK(Ready_Layer_SkyBox(TEXT("Layer_SkyBox")));


	
	// 로드된 오브젝트 정보로 그리기
	GetSingle(CMapLoadMgr)->LoadMap(SCENEID::SCENE_IMGUISCENE,0);
	return S_OK;
}

_int CScene_IMGUI::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	

	return 0;

}

_int CScene_IMGUI::LateUpdate(_float fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;


	return 0;
}

_int CScene_IMGUI::Render()
{
	if (__super::Render() < 0)
		return -1;

	



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

HRESULT CScene_IMGUI::Ready_Layer_Player(const _tchar * pLayerTag)
{
	list<CGameObject*>* pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, pLayerTag);
	if (pPlayerList == nullptr)
	{
		if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, pLayerTag, TAG_OP(Prototype_Player)))
			return E_FAIL;

		GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, pLayerTag)->Set_NowSceneNum(SCENE_IMGUISCENE);
	}
	else
	{
		(pPlayerList->front())->ReInitialize(&_float3(0, 1, 0));

	}
	return S_OK;

}

HRESULT CScene_IMGUI::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, pLayerTag, TEXT("Prototype_GameObject_SkyBox")))
		return E_FAIL;

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
	GetSingle(CMapLoadMgr)->DestroyInstance();

	__super::Free();
}
