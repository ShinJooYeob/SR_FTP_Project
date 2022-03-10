#include "stdafx.h"
#include "..\Public\Scene_Stage1.h"
#include "Scene_Loading.h"

#include "Camera_Main.h"
#include "MapLoadMgr.h"
#include "UI_Status.h"
#include "Object_PortalCube_A.h"
#include "Object_EscalatorCube.h"
#include "Object_OrbitButton.h"
#include "..\public\UI_Result.h"
#include "..\public\Object_Star.h"

#include "ParsedObject_BigWindmill.h"
#include "ParsedObject_WindmillBlue.h"


CScene_Stage1::CScene_Stage1(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
}

HRESULT CScene_Stage1::Initialize()
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

	//FAILED_CHECK(Ready_Layer_OrbitButton_And_Cube(TEXT("Layer_OrbitButton")));
	FAILED_CHECK(Ready_Layer_UI_Result(TEXT("Layer_UI_Result")));
	FAILED_CHECK(Ready_Layer_UI_Start(TEXT("Layer_UI_Start")));
	FAILED_CHECK(Ready_Layer_Object_Star(TEXT("Layer_Object_Star")));
	FAILED_CHECK(Ready_Layer_PlayerStatusUI(TEXT("Layer_StatusUI")));
	FAILED_CHECK(Ready_Layer_StageEndCollsionObject(TEXT("Layer_Collision_StageEnd")));


	// 로드된 오브젝트 정보로 그리기
	GetSingle(CGameInstance)->Add_GameObject_To_Layer(
		SCENEID::SCENE_STAGE1,
		TAG_LAY(Layer_Terrain),
		TAG_OP(Prototype_TerrainCube),
		_float3(0, 0, 0));

	// 생성되지 않는 특수 큐브 저장
	list< SPECIALCUBE*> SpecialCubeList;
	GetSingle(CMapLoadMgr)->LoadMap(SCENEID::SCENE_STAGE1,1, &SpecialCubeList);

	FAILED_CHECK(Ready_Layer_Terrain(&SpecialCubeList));

	for (auto data : SpecialCubeList)
	{
		Safe_Delete(data);
	}
	SpecialCubeList.clear();


	FAILED_CHECK(GetSingle(CGameInstance)->PlayBGM((L"EH_Spiral_of_Secrets.mp3")));


	_float3 TransformPos = _float3(22.7f, 67.5f, -2.2f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, L"Layer_Blossoms", TEXT("Prototype_GameObject_Blossoms"),&TransformPos));

	Set_WindmillBlue();
	Set_BigWindmill();
	
	return S_OK;
}

_int CScene_Stage1::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;


	//if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_RETURN) & DIS_Down) {
	//	if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGESELECT), SCENEID::SCENE_LOADING)))
	//		return E_FAIL;
	//}

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
		case SCENEID::SCENE_STAGE1:
		{
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE1), SCENEID::SCENE_LOADING)))
				return E_FAIL;
			break;
		}

		}
	}


	return 0;
}

_int CScene_Stage1::LateUpdate(_float fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	return 0;
}

_int CScene_Stage1::Render()
{
	if (__super::Render() < 0)
		return -1;




#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("GameScene"));
#endif // _DEBUG


	return 0;
}

_int CScene_Stage1::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

HRESULT CScene_Stage1::Scene_InGame_Chage(_bool Scene_Chage_Switch, _uint _INextScene)
{
	m_bScene_Switch = Scene_Chage_Switch;
	m_INextScene = _INextScene;

	return S_OK;
}




HRESULT CScene_Stage1::Ready_Layer_MainCamera(const _tchar * pLayerTag)
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

	_float3 ActionPos[5] = { _float3(10.f, 40.f, 10.f) ,_float3(20.f, 42.f, -1.f) ,_float3(35.f, 46.f, -32.f),_float3(62.f, 60.f, 65.f),_float3(23.f, 63.f, -5.f) };

	FAILED_CHECK(pMainCam->ReInitialize(ActionPos,5))


	pMainCam->Set_NowSceneNum(SCENE_STAGE1);
	pMainCam->CameraEffect(CCamera_Main::CAM_EFT_FADE_OUT, g_fDeltaTime,4.f);

	return S_OK;
}

HRESULT CScene_Stage1::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{


	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("Prototype_GameObject_SkyBox")))
		return E_FAIL;

	return S_OK;


}

HRESULT CScene_Stage1::Ready_Layer_UI_Result(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("Prototype_GameObject_UI_Result")))
		return E_FAIL;	
	CUI_Result* pResult = (CUI_Result*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGE1, TEXT("Layer_UI_Result")));
	if (pResult == nullptr)
		return E_FAIL;
	pResult->Set_MaxTime(300.f); //MAX시간을 나타냄


	return S_OK;
}

HRESULT CScene_Stage1::Ready_Layer_PlayerStatusUI(const _tchar * pLayerTag)
{
	list<CGameObject*>* pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, TAG_LAY(Layer_Player));

	NULL_CHECK_BREAK(pPlayerList);

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_STAGE1, pLayerTag, TEXT("Prototype_GameObject_StatusUI")))
		return E_FAIL;

	CUI_Status* pStatusUI = (CUI_Status*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGE1, TEXT("Layer_StatusUI")));

	if (pStatusUI == nullptr)
		return E_FAIL;

	FAILED_CHECK(pStatusUI->Set_Player(pPlayerList->front()));


	CUI_Result* pResult = (CUI_Result*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGE1, TEXT("Layer_UI_Result")));
	if (pResult == nullptr)
		return E_FAIL;
	FAILED_CHECK(pStatusUI->Set_ResultUI(pResult));

	return S_OK;
}

