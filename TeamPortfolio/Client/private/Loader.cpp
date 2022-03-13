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
#include "PauseUI.h"
#include "UI_Status.h"
#include "UI_Start.h"
#include "Npc_ivy.h"
#include "Npc_oldy.h"
#include "Npc_geezer.h"
#include "Npc_izaac.h"
#include "UI_BossStageEntryUI.h"
#include "UI_BossStatusUI.h"
#include "Npc_izaacTuto.h"
#include "UI_Complete.h"

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
#include "Object_Star.h"
#include "Collision_Object.h"
#include "Collision_Object_StageEntry.h"
#include "Object_QrcodeCube.h"
#include "Effect_StageClear.h"

#include "ParticleObejct.h"

#include "MyButton.h"

#include "ParsedObject.h"
#include "ParsedObject_Blossoms.h"
#include "ParsedObject_Penguin.h"


#include "BossMonster.h"
#include "Bullet.h"
#include "Com_Gun.h"

#include "ParsedObject_BigGreenTree.h"
#include "ParsedObject_TreasureBox.h"
#include "ParsedObject_DeathSkull.h"
#include "ParsedObject_TombStone.h"
#include "ParsedObject_BigOwl.h"
#include "ParsedObject_BigOwlHead.h"
#include "ParsedObject_Bell.h"
#include "ParsedObject_LightHouse.h"
#include "ParsedObject_MiniTree.h"
#include "ParsedObject_ZuHeadWhite.h"
#include "ParsedObject_WindmillBlue.h"
#include "ParsedObject_WindmillStick.h"
#include "ParsedObject_BigWindmill.h"
#include "ParsedObject_WindmillCore.h"
#include "ParsedObject_Alien.h"
#include "ParsedObject_MapleTree.h"
#include "ParsedObject_TreeOfDeath.h"



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
	case SCENEID::SCENE_BOSS:
		pLoader->Load_Scene_Stage_Boss(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;		
	case SCENEID::SCENE_IMGUISCENE:
		pLoader->Load_Scene_IMGUI(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;
	case SCENEID::SCENE_TUTORIAL:
		pLoader->Load_Scene_TUTORIAL(tThreadArg.IsClientQuit, tThreadArg.CriSec);
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

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("UI_Result.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE_STATIC, TEXT("Prototype_Component_UI_Result_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("Particle.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE_STATIC, L"Prototype_Component_Texture_Particle", CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("JY_Effect.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE_STATIC, L"Prototype_Component_Texture_JY_Effect", CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("ClearEffect.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE_STATIC, L"Prototype_Component_Texture_Effect_StageClear", CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;
		
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("JH_Effect.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENE_STATIC, L"Prototype_Component_Texture_JH_Effect", CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	// 셰이더 프로토타입 생성
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Shader_Monster), CShader::Create(m_pGraphicDevice, TEXT("../Bin/ShaderFiles/Shader_Monster.hlsl")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Shader_Cube), CShader::Create(m_pGraphicDevice, TEXT("../Bin/ShaderFiles/Shader_TerrainCube.hlsl")))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Shader_Skual), CShader::Create(m_pGraphicDevice, TEXT("../Bin/ShaderFiles/Shader_Skual.hlsl")))))
		return E_FAIL;
	
	// 총 컴포넌트 생성
	if (FAILED(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Gun), CCom_Gun::Create(m_pGraphicDevice))))
		return E_FAIL;






	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_TreasureBox", pGameInstance->Create_ParsedObject(L"TreasureVertex.txt", L"TreasureIndex.txt")));
	////
	///////////////////////////////////////스테이지 셀렉 사용목록
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_MiniTree", pGameInstance->Create_ParsedObject(L"MiniTreeVertex.txt", L"MiniTreeIndex.txt")));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_Bell", pGameInstance->Create_ParsedObject(L"BellVertex.txt", L"BellIndex.txt")));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_MapleTree", pGameInstance->Create_ParsedObject(L"MapleTreeVertex.txt", L"MapleTreeIndex.txt")));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_BigGreenTree", pGameInstance->Create_ParsedObject(L"BigGreenTreeVertex.txt", L"BigGreenTreeIndex.txt")));
	////
	///////////////////////////////////////스테이지 튜토리얼 사용목록
	////
	/////////////////////////////////////스테이지 1 사용목록
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_Blossoms", pGameInstance->Create_ParsedObject(L"BlossomsVertex.txt", L"BlossomsIndex.txt")));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_WindmillBlue", pGameInstance->Create_ParsedObject(L"WindmillBlueVertex.txt", L"WindmillBlueIndex.txt")));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_WindmillStick", pGameInstance->Create_ParsedObject(L"WindmillStickVertex.txt", L"WindmillStickIndex.txt")));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_BigWindmill", pGameInstance->Create_ParsedObject(L"BigWindmillVertex.txt", L"BigWindmillIndex.txt")));



	///////////////////////////////////스테이지 2 사용목록
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_TombStone", pGameInstance->Create_ParsedObject(L"TombStoneVertex.txt", L"TombStoneIndex.txt")));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_BigOwl", pGameInstance->Create_ParsedObject(L"BigOwlVertex.txt", L"BigOwlIndex.txt")));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_BigOwlHead", pGameInstance->Create_ParsedObject(L"BigOwlHeadVertex.txt", L"BigOwlHeadIndex.txt")));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_WindmillCore", pGameInstance->Create_ParsedObject(L"WindmillCoreVertex.txt", L"WindmillCoreIndex.txt")));



	///////////////////////////////////스테이지 3 사용목록
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_Alien", pGameInstance->Create_ParsedObject(L"AlienVertex.txt", L"AlienIndex.txt")));
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_TreeOfDeath", pGameInstance->Create_ParsedObject(L"TreeOfDeathVertex.txt", L"TreeOfDeathIndex.txt")));

	///////////////////////////////////스테이지 보스 사용목록
	FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_DeathSkull", pGameInstance->Create_ParsedObject(L"DeathSkullVertex.txt", L"DeathSkullIndex.txt")));




	/////////////////////////////////////남은거
	//FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_WindmillCore", pGameInstance->Create_ParsedObject(L"WindmillCoreVertex.txt", L"WindmillCoreIndex.txt")));
	//FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_LightHouse", pGameInstance->Create_ParsedObject(L"LightHouseVertex.txt", L"LightHouseIndex.txt")));
	//FAILED_CHECK(pGameInstance->Add_Component_Prototype(SCENEID::SCENE_STATIC, L"Prototype_Component_VIBuffer_ZuHeadWhite", pGameInstance->Create_ParsedObject(L"ZuHeadWhiteVertex.txt", L"ZuHeadWhiteIndex.txt")));




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

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Player), CPlayer::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_BackGround), CBackGround::Create(m_pGraphicDevice))))
		return E_FAIL;

	// 큐브 오브젝트 프로토타입

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_TerrainCube), CTerrainCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_FixCube), CObject_FixCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_GravityCube), CObject_GravityCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PortalCube_A), CObject_PortalCube_A::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_PortalCube_B), CObject_PortalCube_B::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_EscalatorCube), CObject_EscalatorCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_OrbitButtonCube), CObject_OrbitButton::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_OrbitCube), CObject_OrbitCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_VanishCube), CObject_VanishCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_AppearCube), CObject_AppearCube::Create(m_pGraphicDevice))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Shop), CShop::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Quest), CQuest_Image::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_UI_Common), CUI_Common::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_UI_Result"), CUI_Result::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_StatusUI"), CUI_Status::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_PauseUI"), CPauseUI::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObject_Object_Star"), CObject_Star::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObject_Collision_Object"), CCollision_Object::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObject_Collision_Object_StageEntry"), CCollision_Object_StageEntry::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObject_Object_particle_Ball"), CParticleeObj_Ball::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObject_Object_particle_Cone"), CParticleeObj_Cone::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObject_Object_particle_Fountain"), CParticleeObj_Fountain::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObject_Object_particle_Straight"), CParticleeObj_Straight::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObject_Object_particle_Spread"), CParticleeObj_Spread::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObject_Object_particle_Fixed"), CParticleeObj_Fixed::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObject_Object_particle_Suck"), CParticleeObj_Suck::Create(m_pGraphicDevice))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObject_Effect_StageClear"), CEffect_StageClear::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, L"Mouse_UI", L"Prototype_Mouse_UI")))
		return E_FAIL;


	/////////////게임오브젝트 파서들
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Blossoms"), CParsedObject_Blossoms::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Penguin"), CParsedObject_Penguin::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_BigGreenTree"), CParsedObject_BigGreenTree::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_TreasureBox"), CParsedObject_TreasureBox::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_TombStone"), CParsedObject_TombStone::Create(m_pGraphicDevice))))
		return E_FAIL;
	
	//올빼미 머리 몸통 세트------
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_BigOwl"), CParsedObject_BigOwl::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_BigOwlHead"), CParsedObject_BigOwlHead::Create(m_pGraphicDevice))))
		return E_FAIL;
	//-------
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Bell"), CParsedObject_Bell::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_LightHouse"), CParsedObject_LightHouse::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_MiniTree"), CParsedObject_MiniTree::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_ZuHeadWhite"), CParsedObject_ZuHeadWhite::Create(m_pGraphicDevice))))
		return E_FAIL;

	//풍차들
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_WindmillBlue"), CParsedObject_WindmillBlue::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_WindmillStick"), CParsedObject_WindmillStick::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_BigWindmill"), CParsedObject_BigWindmill::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_WindmillCore"), CParsedObject_WindmillCore::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Alien"), CParsedObject_Alien::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_MapleTree"), CParsedObject_MapleTree::Create(m_pGraphicDevice))))
		return E_FAIL;
		
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_TreeOfDeath"), CParsedObject_TreeOfDeath::Create(m_pGraphicDevice))))		
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_DeathSkul"), CParsedObject_DeathSkull::Create(m_pGraphicDevice))))
		return E_FAIL;

	//////////////////////////////////

