#include "stdafx.h"
#include "..\Public\Scene_Stage2.h"
#include "Object_OrbitCube.h"
#include "Object_EscalatorCube.h"
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
	////////////////////////////은혁이 테스트
	if (FAILED(Ready_Layer_Cube(TEXT("Layer_Cube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_FixCube(TEXT("Layer_FixCube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_PushCube(TEXT("Layer_PushCube"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_GravityCube(TEXT("Layer_GravityCube"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_SelfRotationCube(TEXT("Layer_SelfRotationCube"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object_ButtonCube(TEXT("Layer_ButtonCube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Object_InteractiveCube(TEXT("Layer_InteractiveCube"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_OrbitCube(TEXT("Layer_OrbitCube"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object_PortalCube_A(TEXT("Layer_PortalCube_A"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Object_PortalCube_B(TEXT("Layer_PortalCube_B"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object_RisingCube(TEXT("Layer_RisingCube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Object_DescentCube(TEXT("Layer_DescentCube"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object_LeftCube(TEXT("Layer_LeftCube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Object_RightCube(TEXT("Layer_RightCube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Object_VanishCube(TEXT("Layer_VanishCube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Object_AppearCube(TEXT("Layer_AppearCube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Object_BlockCube(TEXT("Layer_BlockCube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Object_EscalatorCube(TEXT("Layer_EscalatorCube"))))
		return E_FAIL;


	return S_OK;
}

_int CScene_Stage2::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;



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

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_Camera_Main), &CameraDesc))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Cube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_MoveCube")))
		return E_FAIL;

	return S_OK;
}
HRESULT CScene_Stage2::Ready_Layer_FixCube(const _tchar * pLayerTag)
{
	for (_uint x = 0; x < 20; ++x)
	{
		for (_uint z = 0; z < 20; ++z)
		{
			if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_FixCube"), &_float3(-10.f+(_float)x, (_float)-1, (_float)z)))
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

HRESULT CScene_Stage2::Ready_Layer_Object_DescentCube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_DescentCube"),&_float3(-1.f, 2.f, 2.f)))
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
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_BlockCube")))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Stage2::Ready_Layer_Object_EscalatorCube(const _tchar * pLayerTag)
{
	CObject_EscalatorCube::ESCALATORCUBEDESC EscalatorDesc;

	EscalatorDesc.EscalatorCubePos = _float3(2.f, 0.f, -1.f);
	EscalatorDesc.DestPos = _float3(4.f, 4.f, -2.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_EscalatorCube"),&EscalatorDesc))
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
