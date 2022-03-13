#include "stdafx.h"
#include "..\Public\Scene_StageSelect.h"
#include "Scene_Loading.h"
#include "Camera_Main.h""
#include "MapLoadMgr.h"
#include "Object_PortalCube_A.h"
#include "Object_EscalatorCube.h"
#include "Object_OrbitButton.h"
#include "Object_Star.h"
#include "Npc.h"
#include "BackGround.h"

#include "Collision_Object_StageEntry.h"

_float3		CScene_StageSelect::m_StageReturnBlock =NOT_EXIST_BLOCK; 

CScene_StageSelect::CScene_StageSelect(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
}

HRESULT CScene_StageSelect::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;



	if (FAILED(Ready_Layer_Player(TAG_LAY(Layer_Player))))
		return E_FAIL;

	FAILED_CHECK(Ready_Layer_SkyBox(TEXT("Layer_SkyBox")))

		if (FAILED(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main))))
			return E_FAIL;
	if (FAILED(Ready_Layer_Shop(TAG_LAY(Layer_Shop))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Quest(TAG_LAY(Layer_Quest))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI_Common(TAG_LAY(Layer_UI_Common))))
		return E_FAIL;

	FAILED_CHECK(Ready_Layer_ParsedObject());

	Make_Particle();
	FAILED_CHECK(Ready_Layer_StageEntryCollsionObject(L"Layer_Collision_StageEntry"));

	if (FAILED(Ready_Layer_UI_Complete(TEXT("Layer_UI_Complete"))))
		return E_FAIL;

	
	


	//FAILED_CHECK( Ready_Layer_Object_VanishCube(TAG_LAY(Layer_Terrain)));//사라지는 큐브


	//if (FAILED(Ready_Layer_Object_PortalCube(TEXT("Layer_PotalCube"))))
	//	return E_FAIL;
	//if (FAILED(Ready_Layer_GravityCube(TEXT("Layer_GravityCube"))))
	//	return E_FAIL;
	if (FAILED(Ready_Layer_NPC(TEXT("Layer_NPC"))))
		return E_FAIL;


	/*if (FAILED(Ready_Layer_GravityCube(TEXT("Layer_GravityCube"))))
		return E_FAIL;


	if (FAILED(Ready_Layer_Object_PortalCube(TEXT("Layer_PotalCube"))))
		return E_FAIL;*/

	


	//if (FAILED(Ready_Layer_UI_Result(TEXT("Layer_UI_Result"))))
	//	return E_FAIL;
	//if (FAILED(Ready_Layer_Object_Star(TEXT("Layer_Object_Star"))))
	//	return E_FAIL;


	// 로드된 오브젝트 정보로 그리기
	GetSingle(CGameInstance)->Add_GameObject_To_Layer(
		SCENEID::SCENE_STAGESELECT,
		TAG_LAY(Layer_Terrain),
		TAG_OP(Prototype_TerrainCube),
		_float3(0, 0, 0));

	// 생성되지 않는 특수 큐브 저장
	list< SPECIALCUBE*> SpecialCubeList;
	GetSingle(CMapLoadMgr)->LoadMap(SCENEID::SCENE_STAGESELECT, 0, &SpecialCubeList);

	FAILED_CHECK(Ready_Layer_Terrain(&SpecialCubeList));

	for (auto data : SpecialCubeList)
	{
		Safe_Delete(data);
	}
	SpecialCubeList.clear();

	
	FAILED_CHECK(GetSingle(CGameInstance)->PlayBGM((L"JW_SelectBGM.ogg")));
	

	if (FAILED(Ready_Layer_Door(TEXT("Layer_Door"))))
		return E_FAIL;

	return S_OK;
}

