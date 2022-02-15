#include "stdafx.h"
#include "..\Public\Scene_StageSelect.h"
#include "Camera_Main.h"



CScene_StageSelect::CScene_StageSelect(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
}

HRESULT CScene_StageSelect::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TAG_LAY(Layer_Terrain))))
		return E_FAIL;
	if (FAILED(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Player(TAG_LAY(Layer_Player))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Shop(TAG_LAY(Layer_Shop))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Cube(TEXT("Layer_Cube"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_FixCube(TEXT("Layer_FixCube"))))
	//	return E_FAIL;


	//ZeroMemory(&m_Light, sizeof(m_Light));
	//m_Light.Type = D3DLIGHT_DIRECTIONAL;

	//m_Light.Diffuse.r = m_Light.Diffuse.g = m_Light.Diffuse.b = 1;
	//m_Light.Specular = m_Light.Diffuse;

	//m_Light.Ambient.g = m_Light.Ambient.b = m_Light.Ambient.r = 0;
	//m_Light.Ambient.a = 1;
	//m_Light.Direction = _float3(-0.1f, 0.4f, 0.4f).Get_Nomalize();

	//m_pGraphicDevice->SetLight(0, &m_Light);
	//m_pGraphicDevice->LightEnable(0, TRUE);


	//m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRS_AMBIENT, 0xffffff);
	//m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}

_int CScene_StageSelect::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;



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

HRESULT CScene_StageSelect::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	for (_uint k = 0; k < 5; k++)
	{
		for (_uint i = k; i < 10 - k; i++)
		{
			for (_uint j = k; j < 10 - k; j++) {

				if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_TerrainCube), &_float3((_float)i, (_float)k, (_float)j)))
					return E_FAIL;
			}
		}

	}

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_TerrainCube), &_float3((_float)5, (_float)1, (_float)0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.vEye = _float3(5.f, 3.f, -10.f);
	CameraDesc.vAt = CameraDesc.vWorldRotAxis =  _float3(5.f, 3.f, 5.f);

	CameraDesc.vAxisY = _float3(0, 1, 0);

	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fMovePerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_Camera_Main),&CameraDesc))
		return E_FAIL;
	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_Player(const _tchar * pLayerTag)
{

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TAG_OP(Prototype_Player)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_Shop(const _tchar * pLayerTag)
{

	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_Shop")))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_Cube(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_TestCubeMove")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_StageSelect::Ready_Layer_FixCube(const _tchar * pLayerTag)
{


	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGESELECT, pLayerTag, TEXT("Prototype_GameObject_TestCubeFixObject")))
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
