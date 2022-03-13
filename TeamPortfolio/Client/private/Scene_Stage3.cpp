#include "stdafx.h"
#include "..\Public\Scene_Stage3.h"
#include "Scene_Loading.h"

#include "Camera_Main.h"
#include "MapLoadMgr.h"
#include "UI_Status.h"
#include "Object_PortalCube_A.h"
#include "Object_EscalatorCube.h"
#include "Object_OrbitButton.h"
#include "..\public\UI_Result.h"
#include "..\public\Object_Star.h"


CScene_Stage3::CScene_Stage3(LPDIRECT3DDEVICE9 GraphicDevice)
	:CScene(GraphicDevice)
{
}

HRESULT CScene_Stage3::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main))))
		return E_FAIL;
	if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
		return E_FAIL;

	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_Player)));
	FAILED_CHECK(Ready_Layer_PauseUI(TEXT("Layer_PauseUI")));
	FAILED_CHECK(Ready_Layer_UI_Result(TEXT("Layer_UI_Result")));
	FAILED_CHECK(Ready_Layer_UI_Start(TEXT("Layer_UI_Start")));
	FAILED_CHECK(Ready_Layer_PlayerStatusUI(TEXT("Layer_StatusUI")));
	FAILED_CHECK(Ready_Layer_StageEndCollsionObject(TEXT("Layer_Collision_StageEnd")));
	//	FAILED_CHECK(Ready_Layer_Object_Star(TEXT("Layer_Object_Star")));
	//	FAILED_CHECK(Ready_Layer_OrbitButton_And_Cube(TEXT("Layer_OrbitButton")));

		// FAILED_CHECK(Ready_Layer_Object_particle(TEXT("Layer_Particle")));
	Createparticle_Scene3();

	// 로드된 오브젝트 정보로 그리기

	// 생성되지 않는 특수 큐브 저장
	list< SPECIALCUBE*> SpecialCubeList;
	GetSingle(CMapLoadMgr)->LoadMap(SCENEID::SCENE_STAGE3, 3, &SpecialCubeList);

	FAILED_CHECK(Ready_Layer_Terrain(SCENEID::SCENE_STAGE3, &SpecialCubeList));

	for (auto data : SpecialCubeList)
	{
		Safe_Delete(data);
	}
	SpecialCubeList.clear();
	;
	GetSingle(CGameInstance)->PlayBGM(L"JH_Stage3_BGM.mp3");
	GetSingle(CGameInstance)->Channel_VolumeUp(CHANNEL_BGM, 0.5f);


	FAILED_CHECK(Ready_Layer_ParsedAlienObject(L"Layer_ParsedObject"));
	FAILED_CHECK(Ready_Layer_ParsedDeathTreeObject(L"Layer_ParsedObject"));

	return S_OK;
}

_int CScene_Stage3::Update(_float fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	//if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_RETURN) & DIS_Down) {
	//	if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGESELECT), SCENEID::SCENE_LOADING)))
	//		return E_FAIL;
	//}

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
		case SCENEID::SCENE_STAGE3:
		{
			if (FAILED(GetSingle(CGameInstance)->Scene_Change(CScene_Loading::Create(m_pGraphicDevice, SCENEID::SCENE_STAGE3), SCENEID::SCENE_LOADING)))
				return E_FAIL;
			break;
		}
		}
	}
	else {

		Update_MapEffect(fDeltaTime);

	}

	return 0;
}

_int CScene_Stage3::LateUpdate(_float fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	return 0;
}

_int CScene_Stage3::Render()
{
	if (__super::Render() < 0)
		return -1;

//#ifdef _DEBUG
//	SetWindowText(g_hWnd, TEXT("GameScene"));
//#endif // _DEBUG
//
	return 0;
}

_int CScene_Stage3::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

HRESULT CScene_Stage3::Scene_InGame_Chage(_bool Scene_Chage_Switch, _uint _INextScene)
{
	m_bScene_Switch = Scene_Chage_Switch;
	m_INextScene = _INextScene;

	return S_OK;
}