_int CScene_StageSelect::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	//if (GetKeyState(VK_F1) & 0x8000)
	//{
	//	FAILED_CHECK(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGESELECT), SCENEID::SCENE_LOADING));
	//}
	//if (GetKeyState(VK_F2) & 0x8000)
	//{
	//	FAILED_CHECK(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE1), SCENEID::SCENE_LOADING));
	//}

	//if (GetKeyState(VK_F3) & 0x8000)
	//{
	//	FAILED_CHECK(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE2), SCENEID::SCENE_LOADING));
	//}

	//if (GetKeyState(VK_F4) & 0x8000)
	//{
	//	FAILED_CHECK(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE3), SCENEID::SCENE_LOADING));
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
			m_StageReturnBlock = _float3(12, 5, 6);
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE1), SCENEID::SCENE_LOADING)))
				return E_FAIL;
			break;
		}
		case SCENEID::SCENE_STAGE2:
		{
			m_StageReturnBlock = _float3(6, 8, 14);
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE2), SCENEID::SCENE_LOADING)))
				return E_FAIL;
			break;
		}
		case SCENEID::SCENE_STAGE3:
		{
			m_StageReturnBlock = _float3(13, 15, 13);
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE3), SCENEID::SCENE_LOADING)))
				return E_FAIL;
			break;
		}
		case SCENEID::SCENE_TUTORIAL:
		{
			m_StageReturnBlock = _float3(6, 1, 0);
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_TUTORIAL), SCENEID::SCENE_LOADING)))
				return E_FAIL;
			break;
		}
		case SCENEID::SCENE_BOSS:
		{
			m_StageReturnBlock = _float3(8.5f, -9, 7);
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_BOSS), SCENEID::SCENE_LOADING)))
				return E_FAIL;
			break;
		}
		}
	}

	return 0;
}

_int CScene_StageSelect::LateUpdate(_float fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	return 0;
}

_int CScene_StageSelect::Render()
{
	if (__super::Render() < 0)
		return -1;



//
//#ifdef _DEBUG
//	SetWindowText(g_hWnd, TEXT("GameScene"));
//#endif // _DEBUG
//
//
	return 0;
}

_int CScene_StageSelect::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

