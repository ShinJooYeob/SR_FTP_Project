#include "stdafx.h"
#include "..\Public\Scene_Stage_Boss.h"
#include "Scene_Loading.h"

#include "Camera_Main.h"
#include "MapLoadMgr.h"
#include "UI_Status.h"
#include "Object_PortalCube_A.h"
#include "Object_EscalatorCube.h"
#include "Object_OrbitButton.h"
#include "Object_Star.h"
#include "UI_Result.h"
#include "UI_BossStatusUI.h"

#include "ParsedObject_DeathSkull.h"


CScene_Stage_Boss::CScene_Stage_Boss(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
}

HRESULT CScene_Stage_Boss::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	// 보스스테이지 시작위치 0 10 0

	FAILED_CHECK(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main)));
	FAILED_CHECK(Ready_Layer_SkyBox(TEXT("Layer_SkyBox")));
	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_Player), _float3(0, 11, 0)));
	FAILED_CHECK(Ready_Layer_Monster(TAG_LAY(Layer_Monster)));

//	FAILED_CHECK(Ready_Layer_UI_Start(TEXT("Layer_UI_Start")));
//	FAILED_CHECK(Ready_Layer_PlayerStatusUI(TEXT("Layer_StatusUI")));

	list< SPECIALCUBE*> SpecialCubeList;
	GetSingle(CMapLoadMgr)->LoadMap(SCENEID::SCENE_BOSS, 5, &SpecialCubeList);

	FAILED_CHECK(Ready_Layer_Terrain(&SpecialCubeList));

	for (auto data : SpecialCubeList)
	{
		Safe_Delete(data);
	}
	SpecialCubeList.clear();
	// GetSingle(CGameInstance)->PlayBGM(L"JH_Stage3_BGM.mp3");
	GetSingle(CGameInstance)->PlayBGM(L"JH_Boss_BGM.mp3");

	/////////////////////보스 엔트리 테스트/////////////////////////////////////////////////////

	FAILED_CHECK(Ready_Layer_UI_Result(TEXT("Layer_UI_Result")));
	FAILED_CHECK(Ready_Layer_PauseUI(TEXT("Layer_PauseUI")));
	FAILED_CHECK(Ready_Layer_Object_BossStatusUI(TEXT("Layer_BossStatusUI")))
	FAILED_CHECK(Ready_Layer_Object_BossEntry(TEXT("Layer_BossEntry")));

	//////////////////////////////////////////////////////////////////////////

	Ready_Layer_Parsed(TEXT("Parsed"));

	

	return S_OK;
}

_int CScene_Stage_Boss::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;


	if (m_bScene_Switch == true)
	{
		switch (m_INextScene)
		{
		case SCENEID::SCENE_LOBY:
		{
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_LOBY), SCENEID::SCENE_LOADING)))
				return E_FAIL;
			break;
		}
		case SCENEID::SCENE_STAGESELECT:
		{
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGESELECT), SCENEID::SCENE_LOADING)))
				return E_FAIL;
			break;
		}
		case SCENEID::SCENE_BOSS:
		{
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_BOSS), SCENEID::SCENE_LOADING)))
				return E_FAIL;
			break;
		}
		}
	}

	return 0;
}

_int CScene_Stage_Boss::LateUpdate(_float fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	return 0;
}

_int CScene_Stage_Boss::Render()
{
	if (__super::Render() < 0)
		return -1;



//
//#ifdef _DEBUG
//	SetWindowText(g_hWnd, TEXT("GameScene"));
//#endif // _DEBUG
//

	return 0;
}

_int CScene_Stage_Boss::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

HRESULT CScene_Stage_Boss::Scene_InGame_Chage(_bool Scene_Chage_Switch, _uint _INextScene)
{
	m_bScene_Switch = Scene_Chage_Switch;
	m_INextScene = _INextScene;
	return S_OK;
}




