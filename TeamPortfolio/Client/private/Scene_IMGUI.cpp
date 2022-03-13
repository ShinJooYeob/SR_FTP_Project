#include "stdafx.h"
#include "..\Public\Scene_IMGUI.h"

#include "Player.h"
#include "MapLoadMgr.h"
#include "Object_PortalCube_A.h"
#include "Object_EscalatorCube.h"

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
	GetSingle(CGameInstance)->Add_GameObject_To_Layer(
		SCENEID::SCENE_IMGUISCENE,
		TAG_LAY(Layer_Terrain),
		TAG_OP(Prototype_TerrainCube),
		_float3(0, 0, 0));

	// 생성되지 않는 특수 큐브 저장
	list< SPECIALCUBE*> SpecialCubeList;
	GetSingle(CMapLoadMgr)->LoadMap(SCENEID::SCENE_IMGUISCENE,0, &SpecialCubeList);

	Ready_Layer_Terrain(&SpecialCubeList);

	for (auto data: SpecialCubeList)
	{
		Safe_Delete(data);
	}
	SpecialCubeList.clear();

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

	


//
//#ifdef _DEBUG
//	SetWindowText(g_hWnd, TEXT("CScene_IMGUI"));
//#endif // _DEBUG
//
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

HRESULT CScene_IMGUI::Ready_Layer_Terrain(list<SPECIALCUBE*>* listdata)
{
	CObject_PortalCube_A::POTALDESC potalDesc;
	CObject_EscalatorCube::ESCALATORDESC escalDesc;


	potalDesc.iNowScene = SCENEID::SCENE_IMGUISCENE;
	potalDesc.vPos_A_Cube = _float3(0, 0 ,0);
	potalDesc.vPos_B_Cube = _float3(0, 0, 0);

	escalDesc.vStartPos = _float3(0, 0, 0);
	escalDesc.vEndPos = _float3(0, 0, 0);


	int count_Potal = 0;
	int count_Escalator = 0;

	for (auto data: *listdata)
	{

		// 포탈 큐브 생성방식
		if (data->Tagname == TAG_OP(Prototype_PortalCube_A))
		{
			if (count_Potal % 2 == 0)
			{

				memcpy(&potalDesc.vPos_A_Cube, &(data->WorldMat.m[3]), sizeof(_float3));

			}
			else
			{
				memcpy(&potalDesc.vPos_B_Cube,&(data->WorldMat.m[3]),sizeof(_float3));
				// 생성
				GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_IMGUISCENE,
					TAG_LAY(Layer_Terrain), TAG_OP(Prototype_PortalCube_A),
					&potalDesc);

			}
			count_Potal++;

		}

		// 엘리베이터 생성방식
		if (data->Tagname == TAG_OP(Prototype_EscalatorCube))
		{

			if (count_Escalator % 2 == 0)
			{
				memcpy(escalDesc.vStartPos, &(data->WorldMat.m[3]), sizeof(_float3));

			}
			else
			{
				memcpy(escalDesc.vEndPos, &(data->WorldMat.m[3]), sizeof(_float3));
				// 생성
				GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_IMGUISCENE,
					TAG_LAY(Layer_Terrain), TAG_OP(Prototype_EscalatorCube),
					&escalDesc);
				
			}
			count_Escalator++;

		}
	}
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