HRESULT CScene_StageSelect::Scene_InGame_Chage(_bool Scene_Chage_Switch, _uint _INextScene)
{
	m_bScene_Switch = Scene_Chage_Switch;
	m_INextScene = _INextScene;

	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{
	
	if (FAILED( GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_SkyBox"))))
		return E_FAIL;

	return S_OK;
}
HRESULT CScene_StageSelect::Ready_Layer_ParsedObject()
{
	_float3 temp = _float3(9.0, 22.5, 25.0);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, L"Layer_MapleTree", TEXT("Prototype_GameObject_MapleTree"), &temp));
	temp = _float3(20.0, 12.5, 16);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, L"Layer_MapleTree", TEXT("Prototype_GameObject_MapleTree"), &temp));
	temp = _float3(-6.0, 15.5, 11);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, L"Layer_BigGreenTree", TEXT("Prototype_GameObject_BigGreenTree"), &temp));
	temp = _float3(12.0, 15.5, -8);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, L"Layer_BigGreenTree", TEXT("Prototype_GameObject_BigGreenTree"), &temp));

	if (GetSingle(CQuest)->Get_QuestComplete())
	{
		if (FAILED(Ready_Layer_Object_QrcodeCube(TEXT("Layer_Object_QrcodeCube"))))
			return E_FAIL;
	}
	else
	{
		//_float3 TransformPos = _float3(0.f, 0.f, 0.f);
		_float3 TransformPos = _float3(8.f, 25.3f, 9.5f);
		FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, L"Layer_Bell", TEXT("Prototype_GameObject_Bell"), &TransformPos));

	}

}
HRESULT CScene_StageSelect::Ready_Layer_NPC(const _tchar * pLayerTag)
{
	CNpc::NPCDESC NpcDesc;

	NpcDesc.vPos = _float3(7.f, 1.f, 0.f);
	NpcDesc.vDir = _float3(1.f,0.f,0.f);
	NpcDesc.pNpcTextureName = TEXT("ivy_Idle");
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_Npc_ivy"),(void*)&NpcDesc))
		return E_FAIL;

	NpcDesc.vPos = _float3(15.f, 5.f, 6.f);
	NpcDesc.vDir = _float3(1.f, 0.f, 0.f);
	NpcDesc.pNpcTextureName = TEXT("izaac_Idle");
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_Npc_izaac"), (void*)&NpcDesc))
		return E_FAIL;

	NpcDesc.vPos = _float3(7.f, 8.f, 14.f);
	NpcDesc.vDir = _float3(1.f, 0.f, 0.f);
	NpcDesc.pNpcTextureName = TEXT("geezer_Idle");
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_Npc_geezer"), (void*)&NpcDesc))
		return E_FAIL;

	NpcDesc.vPos = _float3(13.f, 15.f, 11.f);
	NpcDesc.vDir = _float3(0.f, 0.f, -1.f);
	NpcDesc.pNpcTextureName = TEXT("oldy_Idle");
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_Npc_oldy"), (void*)&NpcDesc))
		return E_FAIL;



	return S_OK;


}
HRESULT CScene_StageSelect::Ready_Layer_UI_Complete(const _tchar * pLayerTag)
{

	if (FAILED(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_UI_Complete"))))
		return E_FAIL;

	return S_OK;
}
//
//HRESULT CScene_StageSelect::Ready_Layer_Terrain(const _tchar * pLayerTag)
//{
//	for (_uint i = 0; i < 6; i++)
//	{
//		for (_uint j = 0; j < 6; j++) {
//
//			if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_TerrainCube), &_float3((_float)i, (_float)0, (_float)j)))
//				return E_FAIL;
//		}
//	}
//	for (_uint k = 0; k < 10; k++)
//	{
//		for (_uint i = 1; i < 10; i++)
//		{
//			for (_uint j = 1; j < 10; j++)
//			{
//				if (i == 1 || i == 9 || j == 3 || j == 7)
//				{
//
//					if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_TerrainCube), &_float3((_float)i, (_float)k, (_float)j)))
//						return E_FAIL;
//				}
//			}
//		}
//	}
//
//
//	for (_uint i = 0; i < 6; i++)
//	{
//		for (_uint j = 0; j < 6; j++) {
//
//			if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_TerrainCube), &_float3((_float)i + 30, (_float)-24, (_float)j)))
//				return E_FAIL;
//		}
//	}
//
//
//
//	if (FAILED(Ready_Layer_JumpCube(pLayerTag)))
//		return E_FAIL;
//
//	if (FAILED(Ready_Layer_GravityCube(pLayerTag)))
//		return E_FAIL;
//
//	if (FAILED(Ready_Layer_Object_PortalCube(pLayerTag)))
//		return E_FAIL;
//
//	if (FAILED(Ready_Layer_ElevetorCube(pLayerTag)))
//		return E_FAIL;
//
//	FAILED_CHECK(Ready_Layer_Object_VanishCube(pLayerTag));
//	FAILED_CHECK(Ready_Layer_Object_AppearCube(pLayerTag));
//	
//
//	if (FAILED(Ready_Layer_OrbitButton_And_Cube(TEXT("Layer_OrbitButton"))))
//		return E_FAIL;
//	
//
//
//
//	return S_OK;
//}

HRESULT CScene_StageSelect::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.bIsOrtho = true;

	if (m_StageReturnBlock != NOT_EXIST_BLOCK)
		CameraDesc.vWorldRotAxis = m_StageReturnBlock + _float3(0.f, 3.f, 0.f);
	else
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

	if (m_StageReturnBlock == _float3(6, 8, 14))
	{
		if (FAILED(pMainCam->Reset_LookAtAxis(&CameraDesc, CCamera_Main::Look_Back_Axis)))
			return E_FAIL;
	}
	else if (m_StageReturnBlock == _float3(13, 15, 13))
	{
		if (FAILED(pMainCam->Reset_LookAtAxis(&CameraDesc, CCamera_Main::Look_Left_Axis)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(pMainCam->Reset_LookAtAxis(&CameraDesc)))
			return E_FAIL;
	}


	_float3 ActionPos[1] = { _float3(30,-24,0) };

	FAILED_CHECK(pMainCam->ReInitialize(ActionPos,1))

		pMainCam->Set_NowSceneNum(SCENE_STAGESELECT);
	pMainCam->CameraEffect(CCamera_Main::CAM_EFT_FADE_OUT, g_fDeltaTime, 2.5f);


	
	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_Player(const _tchar * pLayerTag)
{


	list<CGameObject*>* pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, pLayerTag);
	if (pPlayerList == nullptr)
	{
		if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, pLayerTag, TAG_OP(Prototype_Player)))
			return E_FAIL;

		GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, pLayerTag)->Set_NowSceneNum(SCENE_STAGESELECT);
	}
	else 
	{
		if (m_StageReturnBlock != NOT_EXIST_BLOCK)
			(pPlayerList->front())->ReInitialize(&m_StageReturnBlock);
		else
			(pPlayerList->front())->ReInitialize(&_float3(0, 1, 0));
		
	}


	////////////////////////////////////////////////////////////////////////////
	////로그인 테스트

	//CPlayer* pPlayer = (CPlayer*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player)));
	//if (pPlayer == nullptr)
	//	return E_FAIL;
	//_bool bIsSuccess = false;

	//FAILED_CHECK(GetSingle(CLoginMgr)->Save_PlayerData(pPlayer, &bIsSuccess));


	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_UI_Common(const _tchar * pLayerTag)
{
	list<CGameObject*>* pObjectList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, pLayerTag);
	if (pObjectList == nullptr)
	{
		if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, pLayerTag, TAG_OP(Prototype_UI_Common)))
			return E_FAIL;

		GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, pLayerTag)->Set_NowSceneNum(SCENE_STAGESELECT);
	}



	return S_OK;
}
HRESULT CScene_StageSelect::Ready_Layer_Shop(const _tchar * pLayerTag)
{

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_Shop)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_GravityCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, TEXT("Layer_GravityCube"), TEXT("Prototype_GameObject_Object_GravityCube"), &_float3((_float)5, (_float)1, (_float)0)))
		return E_FAIL;

	return S_OK;
}