HRESULT CScene_Stage_Boss::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.bIsOrtho = true;
	CameraDesc.vWorldRotAxis = _float3(0.f, 3.f, 0.f);
	CameraDesc.vAxisY = _float3(0, 1, 0);
	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fMovePerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	CCamera_Main* pMainCam = (CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));

	if (pMainCam == nullptr)
		return E_FAIL;

	if (FAILED(pMainCam->Reset_LookAtAxis(&CameraDesc)))
		return E_FAIL;

	_float3 ActionPos[1] = { _float3(0, 10.f, -2.f) };

	FAILED_CHECK(pMainCam->ReInitialize(ActionPos, 1));


	pMainCam->Set_NowSceneNum(SCENE_BOSS);
	pMainCam->CameraEffect(CCamera_Main::CAM_EFT_FADE_OUT, g_fDeltaTime, 4.f);

	return S_OK;
}

HRESULT CScene_Stage_Boss::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_BOSS, pLayerTag, TEXT("Prototype_GameObject_SkyBox")))
		return E_FAIL;

	return S_OK;


}

HRESULT CScene_Stage_Boss::Ready_Layer_UI_Result(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_BOSS, pLayerTag, TEXT("Prototype_GameObject_UI_Result")))
		return E_FAIL;


	CUI_Result* pResult = (CUI_Result*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_BOSS, TEXT("Layer_UI_Result")));
	if (pResult == nullptr)
		return E_FAIL;
	pResult->Set_MaxTime(300.f);

	return S_OK;
}

HRESULT CScene_Stage_Boss::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_BOSS, pLayerTag, TAG_OP(Prototype_Boss)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage_Boss::Ready_Layer_UI_Start(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_BOSS, pLayerTag, TEXT("Prototype_GameObject_UI_Start")))
		return E_FAIL;

	return S_OK;
}




HRESULT CScene_Stage_Boss::Ready_Layer_PlayerStatusUI(const _tchar * pLayerTag)
{
	list<CGameObject*>* pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, TAG_LAY(Layer_Player));

	NULL_CHECK_BREAK(pPlayerList);

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_BOSS, pLayerTag,TEXT("Prototype_GameObject_StatusUI")))
		return E_FAIL;

	CUI_Status* pStatusUI = (CUI_Status*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_BOSS, TEXT("Layer_StatusUI")));

	if (pStatusUI == nullptr)
		return E_FAIL;

	FAILED_CHECK(pStatusUI->Set_Player(pPlayerList->front()));


	CUI_Result* pResult = (CUI_Result*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_BOSS, TEXT("Layer_UI_Result")));
	if (pResult == nullptr)
		return E_FAIL;
	FAILED_CHECK(pStatusUI->Set_ResultUI(pResult));

	return S_OK;
}

HRESULT CScene_Stage_Boss::Ready_Layer_PauseUI(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_BOSS, pLayerTag, TEXT("Prototype_GameObject_PauseUI")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage_Boss::Ready_Layer_Parsed(const _tchar * pLayerTag)
{
	// 파서 초기화
	SCENEID id = SCENE_BOSS;

	list<CGameObject*>* ParserList = nullptr;

	int RandomInt = rand() % 100;


	for (int x=0;x<=10;x++)
	{
		_float3 TransformPos = _float3(x, 2.f, -1);
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_BOSS, pLayerTag, TEXT("Prototype_GameObject_DeathSkul"), &TransformPos));
	
		if (ParserList == nullptr)
		{
			ParserList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(id, pLayerTag);
	
		}
		CParsedObject_DeathSkull* obj = (CParsedObject_DeathSkull*)ParserList->back();
		obj->Get_Transform()->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(180));
	}
	
	for (int x = 0; x <= 10; x++)
	{
		_float3 TransformPos = _float3(x, 2.f, 2);
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_BOSS, pLayerTag, TEXT("Prototype_GameObject_DeathSkul"), &TransformPos));
	
		if (ParserList == nullptr)
		{
			ParserList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(id, pLayerTag);
	
		}
		//CParsedObject_DeathSkull* obj = (CParsedObject_DeathSkull*)ParserList->back();
		//obj->Get_Transform()->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(180));
	}

	for (int z = 0; z <= 1; z++)
	{
		_float3 TransformPos = _float3(-1, 2.f, z);
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_BOSS, pLayerTag, TEXT("Prototype_GameObject_DeathSkul"), &TransformPos));

		CParsedObject_DeathSkull* obj = (CParsedObject_DeathSkull*)ParserList->back();
		obj->Get_Transform()->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(-90));

		TransformPos = _float3(11, 2.f, z);
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_BOSS, pLayerTag, TEXT("Prototype_GameObject_DeathSkul"), &TransformPos));
		obj = (CParsedObject_DeathSkull*)ParserList->back();
		obj->Get_Transform()->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(90));
	}


	return S_OK;
}

