#include "stdafx.h"
#include "..\Public\Loader.h"

#include "BackGround.h"
#include "SkyBox.h"
#include "TerrainGround.h"
#include "Player.h"
#include "Shop.h"
#include "UI_Loby.h"
#include "Quest_Image.h"
#include "UI_Common.h"
#include "TerrainCube.h"
#include "LobyCube.h"
#include "LobyPlayer.h"
#include "LobySkyBox.h"
#include "LobyScroll.h"
#include "Loby_UI.h"

#include "Object_FixCube.h"
#include "Object_GravityCube.h"
#include "Object_OrbitButton.h"
#include "Object_OrbitCube.h"
#include "Object_PortalCube_A.h"
#include "Object_PortalCube_B.h"
#include "Object_EscalatorCube.h"
#include "Object_MoveCube.h"
#include "Object_PushCube.h"
#include "Object_SelfRotationCube.h"
#include "Object_ButtonCube.h"
#include "Object_InteractiveCube.h"
#include "Object_VanishCube.h"
#include "Object_AppearCube.h"
#include "Object_BlockCube.h"
#include "UI_Result.h"

#include "MyButton.h"


_uint CALLBACK LoadingThread(void* _Prameter)
{
	THREADARG tThreadArg{};
	memcpy(&tThreadArg, _Prameter, sizeof(THREADARG));
	delete _Prameter;


	CLoader* pLoader = (CLoader*)(tThreadArg.pArg);


	switch (pLoader->Get_SceneID())
	{
	case SCENEID::SCENE_LOBY:
		pLoader->Load_Scene_Loby(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
	case SCENEID::SCENE_STAGESELECT:
		pLoader->Load_Scene_StageSelect(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
	case SCENEID::SCENE_STAGE1:
		pLoader->Load_Scene_Stage1(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
	case SCENEID::SCENE_STAGE2:
		pLoader->Load_Scene_Stage2(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
	case SCENEID::SCENE_STAGE3:
		pLoader->Load_Scene_Stage3(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
	case SCENEID::SCENE_IMGUISCENE:
		pLoader->Load_Scene_IMGUI(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;


	default:
		MSGBOX("Failed to Create Scene");
		break;
	}

	return 0;
}


CLoader::CLoader()
{
}



HRESULT CLoader::Initialize(LPDIRECT3DDEVICE9 pGraphicDevice, SCENEID eSceneID)
{
	m_eSceneID = eSceneID;
	m_bIsLoadingFinished = false;
	m_pGraphicDevice = pGraphicDevice;
	Safe_AddRef(m_pGraphicDevice);

	GetSingle(CGameInstance)->PlayThread(LoadingThread, this);

	return S_OK;
}

HRESULT CLoader::Load_Scene_Loby(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pGameInstance = GetSingle(CGameInstance);

#pragma region PROTOTYPE_COMPONENT

	CTexture::TEXTUREDESC TextureDesc;
	TextureDesc.szTextFilePath = TEXT("UI.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TAG_CP(Prototype_Texture_UI), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.szTextFilePath = TEXT("LobyCubeTex.txt");
	TextureDesc.eTextureType = CTexture::TYPE_CUBEMAP;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_LobyCube_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("LobyPlayer.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_LobyPlayerTexture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("LobyScroll.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_LobyScroll"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("LobyUI.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_LobyUI"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;


#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_LobyCube"), CLobyCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_LobyPlayer"), CLobyPlayer::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_LobyScroll"), CLobyScroll::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_LobySkyBox"), CLobySkyBox::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_LobyUI"), CLoby_UI::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_SkyBox"), CSkyBox::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, L"Mouse_UI", L"Prototype_Mouse_UI")))
		return E_FAIL;


#pragma endregion

	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 99999999;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	for (int i = 0; i < m_iLoadingMaxCount; ++i)
	{
		EnterCriticalSection(_CriSec);
		m_iLoadingProgressCount = i;
		LeaveCriticalSection(_CriSec);
	}

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);

	return S_OK;
}

HRESULT CLoader::Load_Scene_StageSelect(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


#pragma region PROTOTYPE_COMPONENT

	//¹öÆÛÀÎµ¦½º ÇÁ·ÎÅäÅ¸ÀÔ »ý¼º
	CVIBuffer_Terrain::TERRAINDESC TerrainDesc;
	TerrainDesc.szHeightFilePath = TEXT("../Bin/Resources/Textures/Terrain/Height.bmp");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TAG_CP(Prototype_VIBuffer_Terrain128x128), CVIBuffer_Terrain::Create(m_pGraphicDevice, &TerrainDesc))))
		return E_FAIL;

	//UI Texture
	CTexture::TEXTUREDESC TextureDesc;
	TextureDesc.szTextFilePath = TEXT("UI.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TAG_CP(Prototype_Texture_UI), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//Player Texture
	TextureDesc.szTextFilePath = TEXT("Cube_Texture.txt");
	TextureDesc.eTextureType = CTexture::TYPE_CUBEMAP;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Cube_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("GravityTexture.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_GravityTexture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.szTextFilePath = TEXT("PotalTexture.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_PotalTexture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//////////eunhyuk_UI
	TextureDesc.szTextFilePath = TEXT("UI_Result.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_UI_Result_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	

#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_TerrainGround), CTerrainGround::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Player), CPlayer::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Shop), CShop::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Quest), CQuest_Image::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_UI_Common), CUI_Common::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_FixCube"), CObject_FixCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_GravityCube"), CObject_GravityCube::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_PortalCube_A"), CObject_PortalCube_A::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_PortalCube_B"), CObject_PortalCube_B::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_TerrainCube), CTerrainCube::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_EscalatorCube"), CObject_EscalatorCube::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_OrbitButton"), CObject_OrbitButton::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_OrbitCube"), CObject_OrbitCube::Create(m_pGraphicDevice))))
		return E_FAIL;	
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_VanishCube"), CObject_VanishCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_AppearCube"), CObject_AppearCube::Create(m_pGraphicDevice))))
		return E_FAIL;

	//eunhyuk_UI
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_UI_Result"), CUI_Result::Create(m_pGraphicDevice))))
		return E_FAIL;
#pragma endregion

	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 99999999;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	for (int i = 0; i < m_iLoadingMaxCount; ++i)
	{
		EnterCriticalSection(_CriSec);
		m_iLoadingProgressCount = i;
		LeaveCriticalSection(_CriSec);
	}

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	return S_OK;
}

HRESULT CLoader::Load_Scene_Stage1(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	m_bIsLoadingFinished = true;

	return S_OK;
}

HRESULT CLoader::Load_Scene_Stage2(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{

	///////////////////////////////////¹ÚÀºÇõ Å×½ºÆ® Àå¼Ò
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//프로토타입 컴포넌트#######################################################################################################
#pragma region PROTOTYPE_COMPONENT

	CTexture::TEXTUREDESC TextureDesc;
	//////////eunhyuk_UI
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("UI_Result.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_UI_Result_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//미는 큐브
	TextureDesc.szTextFilePath = TEXT("Cube_Texture.txt");
	TextureDesc.eTextureType = CTexture::TYPE_CUBEMAP;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Cube_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

#pragma endregion


	//프로토타입_게임 오브젝트@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#pragma  region PROTOTYPE_GAMEOBJECT

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_MoveCube"), CObject_MoveCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_PushCube"), CObject_PushCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_SelfRotationCube"), CObject_SelfRotationCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_ButtonCube"), CObject_ButtonCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_InteractiveCube"), CObject_InteractiveCube::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_BlockCube"), CObject_BlockCube::Create(m_pGraphicDevice))))
		return E_FAIL;


