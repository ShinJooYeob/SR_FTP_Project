#include "stdafx.h"
#include "..\public\Scene_Tutorial.h"
#include "Scene_Loading.h"

#include "Camera_Main.h"
#include "MapLoadMgr.h"
#include "UI_Status.h"
#include "Object_PortalCube_A.h"
#include "Object_EscalatorCube.h"
#include "Object_OrbitButton.h"
#include "..\public\UI_Result.h"
#include "..\public\Object_Star.h"
#include "Npc.h"

CScene_Tutorial::CScene_Tutorial(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
}

HRESULT CScene_Tutorial::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main))))
		return E_FAIL;


	if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Player(TAG_LAY(Layer_Player))))
		return E_FAIL;
	if (FAILED(Ready_Layer_PauseUI(TEXT("Layer_PauseUI"))))
		return E_FAIL;

	FAILED_CHECK(Ready_Layer_UI_Result(TEXT("Layer_UI_Result")));
	FAILED_CHECK(Ready_Layer_UI_Start(TEXT("Layer_UI_Start")));
	FAILED_CHECK(Ready_Layer_Object_Star(TEXT("Layer_Object_Star")));
	FAILED_CHECK(Ready_Layer_PlayerStatusUI(TEXT("Layer_StatusUI")));
	FAILED_CHECK(Ready_Layer_StageEndCollsionObject(TEXT("Layer_Collision_StageEnd")));
	FAILED_CHECK(Ready_Layer_NPC(TEXT("Layer_NPC")));
	

	//////////////////맵 튜토리얼에 쓰임
	FAILED_CHECK(Ready_Layer_OrbitButton_And_Cube(TEXT("Layer_OrbitButton_And_Cube")));
	if (FAILED(Ready_Layer_Object_PortalCube(TEXT("Layer_PotalCube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_GravityCube(TEXT("Layer_GravityCube"))))
		return E_FAIL;
	///////////////////

	// 로드된 오브젝트 정보로 그리기
	//GetSingle(CGameInstance)->Add_GameObject_To_Layer(
	//	SCENEID::SCENE_TUTORIAL,
	//	TAG_LAY(Layer_Terrain),
	//	TAG_OP(Prototype_TerrainCube),
	//	_float3(0, 0, 0));

	// 생성되지 않는 특수 큐브 저장
	list< SPECIALCUBE*> SpecialCubeList;
	GetSingle(CMapLoadMgr)->LoadMap(SCENEID::SCENE_TUTORIAL, 4, &SpecialCubeList);

	FAILED_CHECK(Ready_Layer_Terrain(&SpecialCubeList));

	for (auto data : SpecialCubeList)
	{
		Safe_Delete(data);
	}
	SpecialCubeList.clear();


	//플레이 브금 바꾸기
	FAILED_CHECK(GetSingle(CGameInstance)->PlayBGM((L"EH_Asteria.mp3")));



	_float3 TransformPos;

	//TransformPos = _float3(0.f, 0.f, 0.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_Alien", TEXT("Prototype_GameObject_Alien"), &TransformPos));

	//TransformPos = _float3(0.f, 0.f, 0.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_TreeOfDeath", TEXT("Prototype_GameObject_TreeOfDeath"), &TransformPos));



	//TransformPos = _float3(0, 0, 0);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_BigOwl", TEXT("Prototype_GameObject_BigOwl"), &TransformPos));
	//TransformPos = _float3(0, 0, 0);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_BigOwlHead", TEXT("Prototype_GameObject_BigOwlHead"), &TransformPos));







	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_Blossoms", TEXT("Prototype_GameObject_Blossoms"),&TransformPos));
	
	//TransformPos = _float3(1.f, 0.f, 0.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_Penguin", TEXT("Prototype_GameObject_Penguin"), &TransformPos));

	//TransformPos = _float3(8.f, 0.f, 0.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_BigGreenTree", TEXT("Prototype_GameObject_BigGreenTree"), &TransformPos));

	//TransformPos = _float3(1.f, 0.f, 4.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_TreasureBox", TEXT("Prototype_GameObject_TreasureBox"), &TransformPos));

	//TransformPos = _float3(2.f, 0.f, 4.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_DeathSkull", TEXT("Prototype_GameObject_DeathSkull"), &TransformPos));



	//TransformPos = _float3(4.f, 0.f, 0.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_Bell", TEXT("Prototype_GameObject_Bell"), &TransformPos));

	//TransformPos = _float3(4.f, 0.f, 0.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_LightHouse", TEXT("Prototype_GameObject_LightHouse"), &TransformPos));

	//TransformPos = _float3(4.f, 0.f, 0.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_MiniTree", TEXT("Prototype_GameObject_MiniTree"), &TransformPos));

	//TransformPos = _float3(4.f, 0.f, 0.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_ZuHeadWhite", TEXT("Prototype_GameObject_ZuHeadWhite"), &TransformPos));

	//풍차 종류 2개 만드려는중
	//TransformPos = _float3(4.f, 0.f, 0.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_WindmillBlue", TEXT("Prototype_GameObject_WindmillBlue"), &TransformPos));
	
	//TransformPos = _float3(4.f, 0.f, 0.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_WindmillBlue", TEXT("Prototype_GameObject_BigWindmill"), &TransformPos));

	//TransformPos = _float3(4.f, 0.f, 0.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_WindmillStick", TEXT("Prototype_GameObject_WindmillStick"), &TransformPos));


	//TransformPos = _float3(4.f, 0.f, 0.f);
	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_MapleTree", TEXT("Prototype_GameObject_MapleTree"), &TransformPos));


	return S_OK;
}

_int CScene_Tutorial::Update(_float fDeltaTime)
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
		case SCENEID::SCENE_TUTORIAL:
		{
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_TUTORIAL), SCENEID::SCENE_LOADING)))
				return E_FAIL;
			break;
		}

		}
	}


	return 0;
}