HRESULT CScene_StageSelect::Ready_Layer_Quest(const _tchar * pLayerTag)
{

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_Quest)))
		return E_FAIL;

	return S_OK;
}



HRESULT CScene_StageSelect::Ready_Layer_JumpCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_Object_FixCube"), &_float3((_float)6, (_float)1, (_float)0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_Object_PortalCube(const _tchar * pLayerTag)
{

	CObject_PortalCube_A::POTALDESC tagDesc;

	tagDesc.vPos_A_Cube = _float3(0,3,0);
	tagDesc.vPos_B_Cube = _float3(10,3.f,0);
	tagDesc.iNowScene = SCENEID::SCENE_STAGESELECT;

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, TEXT("Layer_Potal"), TEXT("Prototype_GameObject_Object_PortalCube_A"),&tagDesc))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_ElevetorCube(const _tchar * pLayerTag)
{
	CObject_EscalatorCube::ESCALATORDESC tDesc;

	tDesc.vStartPos = _float3(-2.f, 0.f, 0.f);
	tDesc.vEndPos = _float3(2.f, -5.f, -5.f);

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_Object_EscalatorCube"), &tDesc))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_OrbitButton_And_Cube(const _tchar * pLayerTag)
{
	CObject_OrbitButton::ORBITDESC tDesc;

	tDesc.vButtonPos = _float3(1,10,1);
	tDesc.vOrbitRotAxis = _float3(5, 0, 5);
	tDesc.vOribitCubeStartPos = _float3(-3, 10, 5);
	tDesc.vOribitTotalXYZ = _float3(2, 2, 2);


	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_Object_OrbitButton"), &tDesc))
		return E_FAIL;




	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_UI_Result(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_UI_Result")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_Terrain(list<SPECIALCUBE*>* listdata)
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
				GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_STAGESELECT,
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
				GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_STAGESELECT,
					TAG_LAY(Layer_Terrain), TAG_OP(Prototype_EscalatorCube),
					&escalDesc);

			}
			count_Escalator++;

		}

	}
	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_StageEntryCollsionObject(const _tchar * pLayerTag)
{
	CCollision_Object_StageEntry::STAGEENTERYDESC tDesc;



	////튜토리얼 입구
	tDesc.vPos = _float3(6, 1, 0);
	tDesc.eTargetScene = SCENE_TUTORIAL;////;
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object_StageEntry"), &tDesc))
		return E_FAIL;

	//1스테이지 입구
	tDesc.vPos = _float3(12, 5, 6);
	tDesc.eTargetScene = SCENE_STAGE1;
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object_StageEntry"), &tDesc))
		return E_FAIL;
	//2스테이지 입구
	tDesc.vPos = _float3(6, 8, 14);
	tDesc.eTargetScene = SCENE_STAGE2;
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object_StageEntry"), &tDesc))
		return E_FAIL;
	//3스테이지 입구
	tDesc.vPos = _float3(13, 15, 13);
	tDesc.eTargetScene = SCENE_STAGE3;
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object_StageEntry"), &tDesc))
		return E_FAIL;


	//보스 스테이지 입구
	tDesc.vPos = _float3(8, -9, 7);
	tDesc.eTargetScene = SCENE_BOSS;
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object_StageEntry"), &tDesc))
		return E_FAIL;
	tDesc.vPos = _float3(9, -9, 7);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object_StageEntry"), &tDesc))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_Door(const _tchar * pLayerTag)
{
	_float3 scale = _float3(2, 2, 1);
	_float3 offsetX = _float3(-0.5f, 0.5f, -0.6f);

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_BackGround)))
		return E_FAIL;
	// 0 
	auto objList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENE_STAGESELECT, pLayerTag);
	CBackGround *ObjDoor = (CBackGround*)objList->back();
	ObjDoor->m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(6, 1, 2) + offsetX);
	ObjDoor->m_ComTransform->Scaled(scale);

	// 1
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_BackGround)))
		return E_FAIL;
	ObjDoor = (CBackGround*)objList->back();
	ObjDoor->m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(12, 5, 7 )+ _float3( 0.5f,0.5f,-0.6f));
	ObjDoor->m_ComTransform->Scaled(_float3(2, 2, 1));;

	// 2
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_BackGround)))
		return E_FAIL;
	ObjDoor = (CBackGround*)objList->back();
	ObjDoor->m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(6-0.5f , 8+0.5f, 13+0.6f));
	ObjDoor->m_ComTransform->Scaled(scale);
	ObjDoor->m_ComTransform->Rotation_CW(UPVEC, D3DXToRadian(180));


	// 4
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_BackGround)))
		return E_FAIL;
	ObjDoor = (CBackGround*)objList->back();
	ObjDoor->m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(12+0.6f, 15+0.5f, 13-0.5f));
	ObjDoor->m_ComTransform->Scaled(scale);
	ObjDoor->m_ComTransform->Rotation_CW(UPVEC, D3DXToRadian(270));


	// boss
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_BackGround)))
		return E_FAIL;
	ObjDoor = (CBackGround*)objList->back();
	ObjDoor->m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(9, -8, 8) + offsetX + _float3(0,0,0.0899999999f));
	ObjDoor->m_ComTransform->Scaled(_float3(4, 5, 1));
	ObjDoor->m_ComTexture->Set_LoadTexutreNumber(2);
	return S_OK;

}