HRESULT CScene_Stage3::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;

	CameraDesc.bIsOrtho = true;
	CameraDesc.vWorldRotAxis = _float3(0.f, 3.f, 0.f);
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

	_float3 ActionPos[5] = { _float3(4, 25, -16) ,_float3(3, 35, -4) ,_float3(13, 69, -6),_float3(-2, 86, 14),_float3(2, 105, -5) };

	FAILED_CHECK(pMainCam->ReInitialize(ActionPos, 5));

	pMainCam->Set_NowSceneNum(SCENE_STAGE3);
	pMainCam->CameraEffect(CCamera_Main::CAM_EFT_FADE_OUT, g_fDeltaTime, 4.f);

	return S_OK;
}

HRESULT CScene_Stage3::Ready_Layer_SkyBox(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("Prototype_GameObject_SkyBox")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage3::Ready_Layer_UI_Result(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("Prototype_GameObject_UI_Result")))
		return E_FAIL;

	CUI_Result* pResult = (CUI_Result*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGE3, TEXT("Layer_UI_Result")));
	if (pResult == nullptr)
		return E_FAIL;
	pResult->Set_MaxTime(300.f);

	return S_OK;
}

HRESULT CScene_Stage3::Ready_Layer_PlayerStatusUI(const _tchar * pLayerTag)
{
	list<CGameObject*>* pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, TAG_LAY(Layer_Player));

	NULL_CHECK_BREAK(pPlayerList);

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_STAGE3, pLayerTag, TEXT("Prototype_GameObject_StatusUI")))
		return E_FAIL;

	CUI_Status* pStatusUI = (CUI_Status*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGE3, TEXT("Layer_StatusUI")));

	if (pStatusUI == nullptr)
		return E_FAIL;

	FAILED_CHECK(pStatusUI->Set_Player(pPlayerList->front()));

	CUI_Result* pResult = (CUI_Result*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGE3, TEXT("Layer_UI_Result")));
	if (pResult == nullptr)
		return E_FAIL;
	FAILED_CHECK(pStatusUI->Set_ResultUI(pResult));

	return S_OK;
}

