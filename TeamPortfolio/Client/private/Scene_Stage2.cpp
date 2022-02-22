#include "stdafx.h"
#include "..\Public\Scene_Stage2.h"
#include "Object_OrbitCube.h"

CScene_Stage2::CScene_Stage2(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
}

HRESULT CScene_Stage2::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	////////////////////////////������ �׽�Ʈ
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
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TAG_OP(Prototype_Camera_Main)))
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
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE2, pLayerTag, TEXT("Prototype_GameObject_Object_FixCube")))
		return E_FAIL;

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