HRESULT CScene_StageSelect::Ready_Layer_Object_VanishCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_Object_VanishCube"), &_float3(-2.f, 0.f, 0.f)))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_Object_AppearCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_Object_AppearCube"), &_float3(10.f, -1.f, -1.f)))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_Object_QrcodeCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, TEXT("Layer_Object_QrcodeCube"), TEXT("Prototype_GameObject_Object_QrcodeCube")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_Object_Star(const _tchar * pLayerTag)
{
	CObject_Star::STARDESC StarDesc;


	for (int i = 0; i < 5; ++i)
	{
		StarDesc.fTransform = _float3(1.f+(i*10), 1.f,0.f);
		if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"),&StarDesc))
			return E_FAIL;
	}
	return S_OK;
}

void CScene_StageSelect::Make_Particle()
{
	PARTICLEDESC tDesc;

	tDesc.eParticleID = Particle_Cone;
	tDesc.TotalParticleTime = 3600.f;
	tDesc.EachParticleLifeTime = 4.0f;
	tDesc.ParticleSize = _float3(0.35f, 0.35f, 0.35f);
	tDesc.Particle_Power = 3.f;
	tDesc.PowerRandomRange = _float2(0.8f, 1.f);
	tDesc.MaxParticleCount = 15;
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	tDesc.szTextureLayerTag = TEXT("redleaf");
	tDesc.m_bIsTextureAutoFrame = false;
	tDesc.fAutoFrameMul = 5.f;
	tDesc.FixedTarget = _float3(8.0, 22.5, 25.0);
	tDesc.MaxBoundary = _float3(100, 100, 100);
	tDesc.ParticleColorChage = true;
	tDesc.TargetColor = _float3(240, 153, 94);
	tDesc.TargetColor2 = _float3(0.f, 182.f, 25.f);
	tDesc.m_bIsUI = false;
	tDesc.ParticleStartRandomPosMin = _float3(-3.f, -3.f, 3.f);
	tDesc.ParticleStartRandomPosMax = _float3(3.f, 3.f, 3.f);
	tDesc.vUp = _float3(0, -1, 0);
	tDesc.MustDraw = true;
	GetSingle(CParticleMgr)->Create_ParticleObject(SCENEID::SCENE_STAGESELECT, tDesc);

	tDesc.eParticleID = Particle_Cone;
	tDesc.TotalParticleTime = 3600.f;
	tDesc.EachParticleLifeTime = 4.0f;
	tDesc.ParticleSize = _float3(0.35f, 0.35f, 0.35f);
	tDesc.Particle_Power = 3.f;
	tDesc.PowerRandomRange = _float2(0.8f, 1.f);
	tDesc.MaxParticleCount = 15;
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	tDesc.szTextureLayerTag = TEXT("redleaf");
	tDesc.m_bIsTextureAutoFrame = false;
	tDesc.fAutoFrameMul = 5.f;
	tDesc.FixedTarget = _float3(20.0, 12.5, 16);
	tDesc.MaxBoundary = _float3(100, 100, 100);
	tDesc.ParticleColorChage = true;
	tDesc.TargetColor = _float3(240, 153, 94);
	tDesc.TargetColor2 = _float3(0.f, 182.f, 25.f);
	tDesc.m_bIsUI = false;
	tDesc.ParticleStartRandomPosMin = _float3(-3.f, -3.f, 3.f);
	tDesc.ParticleStartRandomPosMax = _float3(3.f, 3.f, 3.f);
	tDesc.vUp = _float3(0, -1, 0);
	tDesc.MustDraw = true;
	GetSingle(CParticleMgr)->Create_ParticleObject(SCENEID::SCENE_STAGESELECT, tDesc);

	tDesc.eParticleID = Particle_Cone;
	tDesc.TotalParticleTime = 3600.f;
	tDesc.EachParticleLifeTime = 4.0f;
	tDesc.ParticleSize = _float3(0.35f, 0.35f, 0.35f);
	tDesc.Particle_Power = 3.f;
	tDesc.PowerRandomRange = _float2(0.8f, 1.f);
	tDesc.MaxParticleCount = 15;
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	tDesc.szTextureLayerTag = TEXT("greenleaf2");
	tDesc.m_bIsTextureAutoFrame = false;
	tDesc.fAutoFrameMul = 5.f;
	tDesc.FixedTarget = _float3(-6.0, 15.5, 11);
	tDesc.MaxBoundary = _float3(100, 100, 100);
	tDesc.ParticleColorChage = false;
	tDesc.TargetColor = _float3(240, 153, 94);
	tDesc.TargetColor2 = _float3(0.f, 182.f, 25.f);
	tDesc.m_bIsUI = false;
	tDesc.ParticleStartRandomPosMin = _float3(-3.f, -3.f, 3.f);
	tDesc.ParticleStartRandomPosMax = _float3(3.f, 3.f, 3.f);
	tDesc.vUp = _float3(0, -1, 0);
	tDesc.MustDraw = true;
	GetSingle(CParticleMgr)->Create_ParticleObject(SCENEID::SCENE_STAGESELECT, tDesc);

	tDesc.eParticleID = Particle_Cone;
	tDesc.TotalParticleTime = 3600.f;
	tDesc.EachParticleLifeTime = 4.0f;
	tDesc.ParticleSize = _float3(0.35f, 0.35f, 0.35f);
	tDesc.Particle_Power = 3.f;
	tDesc.PowerRandomRange = _float2(0.8f, 1.f);
	tDesc.MaxParticleCount = 15;
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	tDesc.szTextureLayerTag = TEXT("greenleaf2");
	tDesc.m_bIsTextureAutoFrame = false;
	tDesc.fAutoFrameMul = 5.f;
	tDesc.FixedTarget = _float3(12.0, 15.5, -6);
	tDesc.MaxBoundary = _float3(100, 100, 100);
	tDesc.ParticleColorChage = false;
	tDesc.TargetColor = _float3(240, 153, 94);
	tDesc.TargetColor2 = _float3(0.f, 182.f, 25.f);
	tDesc.m_bIsUI = false;
	tDesc.ParticleStartRandomPosMin = _float3(-3.f, -3.f, 3.f);
	tDesc.ParticleStartRandomPosMax = _float3(3.f, 3.f, 3.f);
	tDesc.vUp = _float3(0, -1, 0);
	tDesc.MustDraw = true;
	GetSingle(CParticleMgr)->Create_ParticleObject(SCENEID::SCENE_STAGESELECT, tDesc);
}


CScene_StageSelect * CScene_StageSelect::Create(LPDIRECT3DDEVICE9 GraphicDevice)
{
	
	CScene_StageSelect* pTempGame = new CScene_StageSelect(GraphicDevice);

	if (FAILED(pTempGame->Initialize()))
	{
		Safe_Release(pTempGame);
		MSGBOX("Failed to Creating CScene_StageSelect");
		return nullptr;
	}

	return pTempGame;
}

void CScene_StageSelect::Free()
{
	GetSingle(CMapLoadMgr)->DestroyInstance();
	__super::Free();
}