_int CScene_Tutorial::LateUpdate(_float fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	return 0;
}

_int CScene_Tutorial::Render()
{
	if (__super::Render() < 0)
		return -1;




//#ifdef _DEBUG
//	SetWindowText(g_hWnd, TEXT("GameScene"));
//#endif // _DEBUG
//
//
	return 0;
}

_int CScene_Tutorial::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

HRESULT CScene_Tutorial::Scene_InGame_Chage(_bool Scene_Chage_Switch, _uint _INextScene)
{
	m_bScene_Switch = Scene_Chage_Switch;
	m_INextScene = _INextScene;

	return S_OK;
}




HRESULT CScene_Tutorial::Ready_Layer_MainCamera(const _tchar * pLayerTag)
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

	_float3 ActionPos[5] = { _float3(11.f, 4.f, 11.f) ,_float3(20.f, 1.f, 20.f) ,_float3(37.f, 1.f, 36.f),_float3(19.f, 25.f, 76.f),_float3(-1.f, 22.f, 75.f)};

	FAILED_CHECK(pMainCam->ReInitialize(ActionPos, 5))


	pMainCam->Set_NowSceneNum(SCENE_TUTORIAL);
	pMainCam->CameraEffect(CCamera_Main::CAM_EFT_FADE_OUT, g_fDeltaTime, 4.f);

	return S_OK;
}
HRESULT CScene_Tutorial::Ready_Layer_NPC(const _tchar * pLayerTag)
{
	CNpc::NPCDESC NpcDesc;
//fixed설명충
	NpcDesc.vPos = _float3(10.f, 1.f, 10.f);
	NpcDesc.vDir = _float3(1.f, 0.f, 0.f);
	NpcDesc.pNpcTextureName = TEXT("izaac_Idle");
	NpcDesc.eCubeInfo = E_CUBEID::CUBEID_FIXED;
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Npc_izaacTuto"), (void*)&NpcDesc))
		return E_FAIL;
//appear설명충
	NpcDesc.vPos = _float3(19.f, 1.f, 19.f);
	NpcDesc.vDir = _float3(1.f, 0.f, 0.f);
	NpcDesc.pNpcTextureName = TEXT("izaac_Idle");
	NpcDesc.eCubeInfo = E_CUBEID::CUBEID_APPEAR;
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Npc_izaacTuto"), (void*)&NpcDesc))
		return E_FAIL;
//elevator설명충
	NpcDesc.vPos = _float3(36.f, 1.f, 36.f);
	NpcDesc.vDir = _float3(1.f, 0.f, 0.f);
	NpcDesc.pNpcTextureName = TEXT("izaac_Idle");
	NpcDesc.eCubeInfo = E_CUBEID::CUBEID_ELEVETOR;
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Npc_izaacTuto"), (void*)&NpcDesc))
		return E_FAIL;
//orbit설명충
	NpcDesc.vPos = _float3(49.f, 11.f, 64.f);
	NpcDesc.vDir = _float3(1.f, 0.f, 0.f);
	NpcDesc.pNpcTextureName = TEXT("izaac_Idle");
	NpcDesc.eCubeInfo = E_CUBEID::CUBEID_ORBIT;
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Npc_izaacTuto"), (void*)&NpcDesc))
		return E_FAIL;
//gravity설명충
	NpcDesc.vPos = _float3(21.f, 25.f, 74.f);
	NpcDesc.vDir = _float3(1.f, 0.f, 0.f);
	NpcDesc.pNpcTextureName = TEXT("izaac_Idle");
	NpcDesc.eCubeInfo = E_CUBEID::CUBEID_GRAVITY;
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Npc_izaacTuto"), (void*)&NpcDesc))
		return E_FAIL;
//portal설명충
	NpcDesc.vPos = _float3(16.f, 18.f, 74.f);
	NpcDesc.vDir = _float3(1.f, 0.f, 0.f);
	NpcDesc.pNpcTextureName = TEXT("izaac_Idle");
	NpcDesc.eCubeInfo = E_CUBEID::CUBEID_POTAL;
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Npc_izaacTuto"), (void*)&NpcDesc))
		return E_FAIL;
//vanish설명충
	NpcDesc.vPos = _float3(7.f, 25.f, 88.f);
	NpcDesc.vDir = _float3(1.f, 0.f, 0.f);
	NpcDesc.pNpcTextureName = TEXT("izaac_Idle");
	NpcDesc.eCubeInfo = E_CUBEID::CUBEID_VANISH;
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Npc_izaacTuto"), (void*)&NpcDesc))
		return E_FAIL;

	return S_OK;


}
HRESULT CScene_Tutorial::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{


	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_SkyBox")))
		return E_FAIL;

	return S_OK;


}