HRESULT CScene_Stage3::Ready_Layer_UI_Start(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("Prototype_GameObject_UI_Start")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage3::Ready_Layer_Object_Star(const _tchar * pLayerTag)
{
	CObject_Star::STARDESC StarDesc;

	StarDesc.fTransform = _float3(0, 10.f, -2.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	StarDesc.fTransform = _float3(13.f, -14.f, -15.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	StarDesc.fTransform = _float3(-7.f, -10.f, -20.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	StarDesc.fTransform = _float3(-10.f, 19.f, -12.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	StarDesc.fTransform = _float3(0.f, 32.f, -26.f);
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage3::Ready_Layer_Object_particle(const _tchar * pLayerTag)
{
	PARTICLEDESC tDesc;
	//파티클이 흩날리는 종류 설정
	tDesc.eParticleID = Particle_Spread;

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 20.f;
	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 1.f;

	//파티클의 사이즈를 설정
	tDesc.ParticleSize = _float3(0.3f, 0.3f, 0.3f);
	//파티클의 파워(이동속도)를 결정
	tDesc.Particle_Power = 1;
	//파티클의 파워(이동속도)의 랜덤 범위를 결정
	tDesc.PowerRandomRange = _float2(0.9f, 1.1f);

	//파티클이 한번에 최대 몇개까지 보일 것인지 설정
	tDesc.MaxParticleCount = 30;

	//파티클 텍스처 컴포넌트 이름을 설정 (기본적으로 자기 씬에 컴포넌트가 있는지 검사하고 스테틱에있는지도 검사함)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_UI_Result_Texture");
	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("RankStar");
	//텍스처 오토프레임을 사용할 것인지 말 것인지 결정
	tDesc.m_bIsTextureAutoFrame = true;
	tDesc.fAutoFrameMul = 3.f;
	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	tDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player))->Get_Component(TAG_COM(Com_Transform)));
	//tDesc.FixedTarget = _float3(0,3,0);

	//파티클의 최대 이탈 범위(range)를 설정해 줌
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	tDesc.MaxBoundary = _float3(10, 10, 10);

	//텍스처의 색상을 변경할 수 있는 기능 온오프
	//만약 true로 사용할 경우 텍스처의 원래 색상은 무시되고 타겟 색상으로 반짝반짝 거리게 설정됨
	//true로 사용할 경우 반드시 타겟 컬러를 설정해 줄 것
	tDesc.ParticleColorChage = false;
	tDesc.TargetColor = _float3(237, 28, 36);
	tDesc.TargetColor2 = _float3(53, 255.f, 11);

	//만약 UI에 그려져야한다면 true 월드에 그려져야한다면 false 로 설정할 것
	//UI 로 그리게 될 경우 위의 모든 좌표는 API 좌표 기준으로 셋팅할 것
	//World로 그리게 될 경우 위의 모든 좌표는 월드 좌표 기준으로 셋팅할 것
	tDesc.m_bIsUI = false;

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;
	tDesc.vUp = _float3(0, 0, -1);

	tDesc.ParticleStartRandomPosMin = _float3(-10.0f, 0.0f, -10.1f);
	tDesc.ParticleStartRandomPosMax = _float3(10.1f, 0.5f, 10.1f);

	tDesc.MustDraw = true;
	//오브젝트 뒤에 가려지지 않게 만듬

	tDesc.IsParticleFameEndtoDie = false;
	//프레임이 한번만 돌것인지 정함

	//Create_ParticleObject를 호출하여 스테이지 아이디와 지금까지 설정한 desc를 넣어주면 됨
	GetSingle(CParticleMgr)->Create_ParticleObject(SCENE_STAGE3, tDesc);

	return S_OK;
}

HRESULT CScene_Stage3::Ready_Layer_OrbitButton_And_Cube(const _tchar * pLayerTag)
{
	CObject_OrbitButton::ORBITDESC tDesc;

	tDesc.vButtonPos = _float3(-7, -8, 2);
	tDesc.vOrbitRotAxis = _float3(-7, -8, -7);
	tDesc.vOribitCubeStartPos = _float3(-7, -11, 3);
	tDesc.vOribitTotalXYZ = _float3(2, 1, 4);

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("Prototype_GameObject_Object_OrbitButton"), &tDesc))
		return E_FAIL;

	tDesc.vButtonPos = _float3(-19, 12, 13);
	tDesc.vOrbitRotAxis = _float3(-19, -2, -2);
	tDesc.vOribitCubeStartPos = _float3(-19, 9, 16);
	tDesc.vOribitTotalXYZ = _float3(1, 1, 3);

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("Prototype_GameObject_Object_OrbitButton"), &tDesc))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage3::Ready_Layer_ParsedAlienObject(const _tchar * pLayerTag)
{

	_float3 TransformPos;

	TransformPos = _float3(7, -1, 10);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_Alien", TEXT("Prototype_GameObject_Alien"), &TransformPos));


	TransformPos = _float3(12, 5, -17);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_Alien", TEXT("Prototype_GameObject_Alien"), &TransformPos));


	TransformPos = _float3(23, 16, -6);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_Alien", TEXT("Prototype_GameObject_Alien"), &TransformPos));


	TransformPos = _float3(12, 23, 14);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_Alien", TEXT("Prototype_GameObject_Alien"), &TransformPos));


	TransformPos = _float3(-2, 29, 8);

	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_Alien", TEXT("Prototype_GameObject_Alien"), &TransformPos));


	TransformPos = _float3(-10, 32, 2);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_Alien", TEXT("Prototype_GameObject_Alien"), &TransformPos));



	TransformPos = _float3(11, 89, -13);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_Alien", TEXT("Prototype_GameObject_Alien"), &TransformPos));



	TransformPos = _float3(-8, 98, -9);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_Alien", TEXT("Prototype_GameObject_Alien"), &TransformPos));



	TransformPos = _float3(-12, 104, 11);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_Alien", TEXT("Prototype_GameObject_Alien"), &TransformPos));



	TransformPos = _float3(107, 105, -14);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_Alien", TEXT("Prototype_GameObject_Alien"), &TransformPos));

	return S_OK;
}

