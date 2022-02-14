#include "stdafx.h"
#include "..\Public\Scene_Loby.h"
#include "Scene_Loading.h"

#include "BackGround.h"



CScene_Loby::CScene_Loby(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
}





HRESULT CScene_Loby::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TAG_LAY(Layer_BackGround))))
		return E_FAIL;
	if (FAILED(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main))))
		return E_FAIL;

	return S_OK;
}

_int CScene_Loby::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (GetKeyState(VK_RETURN) & 0x8000)
	{

		if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGESELECT), SCENEID::SCENE_LOADING)))
			return E_FAIL;

	}

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		FAILED_CHECK(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_IMGUISCENE), SCENEID::SCENE_LOADING));
	}


	return 0;
}

_int CScene_Loby::LateUpdate(_float fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	return 0;
}

_int CScene_Loby::Render()
{
	if (__super::Render() < 0)
		return -1;

#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("LobyScene"));
#endif // _DEBUG

	return 0;
}

_int CScene_Loby::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}


HRESULT CScene_Loby::Ready_Layer_BackGround(const _tchar * pLayerTag)
{

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_LOBY, pLayerTag, TAG_OP(Prototype_BackGround)))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_Loby::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_LOBY, pLayerTag, TAG_OP(Prototype_Camera_Main)))
		return E_FAIL;
	return S_OK;
}

CScene_Loby * CScene_Loby::Create(LPDIRECT3DDEVICE9 GraphicDevice)
{
	CScene_Loby* pTempLoby = new CScene_Loby(GraphicDevice);

	if (FAILED(pTempLoby->Initialize())) 
	{
		Safe_Release(pTempLoby); 
		MSGBOX("Failed to Creating CScene_Loby");
		return nullptr;
	}

	return pTempLoby;

}

void CScene_Loby::Free()
{
	__super::Free();
}