HRESULT CScene_Tutorial::Ready_Layer_UI_Result(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UI_Result")))
		return E_FAIL;

	CUI_Result* pResult = (CUI_Result*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_TUTORIAL, TEXT("Layer_UI_Result")));
	if (pResult == nullptr)
		return E_FAIL;
	pResult->Set_MaxTime(300.f); //MAX시간을 나타냄


	return S_OK;
}

HRESULT CScene_Tutorial::Ready_Layer_PlayerStatusUI(const _tchar * pLayerTag)
{
	list<CGameObject*>* pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, TAG_LAY(Layer_Player));

	NULL_CHECK_BREAK(pPlayerList);

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_StatusUI")))
		return E_FAIL;

	CUI_Status* pStatusUI = (CUI_Status*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_TUTORIAL, TEXT("Layer_StatusUI")));

	if (pStatusUI == nullptr)
		return E_FAIL;

	FAILED_CHECK(pStatusUI->Set_Player(pPlayerList->front()));


	CUI_Result* pResult = (CUI_Result*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_TUTORIAL, TEXT("Layer_UI_Result")));
	if (pResult == nullptr)
		return E_FAIL;
	FAILED_CHECK(pStatusUI->Set_ResultUI(pResult));

	return S_OK;
}

HRESULT CScene_Tutorial::Ready_Layer_UI_Start(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_UI_Start")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Tutorial::Ready_Layer_Object_Star(const _tchar * pLayerTag)
{
	CObject_Star::STARDESC StarDesc;

	//////튜토리얼에 쓰임
	StarDesc.fTransform = _float3(11.f, 4.f, 11.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;
	StarDesc.fTransform = _float3(20.f, 1.f, 20.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;
	StarDesc.fTransform = _float3(37.f, 1.f, 36.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;
	StarDesc.fTransform = _float3(19.f, 25.f, 76.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;
	StarDesc.fTransform = _float3(-1.f, 22.f, 75.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Tutorial::Ready_Layer_OrbitButton_And_Cube(const _tchar * pLayerTag)
{

	CObject_OrbitButton::ORBITDESC tDesc;

	tDesc.vButtonPos = _float3(48, 11, 64);
	tDesc.vOrbitRotAxis = _float3(46, 11, 64);
	tDesc.vOribitCubeStartPos = _float3(40, 11, 64);
	tDesc.vOribitTotalXYZ = _float3(1, 1, 4);

	//tDesc.vButtonPos = _float3(-19, 12, 13);
	//tDesc.vOrbitRotAxis = _float3(-19, -2, -2);
	//tDesc.vOribitCubeStartPos = _float3(-19, 9, 16);
	//tDesc.vOribitTotalXYZ = _float3(1, 1, 3);


	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Object_OrbitButton"), &tDesc))
		return E_FAIL;

	//tDesc.vButtonPos = _float3(-19, 12, 13);
	//tDesc.vOrbitRotAxis = _float3(-19, -2, -2);
	//tDesc.vOribitCubeStartPos = _float3(-19, 9, 16);
	//tDesc.vOribitTotalXYZ = _float3(1, 1, 3);


	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_Object_OrbitButton"), &tDesc))
	//	return E_FAIL;



	return S_OK;
}

HRESULT CScene_Tutorial::Ready_Layer_GravityCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, TEXT("Layer_GravityCube"), TEXT("Prototype_GameObject_Object_GravityCube"), &_float3((_float)16, (_float)25, (_float)78)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Tutorial::Ready_Layer_Object_PortalCube(const _tchar * pLayerTag)
{
	CObject_PortalCube_A::POTALDESC tagDesc;

	tagDesc.vPos_A_Cube = _float3(13, 19, 74);
	tagDesc.vPos_B_Cube = _float3(12, 26, 83);
	tagDesc.iNowScene = SCENEID::SCENE_TUTORIAL;

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, TEXT("Layer_Potal"), TEXT("Prototype_GameObject_Object_PortalCube_A"), &tagDesc))
		return E_FAIL;

	return S_OK;
}



HRESULT CScene_Tutorial::Ready_Layer_StageEndCollsionObject(const _tchar * pLayerTag)
{
	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object"), &_float3(23.f, 64.f, -4.f)))
	//	return E_FAIL;

	//튜토리얼에 쓰임
	_float3 TransformPos = _float3(-3.f, 22.f, 75.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, L"Layer_TreasureBox", TEXT("Prototype_GameObject_TreasureBox"), &TransformPos));



	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object"), &_float3(-3.f, 22.f, 75.f)))
		return E_FAIL;
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object"), &_float3(-3.f, 22.f, 76.f)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Tutorial::Ready_Layer_PauseUI(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_TUTORIAL, pLayerTag, TEXT("Prototype_GameObject_PauseUI")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Tutorial::Ready_Layer_Player(const _tchar * pLayerTag)
{
	list<CGameObject*>* pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, pLayerTag);
	if (pPlayerList == nullptr)
	{
		if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, pLayerTag, TAG_OP(Prototype_Player)))
			return E_FAIL;

		GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, pLayerTag)->Set_NowSceneNum(SCENE_TUTORIAL);
		pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, pLayerTag);
	}
	else
	{
		(pPlayerList->front())->ReInitialize(&_float3(0.f, 1.f, 0));

	}


	return S_OK;
}

HRESULT CScene_Tutorial::Ready_Layer_Terrain(list<SPECIALCUBE*>* listdata)
{
	CObject_PortalCube_A::POTALDESC potalDesc;
	CObject_EscalatorCube::ESCALATORDESC escalDesc;

	potalDesc.iNowScene = SCENEID::SCENE_TUTORIAL;
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
				GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_TUTORIAL,
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
				GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_TUTORIAL,
					TAG_LAY(Layer_Terrain), TAG_OP(Prototype_EscalatorCube),
					&escalDesc);

			}
			count_Escalator++;

		}

	}
	return S_OK;
}



CScene_Tutorial * CScene_Tutorial::Create(LPDIRECT3DDEVICE9 GraphicDevice)
{
	CScene_Tutorial* pTempGame = new CScene_Tutorial(GraphicDevice);

	if (FAILED(pTempGame->Initialize()))
	{
		Safe_Release(pTempGame);
		MSGBOX("Failed to Creating Scene_Tutorial");
		return nullptr;
	}

	return pTempGame;
}

void CScene_Tutorial::Free()
{
	GetSingle(CMapLoadMgr)->DestroyInstance();

	__super::Free();
}