HRESULT CScene_Stage1::Ready_Layer_UI_Start(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("Prototype_GameObject_UI_Start")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage1::Ready_Layer_Object_Star(const _tchar * pLayerTag)
{
	CObject_Star::STARDESC StarDesc;

	////테스트중
	//StarDesc.fTransform = _float3(2.f, 1.f, 0.f);
	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
	//	return E_FAIL;

	//StarDesc.fTransform = _float3(4.f, 1.f, 0.f);
	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
	//	return E_FAIL;

	//StarDesc.fTransform = _float3(5.f, 1.f, 0.f);
	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
	//	return E_FAIL;

	//StarDesc.fTransform = _float3(6.f, 1.f, 0.f);
	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
	//	return E_FAIL;

	//StarDesc.fTransform = _float3(3.f, 1.f, 0.f);
	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
	//	return E_FAIL;

	StarDesc.fTransform = _float3(10.f, 40.f, 10.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	StarDesc.fTransform = _float3(20.f, 42.f, -1.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	StarDesc.fTransform = _float3(35.f, 46.f, -32.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	StarDesc.fTransform = _float3(62.f, 60.f, 65.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	StarDesc.fTransform = _float3(23.f, 63.f, -5.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage1::Ready_Layer_StageEndCollsionObject(const _tchar * pLayerTag)
{
	//_float3(21.f, 64.f, -3.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object"), &_float3(23.f, 63.f, -5.f)))
		return E_FAIL;

	//튜토리얼에 쓰임
	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object"), &_float3(0.f, 22.f, 75.f)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage1::Set_WindmillBlue()
{
	CParsedObject_WindmillBlue::WINDMILLBLUEDESC WindmillBlueDesc;
	//막대 2개로 y는 + 2
	//풍차는 최종 막대 + 1시켜줄 것 또한 z축은 -2

	_float3 TransformPos;

	WindmillBlueDesc.fTransform = _float3(4.f, 3.f, -2.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, L"Layer_WindmillBlue", TEXT("Prototype_GameObject_WindmillBlue"), &WindmillBlueDesc));

	TransformPos = _float3(4.f, 2.f, 0.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, L"Layer_WindmillStick", TEXT("Prototype_GameObject_WindmillStick"), &TransformPos));

	return S_OK;
}
HRESULT CScene_Stage1::Set_BigWindmill()
{
	CParsedObject_BigWindmill::BIGWINDMILLDESC BigWindmillDesc;
	////막대 2개로 y는 + 6
	////풍차는 최종 막대 + 1시켜줄 것 또한 z축은 -4
	_float3 TransformPos;

	BigWindmillDesc.Transform = _float3(4.f, 7.f, -4.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, L"Layer_BigWindmill", TEXT("Prototype_GameObject_BigWindmill"), &BigWindmillDesc));

	TransformPos = _float3(4.f, 2.f, 0.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, L"Layer_WindmillStick", TEXT("Prototype_GameObject_WindmillStick"), &TransformPos));
	TransformPos = _float3(4.f, 6.f, 0.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, L"Layer_WindmillStick", TEXT("Prototype_GameObject_WindmillStick"), &TransformPos));



	return S_OK;
}

HRESULT CScene_Stage1::Ready_Layer_PauseUI(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE1, pLayerTag, TEXT("Prototype_GameObject_PauseUI")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage1::Ready_Layer_Player(const _tchar * pLayerTag)
{
	list<CGameObject*>* pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, pLayerTag);
	if (pPlayerList == nullptr)
	{
		if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, pLayerTag, TAG_OP(Prototype_Player)))
			return E_FAIL;

		GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, pLayerTag)->Set_NowSceneNum(SCENE_STAGE1);
		pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, pLayerTag);
	}
	else
	{
		(pPlayerList->front())->ReInitialize(&_float3(0.f, 1.f, 0));

	}


	return S_OK;
}

HRESULT CScene_Stage1::Ready_Layer_Terrain(list<SPECIALCUBE*>* listdata)
{
	CObject_PortalCube_A::POTALDESC potalDesc;
	CObject_EscalatorCube::ESCALATORDESC escalDesc;


	potalDesc.iNowScene = SCENEID::SCENE_IMGUISCENE;
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
				GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_STAGE1,
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
				GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_STAGE1,
					TAG_LAY(Layer_Terrain), TAG_OP(Prototype_EscalatorCube),
					&escalDesc);

			}
			count_Escalator++;

		}

	}
	return S_OK;
}



CScene_Stage1 * CScene_Stage1::Create(LPDIRECT3DDEVICE9 GraphicDevice)
{
	CScene_Stage1* pTempGame = new CScene_Stage1(GraphicDevice);

	if (FAILED(pTempGame->Initialize()))
	{
		Safe_Release(pTempGame);
		MSGBOX("Failed to Creating Scene_Stage2");
		return nullptr;
	}

	return pTempGame;
}

void CScene_Stage1::Free()
{
	GetSingle(CMapLoadMgr)->DestroyInstance();

	__super::Free();
}