HRESULT CScene_Stage_Boss::Ready_Layer_Player(const _tchar * pLayerTag, _float3 startPos)
{
	list<CGameObject*>* pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, pLayerTag);
	if (pPlayerList == nullptr)
	{
		if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, pLayerTag, TAG_OP(Prototype_Player)))
			return E_FAIL;

		GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, pLayerTag)->Set_NowSceneNum(SCENE_BOSS);
		pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, pLayerTag);
		(pPlayerList->front())->ReInitialize(&startPos);

	}
	else
	{
		(pPlayerList->front())->ReInitialize(&startPos);

	}


	return S_OK;
}

HRESULT CScene_Stage_Boss::Ready_Layer_Terrain(list<SPECIALCUBE*>* listdata)
{
	CObject_PortalCube_A::POTALDESC potalDesc;
	CObject_EscalatorCube::ESCALATORDESC escalDesc;


	potalDesc.iNowScene = SCENEID::SCENE_STAGE2;
	potalDesc.vPos_A_Cube = _float3(0, 0, 0);
	potalDesc.vPos_B_Cube = _float3(0, 0, 0);

	escalDesc.vStartPos = _float3(0, 0, 0);
	escalDesc.vEndPos = _float3(0, 0, 0);


	int count_Potal = 0;
	int count_Escalator = 0;

	for (auto data : *listdata)
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
				memcpy(&potalDesc.vPos_B_Cube, &(data->WorldMat.m[3]), sizeof(_float3));
				// 생성
				GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_STAGE2,
					/*TAG_LAY(Layer_Terrain)*/TEXT("Layer_Potal"), TAG_OP(Prototype_PortalCube_A),
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
				GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_STAGE2,
					TAG_LAY(Layer_Terrain), TAG_OP(Prototype_EscalatorCube),
					&escalDesc);

			}
			count_Escalator++;

		}

	}
	return S_OK;
}

HRESULT CScene_Stage_Boss::Ready_Layer_Object_BossEntry(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_BOSS, pLayerTag, TEXT("Prototype_GameObject_BossEntry")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage_Boss::Ready_Layer_Object_BossStatusUI(const _tchar * pLayerTag)
{
	list<CGameObject*>* pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, TAG_LAY(Layer_Player));

	NULL_CHECK_BREAK(pPlayerList);

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_BOSS, pLayerTag, TEXT("Prototype_GameObject_BossStatusUI")))
		return E_FAIL;
	
	CUI_BossStatusUI* pStatusUI = (CUI_BossStatusUI*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_BOSS, pLayerTag));

	if (pStatusUI == nullptr)
		return E_FAIL;

	FAILED_CHECK(pStatusUI->Set_Player(pPlayerList->front()));


	CUI_Result* pResult = (CUI_Result*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_BOSS, TEXT("Layer_UI_Result")));
	if (pResult == nullptr)
		return E_FAIL;
	FAILED_CHECK(pStatusUI->Set_ResultUI(pResult));

	return S_OK;
}

CScene_Stage_Boss * CScene_Stage_Boss::Create(LPDIRECT3DDEVICE9 GraphicDevice)
{
	CScene_Stage_Boss* pTempGame = new CScene_Stage_Boss(GraphicDevice);

	if (FAILED(pTempGame->Initialize()))
	{
		Safe_Release(pTempGame);
		MSGBOX("Failed to Creating SCENE_BOSS");
		return nullptr;
	}

	return pTempGame;
}

void CScene_Stage_Boss::Free()
{
	GetSingle(CMapLoadMgr)->DestroyInstance();

	__super::Free();
}
