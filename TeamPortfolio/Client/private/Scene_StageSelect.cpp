#include "stdafx.h"
#include "..\Public\Scene_StageSelect.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "Object_PortalCube_A.h"
#include "Object_EscalatorCube.h"
#include "Object_OrbitButton.h"



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

	if (FAILED(Ready_Layer_Terrain(TAG_LAY(Layer_Terrain))))
		return E_FAIL;
	if (FAILED(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Shop(TAG_LAY(Layer_Shop))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Quest(TAG_LAY(Layer_Quest))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI_Common(TAG_LAY(Layer_UI_Common))))
		return E_FAIL;





	return S_OK;
}

_int CScene_StageSelect::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if ( GetSingle(CGameInstance)->Get_DIKeyState(DIK_RETURN) & DIS_Down) {
		if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE2), SCENEID::SCENE_LOADING)))
			return E_FAIL;
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




#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("GameScene"));
#endif // _DEBUG


	return 0;
}

_int CScene_StageSelect::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

HRESULT CScene_StageSelect::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_SkyBox")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	for (_uint i = 0; i < 6; i++)
	{
		for (_uint j = 0; j < 6; j++) {

			if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_TerrainCube), &_float3((_float)i, (_float)0, (_float)j)))
				return E_FAIL;
		}
	}
	for (_uint k = 0; k < 10; k++)
	{
		for (_uint i = 1; i < 10; i++)
		{
			for (_uint j = 1; j < 10; j++)
			{
				if (i == 1 || i == 9 || j == 3 || j == 7)
				{

					if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_TerrainCube), &_float3((_float)i, (_float)k, (_float)j)))
						return E_FAIL;
				}
			}
		}
	}


	for (_uint i = 0; i < 6; i++)
	{
		for (_uint j = 0; j < 6; j++) {

			if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_TerrainCube), &_float3((_float)i + 30, (_float)-24, (_float)j)))
				return E_FAIL;
		}
	}



	if (FAILED(Ready_Layer_JumpCube(pLayerTag)))
		return E_FAIL;

	if (FAILED(Ready_Layer_GravityCube(pLayerTag)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object_PortalCube(pLayerTag)))
		return E_FAIL;

	if (FAILED(Ready_Layer_ElevetorCube(pLayerTag)))
		return E_FAIL;
	if (FAILED(Ready_Layer_OrbitButton_And_Cube(TEXT("Layer_OrbitButton"))))
		return E_FAIL;
	



	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.bIsOrtho = true;
	CameraDesc.vWorldRotAxis = _float3(5.f, 3.f, 5.f);
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

	if(FAILED(pMainCam->Reset_LookAtAxis(&CameraDesc)))
		return E_FAIL;


	_float3 ActionPos[1] = { _float3(30,-24,0) };

	FAILED_CHECK(pMainCam->ReInitialize(ActionPos,1))

	pMainCam->Set_NowSceneNum(SCENE_STAGESELECT);
	pMainCam->CameraEffect(CCamera_Main::CAM_EFT_FADE_OUT, 0.016f);

	
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
		(pPlayerList->front())->ReInitialize(&_float3(0,1,0));

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
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, TEXT("Layer_GravityCube"), TEXT("Prototype_GameObject_Object_GravityCube"), &_float3((_float)6, (_float)10, (_float)3)))
		return E_FAIL;

	return S_OK;
}


HRESULT CScene_StageSelect::Ready_Layer_Quest(const _tchar * pLayerTag)
{

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_Quest)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_UI_Common(const _tchar * pLayerTag)
{

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_UI_Common)))
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

	tagDesc.vPos_A_Cube = _float3(3,3,0);
	tagDesc.vPos_B_Cube = _float3(30,-20.f,0);
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





CScene_StageSelect * CScene_StageSelect::Create(LPDIRECT3DDEVICE9 GraphicDevice)
{
	CScene_StageSelect* pTempGame = new CScene_StageSelect(GraphicDevice);

	if (FAILED(pTempGame->Initialize()))
	{
		Safe_Release(pTempGame);
		MSGBOX("Failed to Creating Scene_Stage1");
		return nullptr;
	}

	return pTempGame;
}

void CScene_StageSelect::Free()
{
	__super::Free();
}