HRESULT CScene_Stage3::Ready_Layer_ParsedDeathTreeObject(const _tchar * pLayerTag)
{
	_float3 TransformPos;




	TransformPos =	_float3(-14, 40, -11);

	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_TreeOfDeath", TEXT("Prototype_GameObject_TreeOfDeath"), &TransformPos));



	TransformPos = _float3(1, 42, 14);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_TreeOfDeath", TEXT("Prototype_GameObject_TreeOfDeath"), &TransformPos));


	TransformPos = _float3(4.5, 42, 14);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_TreeOfDeath", TEXT("Prototype_GameObject_TreeOfDeath"), &TransformPos));


	TransformPos = _float3(8, 42, 14);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_TreeOfDeath", TEXT("Prototype_GameObject_TreeOfDeath"), &TransformPos));



	TransformPos = _float3(13.5, 46, 0.5);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_TreeOfDeath", TEXT("Prototype_GameObject_TreeOfDeath"), &TransformPos));



	TransformPos = _float3(7, 51, 10);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_TreeOfDeath", TEXT("Prototype_GameObject_TreeOfDeath"), &TransformPos));




	TransformPos = _float3(-4.5, 60, 2);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_TreeOfDeath", TEXT("Prototype_GameObject_TreeOfDeath"), &TransformPos));



	TransformPos = _float3(5, 59, 14);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_TreeOfDeath", TEXT("Prototype_GameObject_TreeOfDeath"), &TransformPos));








	TransformPos = _float3(-13, 88, 8);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_TreeOfDeath", TEXT("Prototype_GameObject_TreeOfDeath"), &TransformPos));


	TransformPos = _float3(-15, 103, 9);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_TreeOfDeath", TEXT("Prototype_GameObject_TreeOfDeath"), &TransformPos));


	TransformPos = _float3(-4, 110, 1);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_TreeOfDeath", TEXT("Prototype_GameObject_TreeOfDeath"), &TransformPos));




	return S_OK;
}

HRESULT CScene_Stage3::Update_MapEffect(float timer)
{
	mParticleTime_Metao -= timer;
	if (mParticleTime_Metao < 0)
	{
		for (int i = 0; i < 5; i++)
		{
			PARTICLEDESC desc = Create_MeteoDesc(GetRandomBool());
			mParticleTime_Metao = desc.TotalParticleTime;
			GetSingle(CParticleMgr)->Create_ParticleObject(SCENE_STAGE3, desc);
		}

	}

	mParticleTime_Star -= timer;
	if (mParticleTime_Star < 0)
	{
		PARTICLEDESC  desc  = Create_Star();
		mParticleTime_Star = desc.TotalParticleTime;
		for (int i = 0; i < 1; i++)
		{
			GetSingle(CParticleMgr)->Create_ParticleObject(SCENE_STAGE3, desc);
		}
	}



	
	

	return S_OK;
}