#pragma endregion

	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 1;
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


	//UI Texture
	CTexture::TEXTUREDESC TextureDesc;
	TextureDesc.szTextFilePath = TEXT("UI.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TAG_CP(Prototype_Texture_UI), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;
	//Particle
	TextureDesc.szTextFilePath = TEXT("Particle.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_Particle"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;
	//NPC Texture
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("NPC.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_NPC"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
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

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("PotalTexture.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_PotalTexture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//////////eunhyuk_UI
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("UI_Result.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_UI_Result_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("Parsed.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_Parsed"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////
	//FAILED_CHECK(pGameInstance->Add_Component_Prototype(m_eSceneID, L"Prototype_Component_VIBuffer_Penguin", pGameInstance->Create_ParsedObject(L"PenguinVertex.txt", L"PenguinIndex.txt")));
	//TextureDesc.szTextFilePath = TEXT("Penguine.txt");
	//if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, L"Prototype_Component_Texture_Penguin", CTexture::Create(m_pGraphicDevice, &TextureDesc))))
	//	return E_FAIL;
	////////////////////////////


#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT
	/////////////////////////////////////
	//if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Penguin"), CParsedObject::Create(m_pGraphicDevice))))
	//	return E_FAIL;
	
	
	//NPC
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Npc_ivy"), CNpc_ivy::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Npc_oldy"), CNpc_oldy::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Npc_geezer"), CNpc_geezer::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Npc_izaac"), CNpc_izaac::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Object_QrcodeCube"), CObject_QrcodeCube::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_UI_Complete"), CUI_Complete::Create(m_pGraphicDevice))))
		return E_FAIL;




