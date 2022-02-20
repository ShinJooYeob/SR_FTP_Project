#include "stdafx.h"
#include "..\Public\Scene_Stage2.h"

CScene_Stage2::CScene_Stage2(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
}

HRESULT CScene_Stage2::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	////////////////////////////은혁이 테스트
	if (FAILED(Ready_Layer_Cube(TEXT("Layer_Cube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_FixCube(TEXT("Layer_Fix_Cube"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_PushCube(TEXT("Layer_Push_Cube"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_GravityCube(TEXT("Layer_Gravity_Cube"))))
		return E_FAIL;
	////////////////////////////////////////////////////////////////////

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