void CScene_Stage3::Createparticle_Scene3()
{
	//PARTICLEDESC tDesc;
	////파티클이 흩날리는 종류 설정
	//tDesc.eParticleID = Particle_Spread;

	////파티클 텍스처 컴포넌트 이름을 설정 (기본적으로 자기 씬에 컴포넌트가 있는지 검사하고 스테틱에있는지도 검사함)
	//// Prototype_Component_Texture_Particle // 은혁 / 진우 파티클
	//// Prototype_Component_Texture_JY_Effect // 주엽 파티클
	//// Prototype_Component_Texture_JH_Effect // 지환 파티클

	//tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JH_Effect");
	////파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	//tDesc.szTextureLayerTag = TEXT("meteor_A");
	//// tDesc.szTextureLayerTag = TEXT("meteor_B");

	////총 파티클이 몇초동안 흩날릴 것인지 설정
	//tDesc.TotalParticleTime = 20.f;
	////파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	//tDesc.EachParticleLifeTime = 1.f;

	////파티클의 사이즈를 설정
	//tDesc.ParticleSize = _float3(0.3f, 0.3f, 0.3f);
	////파티클의 파워(이동속도)를 결정
	//tDesc.Particle_Power = 1;
	////파티클의 파워(이동속도)의 랜덤 범위를 결정
	//tDesc.PowerRandomRange = _float2(0.9f, 1.1f);

	////파티클이 한번에 최대 몇개까지 보일 것인지 설정
	//tDesc.MaxParticleCount = 30;

	////FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	////FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	////단 둘중 하나만 사용 가능
	////둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	////FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	//// tDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player))->Get_Component(TAG_COM(Com_Transform)));
	// tDesc.FixedTarget = _float3(0,3,0);

	////파티클의 최대 이탈 범위(range)를 설정해 줌
	////FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	//tDesc.MaxBoundary = _float3(10, 10, 10);

	////텍스처의 색상을 변경할 수 있는 기능 온오프
	////만약 true로 사용할 경우 텍스처의 원래 색상은 무시되고 타겟 색상으로 반짝반짝 거리게 설정됨
	////true로 사용할 경우 반드시 타겟 컬러를 설정해 줄 것
	//tDesc.ParticleColorChage = false;
	//// tDesc.TargetColor = _float3(237, 28, 36);
	//// tDesc.TargetColor2 = _float3(53, 255.f, 11);

	////만약 UI에 그려져야한다면 true 월드에 그려져야한다면 false 로 설정할 것
	////UI 로 그리게 될 경우 위의 모든 좌표는 API 좌표 기준으로 셋팅할 것
	////World로 그리게 될 경우 위의 모든 좌표는 월드 좌표 기준으로 셋팅할 것
	//tDesc.m_bIsUI = false;

	////텍스처 오토프레임을 사용할 것인지 말 것인지 결정
	//tDesc.m_bIsTextureAutoFrame = true;
	//tDesc.fAutoFrameMul = 3.f;

	//// 파티클 시작 위치 랜덤
	//tDesc.ParticleStartRandomPosMin = _float3(-5, 0.0f, -5.1f);
	//tDesc.ParticleStartRandomPosMax = _float3(5.f, 0.5f, 5.1f);

	////방향을 설정하고 싶을 때 사용하는 옵션
	////ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	////혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;
	//tDesc.vUp = _float3(0, 0, -1);

	//tDesc.MustDraw = true;
	////오브젝트 뒤에 가려지지 않게 만듬

	//tDesc.IsParticleFameEndtoDie = false;
	////프레임이 한번만 돌것인지 정함

	////Create_ParticleObject를 호출하여 스테이지 아이디와 지금까지 설정한 desc를 넣어주면 됨
	//GetSingle(CParticleMgr)->Create_ParticleObject(SCENE_STAGE3, tDesc);


}

PARTICLEDESC CScene_Stage3::Create_MeteoDesc(bool isRight)
{
	// 둥둥 떠다니는 운석 이펙트
	PARTICLEDESC tDesc;
	tDesc.eParticleID = Particle_Ball;



	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 10.0f;
	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 5.f;
	tDesc.Particle_Power = 0.8f;

	float randSize = GetRandomFloat(0.2, 1.5f);
	tDesc.ParticleSize = _float3(randSize, randSize, randSize);
	tDesc.PowerRandomRange = _float2(0.5f, 2.f);

	tDesc.MaxParticleCount = 2;

	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	// tDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player))->Get_Component(TAG_COM(Com_Transform)));
	
	tDesc.FollowingTarget = (CTransform*)mPlayer->Get_Component(TAG_COM(Com_Transform));

	//파티클의 최대 이탈 범위(range)를 설정해 줌
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	tDesc.MaxBoundary = _float3(20, 20, 5);

	// 파티클 시작 위치 랜덤
	tDesc.ParticleStartRandomPosMin = _float3(-10, -10, -2);
	tDesc.ParticleStartRandomPosMax = _float3(15,15, 2);

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;
	
	// _float3 randVec;
	// GetRandomVector(&randVec,&_float3(-1, -1, 0), &_float3(1, -1, 0));
	
	tDesc.vUp = _float3(0,0,0);

	tDesc.m_bIsTextureAutoFrame = true;
	tDesc.fAutoFrameMul = 2.f;


	tDesc.ParticleColorChage = false;
	tDesc.m_bIsUI = false;
	tDesc.MustDraw = false;
	tDesc.IsParticleFameEndtoDie = false;

	//tDesc.AlphaBlendON = true;
	tDesc.m_fAlphaTestValue = 150.f;

	//Create_ParticleObject를 호출하여 스테이지 아이디와 지금까지 설정한 desc를 넣어주면 됨
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JH_Effect");
	if(isRight)
		tDesc.szTextureLayerTag = TEXT("meteor_A");
	else 
		tDesc.szTextureLayerTag = TEXT("meteor_B");
	return tDesc;


}

