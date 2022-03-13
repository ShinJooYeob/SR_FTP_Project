#include "stdafx.h"
#include "..\Public\Scene_Stage2.h"
#include "Scene_Loading.h"

#include "Camera_Main.h"
#include "MapLoadMgr.h"
#include "UI_Status.h"
#include "Object_PortalCube_A.h"
#include "Object_EscalatorCube.h"
#include "Object_OrbitButton.h"
#include "Object_Star.h"
#include "UI_Result.h"


CScene_Stage2::CScene_Stage2(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
}

HRESULT CScene_Stage2::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main))))
		return E_FAIL;


	if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_PauseUI(TEXT("Layer_PauseUI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TAG_LAY(Layer_Player))))
		return E_FAIL;

	FAILED_CHECK(Ready_Layer_OrbitButton_And_Cube(TEXT("Layer_OrbitButton")));
	FAILED_CHECK(Ready_Layer_UI_Result(TEXT("Layer_UI_Result")));
	FAILED_CHECK(Ready_Layer_UI_Start(TEXT("Layer_UI_Start")));
	FAILED_CHECK(Ready_Layer_Object_Star(TEXT("Layer_Object_Star")));
	FAILED_CHECK(Ready_Layer_PlayerStatusUI(TEXT("Layer_StatusUI")));
	FAILED_CHECK(Ready_Layer_StageEndCollsionObject(TEXT("Layer_Collision_StageEnd")));
	


	// 로드된 오브젝트 정보로 그리기
	//GetSingle(CGameInstance)->Add_GameObject_To_Layer(
	//	SCENEID::SCENE_STAGE2,
	//	TAG_LAY(Layer_Terrain),
	//	TAG_OP(Prototype_TerrainCube),
	//	_float3(0, 0, 0));

	// 생성되지 않는 특수 큐브 저장
	list< SPECIALCUBE*> SpecialCubeList;
	GetSingle(CMapLoadMgr)->LoadMap(SCENEID::SCENE_STAGE2, 2, &SpecialCubeList);

	FAILED_CHECK(Ready_Layer_Terrain(&SpecialCubeList));

	for (auto data : SpecialCubeList)
	{
		Safe_Delete(data);
	}
	SpecialCubeList.clear();

	
	//GetSingle(CGameInstance)->PlayBGM(L"JY_lydiandominant.wav");
	GetSingle(CGameInstance)->PlayBGM(L"JY_Stage2_BGM.mp3");
	//GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_BGM, 0.1f);


	FAILED_CHECK(Ready_Layer_Particle(L"Layer_Particle"));
	FAILED_CHECK(Ready_Layer_ParsedObject(L"Layer_ParsedObject"));

	
	return S_OK;
}

_int CScene_Stage2::Update(_float fDeltaTime)
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
		case SCENEID::SCENE_STAGE2:
		{
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE2), SCENEID::SCENE_LOADING)))
				return E_FAIL;
			break;
		}

		}
	}

	return 0;
}

_int CScene_Stage2::LateUpdate(_float fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	return 0;
}

_int CScene_Stage2::Render()
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

_int CScene_Stage2::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

HRESULT CScene_Stage2::Scene_InGame_Chage(_bool Scene_Chage_Switch, _uint _INextScene)
{
	m_bScene_Switch = Scene_Chage_Switch;
	m_INextScene = _INextScene;

	return S_OK;
}




HRESULT CScene_Stage2::Ready_Layer_MainCamera(const _tchar * pLayerTag)
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

	_float3 ActionPos[5] = { _float3(0, 10.f, -2.f) ,_float3(13.f, -14.f, -15.f) ,_float3(-7.f, -10.f, -20.f),_float3(-10.f, 19.f, -12.f),_float3(0.f, 32.f, -26.f) };

	FAILED_CHECK(pMainCam->ReInitialize(ActionPos,4))


		pMainCam->Set_NowSceneNum(SCENE_STAGE2);
	pMainCam->CameraEffect(CCamera_Main::CAM_EFT_FADE_OUT, g_fDeltaTime, 4.f);

	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{


	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_SkyBox")))
		return E_FAIL;

	return S_OK;


}