#pragma endregion


	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 99999999;
	m_iLoadingProgressCount = 0;
	LeaveCriticalSection(_CriSec);

	for (int i = 0; i < m_iLoadingMaxCount; ++i)
	{
		EnterCriticalSection(_CriSec);
		m_iLoadingProgressCount = i;
		LeaveCriticalSection(_CriSec);
	}

	EnterCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	LeaveCriticalSection(_CriSec);
	m_bIsLoadingFinished = true;
	return S_OK;
}

HRESULT CLoader::Load_Scene_Stage3(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	m_bIsLoadingFinished = true;

	return S_OK;
}

HRESULT CLoader::Load_Scene_IMGUI(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTexture::TEXTUREDESC TextureDesc;

#pragma region PROTOTYPE_COMPONENT

	TextureDesc.szTextFilePath = TEXT("Cube_Texture.txt");
	TextureDesc.eTextureType = CTexture::TYPE_CUBEMAP;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Cube_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;
#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Player), CPlayer::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_TerrainCube), CTerrainCube::Create(m_pGraphicDevice))))
		return E_FAIL;

#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	m_bIsLoadingFinished = true;	

	return S_OK;
}



CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphicDevice, SCENEID eSceneID)
{
	CLoader* pTempLoader = new CLoader;

	if (FAILED(pTempLoader->Initialize(pGraphicDevice,eSceneID)))
	{
		Safe_Release(pTempLoader);
		MSGBOX("Failed to Create CLoader");
		return nullptr;
	}

	return pTempLoader;

}

void CLoader::Free()
{
	Safe_Release(m_pGraphicDevice);
	
}