PARTICLEDESC CScene_Stage3::Create_Star()
{
	// 둥둥 떠다니는 운석 이펙트
	PARTICLEDESC tDesc;
	tDesc.eParticleID = Particle_Fixed;

	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JH_Effect");
	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("star");

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 3.0f;
	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 0.5f;
	tDesc.Particle_Power = 1.0f;

	float randSize = GetRandomFloat(0.3f, 1.5f);

	tDesc.ParticleSize = _float3(randSize, randSize, randSize);
	tDesc.PowerRandomRange = _float2(0.8f, 1.2f);

	tDesc.MaxParticleCount = 10;

	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	// tDesc.FollowingTarget = (CTransform*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player))->Get_Component(TAG_COM(Com_Transform)));

	tDesc.FollowingTarget = (CTransform*)mPlayer->Get_Component(TAG_COM(Com_Transform));

	//파티클의 최대 이탈 범위(range)를 설정해 줌
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	tDesc.MaxBoundary = _float3(15, 15, 15);

	// 파티클 시작 위치 랜덤
	tDesc.ParticleStartRandomPosMin = _float3(-15, -15, -10);
	tDesc.ParticleStartRandomPosMax = _float3(15.f, 15, 10);

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;
	tDesc.vUp = _float3(0,1,0);

	tDesc.m_bIsTextureAutoFrame = false;
	tDesc.fAutoFrameMul = 2.f;


	tDesc.ParticleColorChage = false;
	tDesc.m_bIsUI = false;
	tDesc.MustDraw = false;
	tDesc.IsParticleFameEndtoDie = false;
	tDesc.AlphaBlendON = true;

	// tDesc.m_fAlphaTestValue = 180.f;

	return tDesc;
}

HRESULT CScene_Stage3::Ready_Layer_StageEndCollsionObject(const _tchar * pLayerTag)
{

	_float3 TransformPos = _float3(-1, 104, -6.5);
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, L"Layer_TreasureBox", TEXT("Prototype_GameObject_TreasureBox"), &TransformPos));



	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object"),
		&_float3(-1, 104, -7.f)))
		return E_FAIL;

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object"),
		&_float3(-2, 104, -7.f)))
		return E_FAIL;

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("ProtoType_GameObject_Collision_Object"),
		&_float3(0, 104, -7.f)))
		return E_FAIL;


	return S_OK;
}

HRESULT CScene_Stage3::Ready_Layer_PauseUI(const _tchar * pLayerTag)
{
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, pLayerTag, TEXT("Prototype_GameObject_PauseUI")))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Stage3::Ready_Layer_Player(const _tchar * pLayerTag)
{
	list<CGameObject*>* pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, pLayerTag);
	if (pPlayerList == nullptr)
	{
		if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, pLayerTag, TAG_OP(Prototype_Player)))
			return E_FAIL;

		GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, pLayerTag)->Set_NowSceneNum(SCENE_STAGE3);
		pPlayerList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, pLayerTag);
		mPlayer = pPlayerList->front();
		Safe_AddRef(mPlayer);
		//mPlayer->ReInitialize(&_float3(0, 104, -7.f));
		mPlayer->ReInitialize(&_float3(0.f, 1.f, 0));
		
	}
	else
	{
		
		mPlayer = pPlayerList->front();
		Safe_AddRef(mPlayer);
		//mPlayer->ReInitialize(&_float3(0, 104, -7.f));
		mPlayer->ReInitialize(&_float3(0.f, 1.f, 0));
	}

	return S_OK;
}