HRESULT CScene_Stage2::Ready_Layer_UI_Result(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_UI_Result")))
		return E_FAIL;


	CUI_Result* pResult = (CUI_Result*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGE2, TEXT("Layer_UI_Result")));
	if (pResult == nullptr)
		return E_FAIL;
	pResult->Set_MaxTime(300.f);

	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_UI_Start(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_UI_Start")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Object_Star(const _tchar * pLayerTag)
{
	
	CObject_Star::STARDESC StarDesc;

	StarDesc.fTransform = _float3(0, 10.f, -2.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	StarDesc.fTransform = _float3(13.f, -14.f, -15.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	StarDesc.fTransform = _float3(-7.f, -10.f, -20.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	StarDesc.fTransform = _float3(-10.f, 19.f, -12.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	StarDesc.fTransform = _float3(0.f, 32.f, -26.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;


	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_PlayerStatusUI(const _tchar * pLayerTag)
{
	list<CGameObject*>* pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, TAG_LAY(Layer_Player));

	NULL_CHECK_BREAK(pPlayerList);

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_STAGE2, pLayerTag,TEXT("Prototype_GameObject_StatusUI")))
		return E_FAIL;

	CUI_Status* pStatusUI = (CUI_Status*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGE2, TEXT("Layer_StatusUI")));

	if (pStatusUI == nullptr)
		return E_FAIL;

	FAILED_CHECK(pStatusUI->Set_Player(pPlayerList->front()));


	CUI_Result* pResult = (CUI_Result*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGE2, TEXT("Layer_UI_Result")));
	if (pResult == nullptr)
		return E_FAIL;
	FAILED_CHECK(pStatusUI->Set_ResultUI(pResult));

	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_OrbitButton_And_Cube(const _tchar * pLayerTag)
{

	CObject_OrbitButton::ORBITDESC tDesc;

	tDesc.vButtonPos = _float3(-7, -8, 2);
	tDesc.vOrbitRotAxis = _float3(-7, -8, -7);
	tDesc.vOribitCubeStartPos = _float3(-7, -11, 3);
	tDesc.vOribitTotalXYZ = _float3(2, 1, 4);


	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_OrbitButton"), &tDesc))
		return E_FAIL;

	tDesc.vButtonPos = _float3(-19, 12, 13);
	tDesc.vOrbitRotAxis = _float3(-19, -2, -2);
	tDesc.vOribitCubeStartPos = _float3(-19, 9, 16);
	tDesc.vOribitTotalXYZ = _float3(1, 1, 3);


	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_OrbitButton"), &tDesc))
		return E_FAIL;



	return S_OK	;
}

HRESULT CScene_Stage2::Ready_Layer_ParsedObject(const _tchar * pLayerTag)
{
	_float3 TransformPos;
	//올빼미 머리 몸통 세트
	TransformPos = _float3(14.f, 64.f, -37.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_BigOwl", TEXT("Prototype_GameObject_BigOwl"), &TransformPos));
	TransformPos = _float3(14.f, 64.f, -37.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_BigOwlHead", TEXT("Prototype_GameObject_BigOwlHead"), &TransformPos));

	TransformPos = _float3(-1.f, 64.f, -34.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_BigOwl", TEXT("Prototype_GameObject_BigOwl"), &TransformPos));
	TransformPos = _float3(-1.f, 64.f, -34.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_BigOwlHead", TEXT("Prototype_GameObject_BigOwlHead"), &TransformPos));

	TransformPos = _float3(-4.f, 64.f, -17.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_BigOwl", TEXT("Prototype_GameObject_BigOwl"), &TransformPos));
	TransformPos = _float3(-4.f, 64.f, -17.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_BigOwlHead", TEXT("Prototype_GameObject_BigOwlHead"), &TransformPos));

	TransformPos = _float3(11.f, 64.f, -14.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_BigOwl", TEXT("Prototype_GameObject_BigOwl"), &TransformPos));
	TransformPos = _float3(11.f, 64.f, -14.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_BigOwlHead", TEXT("Prototype_GameObject_BigOwlHead"), &TransformPos));

	////////////tomb//////////////////////////////////////////////////////////////
	 TransformPos = _float3(-16, 18, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	
	TransformPos = _float3(-3, 20, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(15, 18, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(6, 16, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-4, 11, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-10, 12, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-20, 11, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-7, 6, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(1, 5, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(10, 8, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(21, 7, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(16, 6, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(11, 2, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-19, 1, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-15, -8, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-21, -12, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-2, -6, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(6, -5, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(20, -5, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(14, -8, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-4, -10, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-9, -14, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-17, -17, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-21, -21, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-24, -28, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-11, -23, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(0, -18, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(9, -14, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(18, -17, 0.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));




	TransformPos = _float3(-16, 18, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));


	TransformPos = _float3(-3, 20, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(15, 18, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(6, 16, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-4, 11, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-10, 12, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-20, 11, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-7, 6, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(1, 5, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(10, 8, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(21, 7, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(16, 6, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(11, 2, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-19, 1, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-15, -8, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-21, -12, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-2, -6, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(6, -5, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(20, -5, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(14, -8, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-4, -10, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-9, -14, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-17, -17, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-21, -21, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-24, -28, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(-11, -23, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(0, -18, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(9, -14, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));

	TransformPos = _float3(18, -17, 1.5f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TombStone", TEXT("Prototype_GameObject_TombStone"), &TransformPos));





	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_StageEndCollsionObject(const _tchar * pLayerTag)
{

	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object"), &_float3(0.f, 1.f, 0.f)))
	//	return E_FAIL; 

	_float3 TransformPos = _float3(5.f, 64.f, -28.f);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, L"Layer_TreasureBox", TEXT("Prototype_GameObject_TreasureBox"), &TransformPos));


	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object"),&_float3(5.f, 64.f, -28.f)))
		return E_FAIL;


	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_PauseUI(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_PauseUI")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Particle(const _tchar * pLayerTag)
{
	PARTICLEDESC tParticleDesc = {};


	tParticleDesc.eParticleID = Particle_Ball;
	tParticleDesc.TotalParticleTime = 3000.f;
	tParticleDesc.EachParticleLifeTime = 3.0;
	
	tParticleDesc.ParticleSize = _float3(0.5f, 0.5f, 0.5f);
	
	tParticleDesc.Particle_Power = 1;
	tParticleDesc.PowerRandomRange = _float2(0.5f, 1.5f);
	
	tParticleDesc.MaxParticleCount = 40;

	tParticleDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JY_Effect");
	tParticleDesc.szTextureLayerTag = TEXT("Meteo_dust_B");

	tParticleDesc.m_bIsTextureAutoFrame = true;
	tParticleDesc.fAutoFrameMul = 8.f;
	tParticleDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_Commponent_By_LayerIndex(SCENE_STATIC,TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));
	tParticleDesc.FixedTarget = _float3();

	tParticleDesc.MaxBoundary = _float3(100, 100, 100);
	tParticleDesc.ParticleColorChage = false;
	//tParticleDesc.TargetColor = _float3(237, 28, 36);
	//tParticleDesc.TargetColor2 = _float3(53, 255.f, 11);
	tParticleDesc.m_bIsUI = false;
	tParticleDesc.ParticleStartRandomPosMin = _float3(-24.f, -20.f, -24.0f);
	tParticleDesc.ParticleStartRandomPosMax = _float3(24.f, 20.0f, 24.0f);

	tParticleDesc.MustDraw = false;
	tParticleDesc.IsParticleFameEndtoDie = false;
	tParticleDesc.AlphaBlendON = true;

	//Create_ParticleObject를 호출하여 스테이지 아이디와 지금까지 설정한 desc를 넣어주면 됨
	GetSingle(CParticleMgr)->Create_ParticleObject(SCENE_STAGE2, tParticleDesc);



	PARTICLEDESC tSubParticleDesc = {};

	tParticleDesc.eParticleID = Particle_Cone;
	tParticleDesc.TotalParticleTime = 3000.f;
	tParticleDesc.EachParticleLifeTime = 6.0;

	tParticleDesc.ParticleSize = _float3(1.f, 1.2f, 1.f);

	tParticleDesc.Particle_Power = 8;
	tParticleDesc.PowerRandomRange = _float2(1.f, 1.5f);

	tParticleDesc.MaxParticleCount = 6;

	tParticleDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JY_Effect");
	tParticleDesc.szTextureLayerTag = TEXT("Meteo_B");

	tParticleDesc.m_bIsTextureAutoFrame = true;
	tParticleDesc.fAutoFrameMul = 3.f;
	tParticleDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_Commponent_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));
	tParticleDesc.FixedTarget = _float3();

	tParticleDesc.MaxBoundary = _float3(100, 20, 100);
	tParticleDesc.ParticleColorChage = false;
	//tParticleDesc.TargetColor = _float3(237, 28, 36);
	//tParticleDesc.TargetColor2 = _float3(53, 255.f, 11);
	tParticleDesc.m_bIsUI = false;
	tParticleDesc.ParticleStartRandomPosMin = _float3(-24.f, 20.f, -24.0f);
	tParticleDesc.ParticleStartRandomPosMax = _float3(24.f, 20.0f, 24.0f);

	tParticleDesc.vUp = _float3(0, -1, 0);

	tParticleDesc.MustDraw = false;
	tParticleDesc.IsParticleFameEndtoDie = false;
	tParticleDesc.AlphaBlendON = false;
	tParticleDesc.bSubPraticle= true;


	tSubParticleDesc.TotalParticleTime = 3000.f;
	tSubParticleDesc.EachParticleLifeTime = 0.35f;

	tSubParticleDesc.ParticleSize = _float3(1.5f, 1.5f, 1.5f);

	tSubParticleDesc.Particle_Power = 8;
	tSubParticleDesc.PowerRandomRange = _float2(0.5f, 1.5f);

	tSubParticleDesc.MaxParticleCount = 10;

	tSubParticleDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JY_Effect");
	tSubParticleDesc.szTextureLayerTag = TEXT("Meteo_dust_C");

	tSubParticleDesc.m_bIsTextureAutoFrame = true;
	tSubParticleDesc.fAutoFrameMul = 10.f;
	tSubParticleDesc.FollowingTarget = nullptr;
	tSubParticleDesc.FixedTarget = _float3();

	tSubParticleDesc.MaxBoundary = _float3(100, 20, 100);
	tSubParticleDesc.ParticleColorChage = false;

	tSubParticleDesc.m_bIsUI = false;
	tSubParticleDesc.ParticleStartRandomPosMin = _float3(0.f, 0.0f, 0.0f);
	tSubParticleDesc.ParticleStartRandomPosMax = _float3(0.f, 0.0f, 0.0f);

	tSubParticleDesc.vUp = _float3(0, 1, 0);

	tSubParticleDesc.MustDraw = false;
	tSubParticleDesc.IsParticleFameEndtoDie = false;
	tSubParticleDesc.AlphaBlendON = true;

	GetSingle(CParticleMgr)->Create_ParticleObject_AddSub(SCENE_STAGE2, tParticleDesc, tSubParticleDesc);




	tParticleDesc.eParticleID = Particle_Cone;
	tParticleDesc.TotalParticleTime = 3000.f;
	tParticleDesc.EachParticleLifeTime = 6.0;

	tParticleDesc.ParticleSize = _float3(1.f, 1.2f, 1.f);

	tParticleDesc.Particle_Power = 8;
	tParticleDesc.PowerRandomRange = _float2(1.f, 1.5f);

	tParticleDesc.MaxParticleCount = 6;

	tParticleDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JY_Effect");
	tParticleDesc.szTextureLayerTag = TEXT("Meteo_A");

	tParticleDesc.m_bIsTextureAutoFrame = true;
	tParticleDesc.fAutoFrameMul = 1.5f;
	tParticleDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_Commponent_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));
	tParticleDesc.FixedTarget = _float3();

	tParticleDesc.MaxBoundary = _float3(100, 20, 100);
	tParticleDesc.ParticleColorChage = false;
	//tParticleDesc.TargetColor = _float3(237, 28, 36);
	//tParticleDesc.TargetColor2 = _float3(53, 255.f, 11);
	tParticleDesc.m_bIsUI = false;
	tParticleDesc.ParticleStartRandomPosMin = _float3(-24.f, 20.f, -24.0f);
	tParticleDesc.ParticleStartRandomPosMax = _float3(24.f, 20.0f, 24.0f);

	tParticleDesc.vUp = _float3(0, -1, 0);

	tParticleDesc.MustDraw = false;
	tParticleDesc.IsParticleFameEndtoDie = false;
	tParticleDesc.AlphaBlendON = false;
	tParticleDesc.bSubPraticle = true;



	tSubParticleDesc.eParticleID = Particle_Ball;
	tSubParticleDesc.TotalParticleTime = 3000.f;
	tSubParticleDesc.EachParticleLifeTime = 0.35f;

	tSubParticleDesc.ParticleSize = _float3(1.5f, 1.5f, 1.5f);

	tSubParticleDesc.Particle_Power = 1;
	tSubParticleDesc.PowerRandomRange = _float2(0.1f, 1.5f);

	tSubParticleDesc.MaxParticleCount = 10;

	tSubParticleDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JY_Effect");
	tSubParticleDesc.szTextureLayerTag = TEXT("Meteo_dust_A");

	tSubParticleDesc.m_bIsTextureAutoFrame = true;
	tSubParticleDesc.fAutoFrameMul = 10.f;
	tSubParticleDesc.FollowingTarget = nullptr;
	tSubParticleDesc.FixedTarget = _float3();

	tSubParticleDesc.MaxBoundary = _float3(100, 20, 100);
	tSubParticleDesc.ParticleColorChage = false;

	tSubParticleDesc.m_bIsUI = false;
	tSubParticleDesc.ParticleStartRandomPosMin = _float3(-0.2f, 0.0f, -0.2f);
	tSubParticleDesc.ParticleStartRandomPosMax = _float3(0.2f, 0.0f, 0.2f);

	tSubParticleDesc.vUp = _float3(0, 1, 0);

	tSubParticleDesc.MustDraw = false;
	tSubParticleDesc.IsParticleFameEndtoDie = false;
	tSubParticleDesc.AlphaBlendON = true;

	GetSingle(CParticleMgr)->Create_ParticleObject_AddSub(SCENE_STAGE2, tParticleDesc,tSubParticleDesc);


	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Player(const _tchar * pLayerTag)
{
	list<CGameObject*>* pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, pLayerTag);
	if (pPlayerList == nullptr)
	{
		if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, pLayerTag, TAG_OP(Prototype_Player)))
			return E_FAIL;

		GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, pLayerTag)->Set_NowSceneNum(SCENE_STAGE2);
		pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, pLayerTag);
		//(pPlayerList->front())->ReInitialize(&_float3(5.f, 64.f, -28.f));
	}
	else
	{

		(pPlayerList->front())->ReInitialize(&_float3(0.f, 1.f, 0));
		//(pPlayerList->front())->ReInitialize(&_float3(5.f, 64.f, -28.f));

	}


	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Terrain(list<SPECIALCUBE*>* listdata)
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
				GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_STAGE2,
					TAG_LAY(Layer_Terrain), TAG_OP(Prototype_EscalatorCube),
					&escalDesc);

			}
			count_Escalator++;

		}

	}
	return S_OK;
}



CScene_Stage2 * CScene_Stage2::Create(LPDIRECT3DDEVICE9 GraphicDevice)
{
	CScene_Stage2* pTempGame = new CScene_Stage2(GraphicDevice);

	if (FAILED(pTempGame->Initialize()))
	{
		Safe_Release(pTempGame);
		MSGBOX("Failed to Creating Scene_Stage2");
		return nullptr;
	}

	return pTempGame;
}

void CScene_Stage2::Free()
{
	GetSingle(CMapLoadMgr)->DestroyInstance();

	__super::Free();
}