#pragma endregion

	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 399999999;
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

	TextureDesc.szTextFilePath = TEXT("Pause.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_Pause"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("UI_Status.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_StatusUI"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//UI_Start
	TextureDesc.szTextFilePath = TEXT("UI_Start.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_StartUI"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//파서들
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("Parsed.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, L"Prototype_Component_Texture_Parsed", CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;


	/////////////튜토리얼을 위한 내용들
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("GravityTexture.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_GravityTexture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("PotalTexture.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_PotalTexture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;




	////////////////////
#pragma endregion


#pragma  region PROTOTYPE_GAMEOBJECT

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_UI_Start"), CUI_Start::Create(m_pGraphicDevice))))
		return E_FAIL;

#pragma endregion


	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 399999999;
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

HRESULT CLoader::Load_Scene_Stage2(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{

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

	TextureDesc.szTextFilePath = TEXT("Pause.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_Pause"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("UI_Status.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_StatusUI"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;


	//UI_Start
	TextureDesc.szTextFilePath = TEXT("UI_Start.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_StartUI"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.szTextFilePath = TEXT("Parsed.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, L"Prototype_Component_Texture_Parsed", CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;



#pragma endregion


#pragma  region PROTOTYPE_GAMEOBJECT

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_UI_Start"), CUI_Start::Create(m_pGraphicDevice))))
		return E_FAIL;
	

#pragma endregion


	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 399999999;
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
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//프로토타입 컴포넌트#######################################################################################################
#pragma region PROTOTYPE_COMPONENT

	CTexture::TEXTUREDESC TextureDesc;
	//////////eunhyuk_UI
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("UI_Result.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_UI_Result_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//UI_Start
	TextureDesc.szTextFilePath = TEXT("UI_Start.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_StartUI"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//미는 큐브
	TextureDesc.szTextFilePath = TEXT("Cube_Texture.txt");
	TextureDesc.eTextureType = CTexture::TYPE_CUBEMAP;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Cube_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.szTextFilePath = TEXT("Pause.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_Pause"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("UI_Status.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_StatusUI"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("GravityTexture.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_GravityTexture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.szTextFilePath = TEXT("PotalTexture.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_PotalTexture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("Parsed.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_Parsed"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("Alien_Effect.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_Alien_Effect"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("DeathTree_Effect.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_DeathTree_Effect"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;




#pragma endregion


#pragma  region PROTOTYPE_GAMEOBJECT

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_UI_Start"), CUI_Start::Create(m_pGraphicDevice))))
		return E_FAIL;

#pragma endregion


	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 399999999;
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

HRESULT CLoader::Load_Scene_Stage_Boss(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//프로토타입 컴포넌트#######################################################################################################
#pragma region PROTOTYPE_COMPONENT

	CTexture::TEXTUREDESC TextureDesc;
	//////////eunhyuk_UI
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("UI_Result.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_UI_Result_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//UI_Start
	TextureDesc.szTextFilePath = TEXT("UI_Start.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_StartUI"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//미는 큐브
	TextureDesc.szTextFilePath = TEXT("Cube_Texture.txt");
	TextureDesc.eTextureType = CTexture::TYPE_CUBEMAP;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Cube_Texture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.szTextFilePath = TEXT("Pause.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_Pause"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("UI_Status.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_StatusUI"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("GravityTexture.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_GravityTexture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.szTextFilePath = TEXT("PotalTexture.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_PotalTexture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("Parsed.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_Parsed"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	// 몬스터 텍스처
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("Monster.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TAG_CP(Prototype_Texture_Monster), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	////////////////////
	///////////////보스 엔트리용///////////////////////////////////////////////////////////
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("UI_BossEntry.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_BossEntry"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("BossStatusUI.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_BossStatusUI"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

#pragma endregion


#pragma  region PROTOTYPE_GAMEOBJECT
	// 몬스터
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Boss),
		CBossMonster::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Bullet),
		CBullet::Create(m_pGraphicDevice,nullptr))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_BossEntry"), CUI_BossStageEntryUI::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_BossStatusUI"), CUI_BossStatusUI::Create(m_pGraphicDevice))))
		return E_FAIL;

#pragma endregion


	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 399999999;
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

HRESULT CLoader::Load_Scene_TUTORIAL(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
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

	TextureDesc.szTextFilePath = TEXT("Pause.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_Pause"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("UI_Status.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_StatusUI"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	//UI_Start
	TextureDesc.szTextFilePath = TEXT("UI_Start.txt");
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_StartUI"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("NPC.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_NPC"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;
	

	/////////////튜토리얼을 위한 내용들
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("GravityTexture.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_GravityTexture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;

	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("PotalTexture.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_PotalTexture"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;


	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("DeathTree_Effect.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_Alien_Effect"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("DeathTree_Effect.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, TEXT("Prototype_Component_Texture_DeathTree_Effect"), CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;



	/////////////////파서들 실험중
	//벚꽃
	/////txt 하나만 있으면 돌아가게 만들어둠
	TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	TextureDesc.szTextFilePath = TEXT("Parsed.txt");
	if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, L"Prototype_Component_Texture_Parsed", CTexture::Create(m_pGraphicDevice, &TextureDesc))))
		return E_FAIL;


	////펭귄
	//TextureDesc.szTextFilePath = TEXT("Parsed.txt");
	//if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, L"Prototype_Component_Texture_Parsed", CTexture::Create(m_pGraphicDevice, &TextureDesc))))
	//	return E_FAIL;

	//파티클
	//TextureDesc.eTextureType = CTexture::TYPE_DEFAULT;
	//TextureDesc.szTextFilePath = TEXT("Particle.txt");
	//if (FAILED(pGameInstance->Add_Component_Prototype(m_eSceneID, L"Prototype_Component_Texture_Particle", CTexture::Create(m_pGraphicDevice, &TextureDesc))))
	//	return E_FAIL;

	//////////////
#pragma endregion


#pragma  region PROTOTYPE_GAMEOBJECT

	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_UI_Start"), CUI_Start::Create(m_pGraphicDevice))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Npc_izaacTuto"), CNpc_izaacTuto::Create(m_pGraphicDevice))))
		return E_FAIL;




#pragma endregion


	RELEASE_INSTANCE(CGameInstance);
	EnterCriticalSection(_CriSec);
	m_iLoadingMaxCount = 399999999;
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

HRESULT CLoader::Load_Scene_IMGUI(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTexture::TEXTUREDESC TextureDesc;

#pragma region PROTOTYPE_COMPONENT

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


#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT


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
