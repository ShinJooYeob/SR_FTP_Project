#include "stdafx.h"
#include "..\Public\Scene_Stage2.h"
#include "Scene_Loading.h"
#include "Object_OrbitCube.h"
#include "Camera_Main.h""


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
	////////////////////////////은혁이 테스트Layer_Player
	if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Cube(TAG_LAY(Layer_Player))))
		return E_FAIL;
	if (FAILED(Ready_Layer_FixCube(TAG_LAY(Layer_Terrain))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI_Result(TEXT("Layer_UI_Result"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_PushCube(TEXT("Layer_PushCube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_SelfRotationCube(TEXT("Layer_SelfRotationCube"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object_ButtonCube(TEXT("Layer_ButtonCube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Object_InteractiveCube(TEXT("Layer_InteractiveCube"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object_BlockCube(TEXT("Layer_BlockCube"))))
		return E_FAIL;



	return S_OK;
}

_int CScene_Stage2::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_RETURN) & DIS_Down) {
		if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGESELECT), SCENEID::SCENE_LOADING)))
			return E_FAIL;
	}

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
		case SCENEID::SCENE_STAGE2:
		{
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE2), SCENEID::SCENE_LOADING)))
				return E_FAIL;
			break;
		}
		case SCENEID::SCENE_STAGE3:
		{
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE1), SCENEID::SCENE_LOADING)))
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




#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("GameScene"));
#endif // _DEBUG


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

HRESULT CScene_Stage2::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_MainCamera(const _tchar * pLayerTag)
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

	if (FAILED(pMainCam->Reset_LookAtAxis(&CameraDesc)))
		return E_FAIL;

	_float3 ActionPos[4] = { _float3(5,5,0) ,_float3(5,-5,0) ,_float3(-5,5,0),_float3(-5,-5,0) };

	FAILED_CHECK(pMainCam->ReInitialize(ActionPos,4))


	pMainCam->Set_NowSceneNum(SCENE_STAGESELECT);
	pMainCam->CameraEffect(CCamera_Main::CAM_EFT_FADE_OUT, 0.016f);

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
	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_UI_Result")))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Cube(const _tchar * pLayerTag)
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
		(pPlayerList->front())->ReInitialize(&_float3(0, 2, 0));

	}

	return S_OK;
}
HRESULT CScene_Stage2::Ready_Layer_FixCube(const _tchar * pLayerTag)
{
	for (_uint x = 0; x < 20; ++x)
	{
		for (_uint z = 0; z < 20; ++z)
		{

			if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_TerrainCube), &_float3(-10.f + (_float)x, (_float)-1, (_float)z)))
				return E_FAIL;
		}
	}
	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_FixCube")))
	//	return E_FAIL;

	return S_OK;
}
HRESULT CScene_Stage2::Ready_Layer_PushCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_PushCube")))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_GravityCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_GravityCube")))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_SelfRotationCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_SelfRotationCube")))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Object_ButtonCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_ButtonCube")))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Object_InteractiveCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_InteractiveCube")))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Object_PortalCube_A(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_PortalCube_A")))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Object_PortalCube_B(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_PortalCube_B")))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Object_RisingCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_RisingCube"),&_float3(1.f, -1.f, 2.f)))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Object_EscalatorCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_EscalatorCube"),&_float3(-1.f, 2.f, 2.f)))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Object_LeftCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_LeftCube"), &_float3(-8.f, -1.f, -1.f)))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Object_RightCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_RightCube"), &_float3(-6.f, -1.f, -1.f)))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Object_VanishCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_VanishCube"), &_float3(9.f, -1.f, -1.f)))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Object_AppearCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_AppearCube"), &_float3(10.f, -1.f, -1.f)))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Object_BlockCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_BlockCube"), &_float3(-5.f, 0.f, 1.f)))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_OrbitCube(const _tchar * pLayerTag)
{
	CObject_OrbitCube::ORBITCUBEDESC OrbitCubeDesc;

	for (_uint Z = 0; Z < 2; Z++)
	{
		for (_uint Y = 0; Y < 2; Y++)
		{
			for (_uint X = 0; X < 2; X++) {
				OrbitCubeDesc.fTransform = _float3(5.f+X, 1.f + Y, 7.f+Z);
				OrbitCubeDesc.fRotAxis = _float3(0.f, 4.f, 0.f);
				if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_OrbitCube"),&OrbitCubeDesc))
					return E_FAIL;
				}
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
	__super::Free();
}