HRESULT CScene_Stage3::Ready_Layer_Terrain(_uint sceneid, list<SPECIALCUBE*>* listdata)
{
	CObject_PortalCube_A::POTALDESC potalDesc;
	CObject_EscalatorCube::ESCALATORDESC escalDesc;

	potalDesc.iNowScene = sceneid;
	potalDesc.vPos_A_Cube = _float3(0, 0, 0);
	potalDesc.vPos_B_Cube = _float3(0, 0, 0);

	escalDesc.vStartPos = _float3(0, 0, 0);
	escalDesc.vEndPos = _float3(0, 0, 0);

	int count_Potal = 0;
	int count_Escalator = 0;
	int count_Oribit = 0;

	for (auto data : *listdata)
	{
		// 포탈 큐브 생성방식
		if (data->Tagname == TAG_OP(Prototype_PortalCube_A))
		{
			if (count_Potal % 2 == 0)
			{
				memcpy(&potalDesc.vPos_A_Cube, &(data->WorldMat.m[3]), sizeof(_float3));
			}
			else
			{
				memcpy(&potalDesc.vPos_B_Cube, &(data->WorldMat.m[3]), sizeof(_float3));
				// 생성
				GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_STAGE3,
					/*TAG_LAY(Layer_Terrain)*/TEXT("Layer_Potal"), TAG_OP(Prototype_PortalCube_A),
					&potalDesc);
			}
			count_Potal++;
		}

		// 엘리베이터 생성방식
		if (data->Tagname == TAG_OP(Prototype_EscalatorCube))
		{
			if (count_Escalator % 2 == 0)
			{
				memcpy(escalDesc.vStartPos, &(data->WorldMat.m[3]), sizeof(_float3));
			}
			else
			{
				memcpy(escalDesc.vEndPos, &(data->WorldMat.m[3]), sizeof(_float3));
				// 생성
				GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_STAGE3,
					TAG_LAY(Layer_Terrain), TAG_OP(Prototype_EscalatorCube),
					&escalDesc);
			}
			count_Escalator++;
		}

		// 공전 큐브 생성 테스트
		if (data->Tagname == TAG_OP(Prototype_OrbitButtonCube))
		{
			CObject_OrbitButton::ORBITDESC orbitDesc;

			_float3 buttonPos = _float3(0, 0, 0);
			_float3 oriPos = _float3(0, 0, 0);
			memcpy(buttonPos, &(data->WorldMat.m[3]), sizeof(_float3));

			if (count_Oribit == 0)
			{
				orbitDesc.vButtonPos = buttonPos;
				orbitDesc.vOrbitRotAxis = buttonPos;
				oriPos = buttonPos;
				oriPos.x += 2;
				oriPos.y += 2;
				// 공전 큐브 위치
				orbitDesc.vOribitCubeStartPos = oriPos;

				// 공전큐브 xyz 개수
				orbitDesc.vOribitTotalXYZ = _float3(1, 2, 2);
				if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, TAG_LAY(Layer_Terrain), TAG_OP(Prototype_OrbitButtonCube), &orbitDesc))
					return E_FAIL;
			}

			if (count_Oribit == 1)
			{
				// 자전 큐브
				orbitDesc.vButtonPos = buttonPos;

				oriPos = buttonPos;
				oriPos.x += 3;
				oriPos.y += 3;
				// 공전 큐브 위치
				orbitDesc.vOribitCubeStartPos = oriPos;
				orbitDesc.vOrbitRotAxis = oriPos;

				// 공전큐브 xyz 개수
				orbitDesc.vOribitTotalXYZ = _float3(6, 3, 2);
				if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, TAG_LAY(Layer_Terrain), TAG_OP(Prototype_OrbitButtonCube), &orbitDesc))
					return E_FAIL;
			}
			count_Oribit++;
		}

		// 별생성
		if (data->Tagname == TEXT("ProtoType_GameObject_Object_Star"))
		{
			CObject_Star::STARDESC StarDesc;

			memcpy(StarDesc.fTransform, &(data->WorldMat.m[3]), sizeof(_float3));

			if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE3, TEXT("Layer_Object_Star"), TEXT("ProtoType_GameObject_Object_Star"), &StarDesc))
				return E_FAIL;
		}
	}
	return S_OK;
}

CScene_Stage3 * CScene_Stage3::Create(LPDIRECT3DDEVICE9 GraphicDevice)
{
	CScene_Stage3* pTempGame = new CScene_Stage3(GraphicDevice);

	if (FAILED(pTempGame->Initialize()))
	{
		Safe_Release(pTempGame);
		MSGBOX("Failed to Creating Scene_Stage2");
		return nullptr;
	}

	return pTempGame;
}

void CScene_Stage3::Free()
{
	GetSingle(CMapLoadMgr)->DestroyInstance();
	Safe_Release(mPlayer);
	__super::Free();
}