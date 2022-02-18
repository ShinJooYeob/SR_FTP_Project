#include "stdafx.h"
#include "..\Public\Loader.h"

#include "BackGround.h"
#include "TerrainGround.h"
#include "Player.h"
#include "Shop.h"
#include "TerrainCube.h"
#include "Object_MoveCube.h"
#include "Object_FixCube.h"
#include "Object_PushCube.h"
#include "Object_GravityCube.h"

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


	if (FAILED(GetSingle(CGameInstance)->Add_GameObject_Prototype(TAG_OP(Prototype_BackGround), CBackGround::Create(m_pGraphicDevice))))
		return E_FAIL;

	m_bIsLoadingFinished = true;

	return S_OK;
}

HRESULT CLoader::Load_Scene_StageSelect(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


#pragma region PROTOTYPE_COMPONENT

	//버퍼인덱스 프로토타입 생성
	CVIBuffer_Terrain::TERRAINDESC TerrainDesc;
	TerrainDesc.szHeightFilePath = TEXT("../Bin/Resources/Textures/Terrain/Height.bmp");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TAG_CP(Prototype_VIBuffer_Terrain128x128), CVIBuffer_Terrain::Create(m_pGraphicDevice, &TerrainDesc))))
		return E_FAIL;

	//Player Texture
	CTexture::TEXTUREDESC TextureDesc;
	TextureDesc.szFilePath = TEXT("../Bin/Resources/Textures/Player/walk/walk_%d.png");
	TextureDesc.iNumTexture = 6;
	TextureDesc.iStartIndex = 1;

	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TAG_CP(Prototype_Texture_Player), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;
	
	//Terrain Texture
	TextureDesc.szFilePath = TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.tga");
	TextureDesc.iNumTexture = 2;
	TextureDesc.iStartIndex = 0;

	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TAG_CP(Prototype_Texture_Terrain), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//Shop Texture
	TextureDesc.szFilePath = TEXT("../Bin/Resources/Textures/UI/drapes_ground.png");
	TextureDesc.iNumTexture = 1;

	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TAG_CP(Prototype_Texture_Shop), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	

#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_TerrainGround), CTerrainGround::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Player), CPlayer::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Shop), CShop::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_TerrainCube), CTerrainCube::Create(m_pGraphicDevice))))
		return E_FAIL;

	
#pragma endregion


	RELEASE_INSTANCE(CGameInstance);

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

	///////////////////////////////////박은혁 테스트 장소
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


#pragma region PROTOTYPE_COMPONENT

	CTexture::TEXTUREDESC TextureDesc;

	TextureDesc.szFilePath = TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.tga");
	TextureDesc.iNumTexture = 1;
	TextureDesc.iStartIndex = 0;

	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Object_FixCube_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.szFilePath = TEXT("../Bin/Resources/Textures/UI/drapes_ground.png");
	TextureDesc.iNumTexture = 1;
	TextureDesc.iStartIndex = 1;

	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Object_PushCube_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.szFilePath = TEXT("../Bin/Resources/Textures/Terrain/Brush.png");
	TextureDesc.iNumTexture = 1;
	TextureDesc.iStartIndex = 1;

	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Object_GravityCube_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.szFilePath = TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds");
	TextureDesc.eTextureType = CTexture::TYPE_CUBEMAP;
	TextureDesc.iNumTexture = 4; //텍스쳐 갯수
	TextureDesc.iStartIndex = 0; //인덱스 시작 번호 FEZ는 1로 되어 있어서 이렇게 씀

	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_Sky"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

#pragma endregion



#pragma  region PROTOTYPE_GAMEOBJECT
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_MoveCube"), CObject_MoveCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_FixCube"), CObject_FixCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_PushCube"), CObject_PushCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_GravityCube"), CObject_GravityCube::Create(m_pGraphicDevice))))
		return E_FAIL;

#pragma endregion


	RELEASE_INSTANCE(CGameInstance);

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
