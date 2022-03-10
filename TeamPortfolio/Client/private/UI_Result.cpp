#include "stdafx.h"
#include "..\public\UI_Result.h"
#include "MyButton.h"
#include "UI_Image.h"
#include "Scene_Loading.h"
#include "UI_RankStar.h"
#include "Player.h"
#include "Inventory.h"
#include <time.h>

CUI_Result::CUI_Result(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_Result::CUI_Result(const CUI_Result & rhs)
	: CUI(rhs), m_UIPrototypes(rhs.m_UIPrototypes)
{
	for (auto& pair : m_UIPrototypes)
	{
		Safe_AddRef(pair.second);
	}
}

HRESULT CUI_Result::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	CGameInstance* pGameInstance = GetSingle(CGameInstance);

	//SCENEID::SCENE_STATIC
	//m_eNowSceneNum
	//프로토타입 넣는중
	m_UIPrototypes.emplace(TEXT("ProtoType_GameObject_UI_Button"), CMyButton::Create(m_pGraphicDevice));

	//if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Component_Texture_UI"), CMyButton::Create(m_pGraphicDevice))))
	//	return E_FAIL;
	
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObject_UI_RankStar"), CUI_RankStar::Create(m_pGraphicDevice))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Result::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_vUIDesc = _float4(g_iWinCX >> 1, g_iWinCY >> 1, g_iWinCX >> 1, 500);

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, NOT_EXIST_BLOCK);
	

	if (FAILED(Ready_Layer_Button(TEXT("Layer_Button"))))
		return E_FAIL;

	m_ComTexture->Change_TextureLayer(TEXT("Result"));

	//레벨디자인할 때 만져야할 설정들

	srand(time(NULL));
	RankStarGold = 900.f + (rand()%200);	//등급 하나당 획득 골드량

	//보간을 위한 변수 Result 창
	StartPosY = -900;
	TargetPosY = g_iWinCY >> 1;

	//보간을 위한 뉴 스타트 창
	//NTargetPosY
	return S_OK;
}

_int CUI_Result::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	m_fFrame = fDeltaTime;

	//if (m_fTimer > 3.f)  //클리어가 트루일 때 제대로 값이 들어가는지 확인용
	//{

	//	m_bClear = true;
	//}

	//if (GetSingle(CGameInstance)->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press)
	//{
	//	m_bClear = true;
	//	m_bAfterAnimIsClear = true;
	//}

	if (m_bStopSwitch == false)
	{
		m_fTimer += m_fFrame;
		TempMinutes = m_fTimer / 60;
		TempSeconds = (_uint)m_fTimer % 60;
	}
	else 
	{
		m_fPlayerAnimFrame += fDeltaTime;
		if (m_fPlayerAnimFrame > m_fForPlayerAnimationMaxTime) {
			if (m_bAfterAnimIsClear)
				m_bClear = true;
			else
				m_fTimer = m_fMaxTime + 1.f;

		}
	}

	//_float TempY; 보간을 담을 변수... 일단 멤버변수로 빼놔서 주석처리함

	if (m_fTimer > m_fMaxTime || m_bClear == true)
	{
		

		EasingTime += fDeltaTime;
		TempY = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, StartPosY, TargetPosY, EasingTime, 2.0f);
		if (EasingTime > 2)
		{
			TempY = TargetPosY;
			//EasingTime = 0;  한번만 실행시킬 예정이기 때문에 주석처리함.
			EasingSwitch = true;
		}
		m_vUIDesc.y = TempY;
		if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
			return E_FAIL;

		if (EasingSwitch == true)
		{
			if (m_bSetupGenerate == false)
			{

				//EasingTime += fDeltaTime;
				//TempW = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, StartPosW, TargetPosW, EasingTime, 2.0f);
				//if (EasingTime > 2)
				//{
				//	TempW = TargetPosW;
				//	EasingTime = 0;
				//}
				//m_vUIDesc.w = TempW;
				//if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
				//	return E_FAIL;

				if (FAILED(Ready_Layer_RankStar(TEXT("Layer_RankStar"))))
					return E_FAIL;

				SetUp_Player();
				m_bSetupGenerate = true;

				if (m_bClear == true)
				{
					GetSingle(CGameInstance)->PlaySound(L"EH_cheer.mp3", CHANNEL_UI);
				}
				else
				{
					GetSingle(CGameInstance)->PlaySound(L"EH_boo.mp3", CHANNEL_UI);
				}
			}


			m_fFrameTexture += fDeltaTime * 7.5;
		}

		Button_Picking();


		if (FAILED(Update_UIButtonList(fDeltaTime)))
			return E_FAIL;



		m_bStopSwitch = true;

	}



	return _int();
}

_int CUI_Result::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;
	//RENDER_PRIORITY ,RENDER_UI

	//이걸 함으로써 렌더되지 않게 연산량을 줄여주자!!!
	if (m_fTimer > m_fMaxTime || m_bClear == true)
	{
		if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
			return E_FAIL;
		if (FAILED(LateUpdate_UIButtonList(fDeltaTime)))
			return E_FAIL;
	}


	return _int();
}

_int CUI_Result::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	//if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame)))
	//	return E_FAIL;
	if (FAILED(m_ComTexture->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	//폰트 관련된건 여기에 있음
		SetUp_Pont();

	return _int();
}

_int CUI_Result::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CUI_Result::Ready_Layer_Button(const _tchar * pLayerTag)
{
	CMyButton* temp = (CMyButton*)(Find_UI(TEXT("ProtoType_GameObject_UI_Button"))->Clone(&_float4(9999.f, 9999.f, 250, 100)));
	temp->Set_ButtonName(L"Button_Result_Start");
	m_UIButtonList.emplace(L"Button_Result_Start", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("ProtoType_GameObject_UI_Button"))->Clone(&_float4(9999.f, 9999.f, 250, 100)));
	temp->Set_ButtonName(L"Button_Result_Cancel");
	m_UIButtonList.emplace(L"Button_Result_Cancel", (CUI*)temp);

	//m_vButtonDesc = { g_iWinCX >> 1,g_iWinCY >> 1, 250 ,100 };

	return S_OK;
}

HRESULT CUI_Result::Update_UIButtonList(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	int hr;
	for (auto pair : m_UIButtonList) {
		hr = (pair.second->Update(fTimeDelta));
		if (hr != SHOP_END)
			break;
			//스위치는 for문 밖에다 쓰도록 하자!
	}
	switch (hr)
	{
	case RESULT_START:
	{
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press)
		{
			POINT ptMouse;
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);
			m_isClicked = false;
			if (PtInRect(&m_fStartButton, ptMouse))
			{
				m_isClicked = true;
				CMyButton* ButtonStart = (CMyButton*)Find_Button(L"Button_Result_Start");
				CTexture* ButtonTexture = (CTexture*)ButtonStart->Get_Component(TEXT("Com_Texture"));
				ButtonTexture->Change_TextureLayer(L"Button_Result_Start3");
			}
		}
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Up)
		{
			CScene* TempScene = pGameInstance->Get_Scene();
			TempScene->Scene_InGame_Chage(true, pGameInstance->Get_NowSceneNum());
			GetSingle(CGameInstance)->PlaySound(L"EH_Button_Result_Start.wav", CHANNEL_UI);
		}
		break;
	}
	case RESULT_CANCEL:
	{
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press)
		{
			POINT ptMouse;
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);
			m_isClicked = false;
			if (PtInRect(&m_fCancelButton, ptMouse))
			{
				m_isClicked = true;
				CMyButton* ButtonCancel = (CMyButton*)Find_Button(L"Button_Result_Cancel");
				CTexture* ButtonTexture = (CTexture*)ButtonCancel->Get_Component(TEXT("Com_Texture"));
				ButtonTexture->Change_TextureLayer(L"Button_Result_Cancel3");
			}
		}
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Up)
		{
			CScene* TempScene = pGameInstance->Get_Scene();
			TempScene->Scene_InGame_Chage(true, SCENEID::SCENE_STAGESELECT);
			GetSingle(CGameInstance)->PlaySound(L"EH_Button_Result_Cancel.wav", CHANNEL_UI);
		}
		break;
	}
	default:
	{
		break;
	}
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CUI_Result::LateUpdate_UIButtonList(_float fTimeDelta)
{

	for (auto pair : m_UIButtonList)
	{
		if (FAILED(pair.second->LateUpdate(fTimeDelta)))
			return E_FAIL;






	}
	return S_OK;
}

CUI * CUI_Result::Find_UI(const _tchar * tagUI)
{
	auto iter = find_if(m_UIPrototypes.begin(), m_UIPrototypes.end(), CTagFinder(tagUI));

	if (iter == m_UIPrototypes.end())
		return nullptr;

	return iter->second;
}

CUI * CUI_Result::Find_Button(const _tchar * tagUIList)
{
	auto iter = find_if(m_UIButtonList.begin(), m_UIButtonList.end(), CTagFinder(tagUIList));

	if (iter == m_UIButtonList.end())
		return nullptr;

	return iter->second;
}

void CUI_Result::Set_Particle_Firecracker2()
{
	PARTICLEDESC tDesc;
	tDesc.eParticleID = Particle_Fountain;
	tDesc.TotalParticleTime = 1.f;
	tDesc.EachParticleLifeTime = 2.f;

	tDesc.ParticleSize = _float3(40.f, 40.f, 40.f);

	tDesc.Particle_Power = -600;

	tDesc.PowerRandomRange = _float2(0.5f, 1.5f);

	tDesc.MaxParticleCount = 100;

	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");

	tDesc.szTextureLayerTag = TEXT("Particle_Firecracker2");

	tDesc.m_bIsTextureAutoFrame = false;

	tDesc.FixedTarget = _float3(120, 610, 0);

	tDesc.MaxBoundary = _float3(1280, 720, 1);

	tDesc.ParticleColorChage = true;
	tDesc.TargetColor = _float3(46.f, 231.f, 60.f);
	tDesc.TargetColor2 = _float3(255.f, 0.f, 00.f);

	tDesc.m_bIsUI = true;

	tDesc.ParticleStartRandomPosMin = _float3(-0.5f, -600.f, -0.5f);
	tDesc.ParticleStartRandomPosMax = _float3(0.5f, 0.f, 0.5f);

	tDesc.MustDraw = true;
	tDesc.IsParticleFameEndtoDie = false;

	tDesc.m_bUIDepth = -9999;

	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	tDesc.FixedTarget = _float3(220, 720, 1);
	tDesc.TargetColor = _float3(33.f, 38.f, 243.f);
	tDesc.TargetColor2 = _float3(34.f, 248.f, 29.f);
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	tDesc.FixedTarget = _float3(320, 720, 1);
	tDesc.TargetColor = _float3(187.f, 90.f, 201.f);
	tDesc.TargetColor2 = _float3(91.f, 186.f, 200.f);
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	tDesc.FixedTarget = _float3(960, 720, 1);
	tDesc.TargetColor = _float3(192.f, 99.f, 113.f);
	tDesc.TargetColor2 = _float3(170.f, 98.f, 193.f);
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	tDesc.FixedTarget = _float3(1060, 720, 1);
	tDesc.TargetColor = _float3(37.f, 254.f, 227.f);
	tDesc.TargetColor2 = _float3(255.f, 239.f, 36.f);
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	tDesc.FixedTarget = _float3(1160, 720, 1);
	tDesc.TargetColor = _float3(0.f, 0.f, 255.f);
	tDesc.TargetColor2 = _float3(255.f, 0.f, 0.f);
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	//Sub
	//tDesc.bSubPraticle = true;

	//PARTICLEDESC tSubParticleDesc = {};

	//tSubParticleDesc.TotalParticleTime = 3000.f;
	//tSubParticleDesc.EachParticleLifeTime = 0.35f;
	//tSubParticleDesc.eParticleID = Particle_Fountain;

	//tSubParticleDesc.ParticleSize = _float3(20.f, 20.f, 20.f);

	//tSubParticleDesc.Particle_Power = -600;
	//tSubParticleDesc.PowerRandomRange = _float2(0.5f, 1.5f);

	//tSubParticleDesc.MaxParticleCount = 10;

	//tSubParticleDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	//tSubParticleDesc.szTextureLayerTag = TEXT("Particle_Firecracker2");

	//tSubParticleDesc.m_bIsTextureAutoFrame = false;
	//tSubParticleDesc.fAutoFrameMul = 10.f;
	//tSubParticleDesc.FollowingTarget = nullptr;
	//tSubParticleDesc.FixedTarget = _float3();

	//tSubParticleDesc.MaxBoundary = _float3(100, 20, 100);
	//tSubParticleDesc.ParticleColorChage = false;

	//tSubParticleDesc.m_bIsUI = false;
	//tSubParticleDesc.ParticleStartRandomPosMin = _float3(0.f, 0.0f, 0.0f);
	//tSubParticleDesc.ParticleStartRandomPosMax = _float3(0.f, 0.0f, 0.0f);

	//tSubParticleDesc.vUp = _float3(0, 1, 0);

	//tSubParticleDesc.m_bIsUI = true;

	//tSubParticleDesc.MustDraw = false;
	//tSubParticleDesc.IsParticleFameEndtoDie = false;
	//tSubParticleDesc.m_bUIDepth = -9999;
	//tSubParticleDesc.AlphaBlendON = true;

	//GetSingle(CParticleMgr)->Create_ParticleObject_AddSub(m_eNowSceneNum, tDesc, tSubParticleDesc);

}

void CUI_Result::Set_Particle_Firecracker()
{
	PARTICLEDESC tDesc;
	//파티클이 흩날리는 종류 설정
	tDesc.eParticleID = Particle_Fixed;

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 0.f;

	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 6.f;

	//파티클의 사이즈를 설정
	tDesc.ParticleSize = _float3(20.f, 20.f, 20.f);

	//파티클의 파워(이동속도)를 결정
	tDesc.Particle_Power = -600;

	//파티클의 파워(이동속도)의 랜덤 범위를 결정
	tDesc.PowerRandomRange = _float2(0.5f, 1.5f);

	//파티클이 한번에 최대 몇개까지 보일 것인지 설정
	tDesc.MaxParticleCount = 20;

	//파티클 텍스처 컴포넌트 이름을 설정 (기본적으로 자기 씬에 컴포넌트가 있는지 검사하고 스테틱에있는지도 검사함)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");

	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("Particle_Firecracker");

	//텍스처 오토프레임을 사용할 것인지 말 것인지 결정
	tDesc.m_bIsTextureAutoFrame = true;

	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	//tDesc.FollowingTarget = m_ComTransform;
	tDesc.FixedTarget = _float3(g_iWinCX >> 1, g_iWinCY >> 1, 0);

	//파티클의 최대 이탈 범위(range)를 설정해 줌 
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	tDesc.MaxBoundary = _float3(640, 360, 0);

	//텍스처의 색상을 변경할 수 있는 기능 온오프
	//만약 true로 사용할 경우 텍스처의 원래 색상은 무시되고 타겟 색상으로 반짝반짝 거리게 설정됨
	//true로 사용할 경우 반드시 타겟 컬러를 설정해 줄 것
	tDesc.ParticleColorChage = true;
	tDesc.TargetColor = _float3(46.f, 231.f, 60.f);
	tDesc.TargetColor2 = _float3(255.f, 0.f, 00.f);

	//만약 UI에 그려져야한다면 true 월드에 그려져야한다면 false 로 설정할 것
	//UI 로 그리게 될 경우 위의 모든 좌표는 API 좌표 기준으로 셋팅할 것
	//World로 그리게 될 경우 위의 모든 좌표는 월드 좌표 기준으로 셋팅할 것
	tDesc.m_bIsUI = true;

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;
	/*tDesc.vUp = _float3(-1, -1, 0);*/

	//파티클의 랜덤 설정을 Min과 Max를 정해준다.
	tDesc.ParticleStartRandomPosMin = _float3(-640.f, -360.f, -0.5f);
	tDesc.ParticleStartRandomPosMax = _float3(640.f, 360.f, 0.5f);

	tDesc.MustDraw = true;
	//오브젝트 뒤에 가려지지 않게 만듬

	tDesc.IsParticleFameEndtoDie = false;
	//프레임이 한번만 돌것인지 정함

	tDesc.m_bUIDepth = -9999;

	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	tDesc.TargetColor = _float3(36.f, 69.f, 255.f);
	tDesc.TargetColor2 = _float3(206.f, 255.f, 36.f);
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

	tDesc.TargetColor = _float3(234.f, 57.f, 238.f);
	tDesc.TargetColor2 = _float3(67.f, 237.f, 24.f);
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

}

void CUI_Result::Set_Particle_Cry()
{
	PARTICLEDESC tDesc;
	//파티클이 흩날리는 종류 설정
	tDesc.eParticleID = Particle_Straight;

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 10.f;

	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 6.f;

	//파티클의 사이즈를 설정
	tDesc.ParticleSize = _float3(30.f, 30.f, 30.f);

	//파티클의 파워(이동속도)를 결정
	tDesc.Particle_Power = 400;

	//파티클의 파워(이동속도)의 랜덤 범위를 결정
	tDesc.PowerRandomRange = _float2(0.3f, 1.8f);

	//파티클이 한번에 최대 몇개까지 보일 것인지 설정
	tDesc.MaxParticleCount = 30;

	//파티클 텍스처 컴포넌트 이름을 설정 (기본적으로 자기 씬에 컴포넌트가 있는지 검사하고 스테틱에있는지도 검사함)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");

	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("Particle_Sweat");

	//텍스처 오토프레임을 사용할 것인지 말 것인지 결정
	tDesc.m_bIsTextureAutoFrame = false;

	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	//tDesc.FollowingTarget = m_ComTransform;
	tDesc.FixedTarget = _float3(g_iWinCX >> 1, 0, 0);

	//파티클의 최대 이탈 범위(range)를 설정해 줌 
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	tDesc.MaxBoundary = _float3(640, 720, 0);

	//텍스처의 색상을 변경할 수 있는 기능 온오프
	//만약 true로 사용할 경우 텍스처의 원래 색상은 무시되고 타겟 색상으로 반짝반짝 거리게 설정됨
	//true로 사용할 경우 반드시 타겟 컬러를 설정해 줄 것
	tDesc.ParticleColorChage = false;
	tDesc.TargetColor = _float3(46.f, 231.f, 60.f);
	tDesc.TargetColor2 = _float3(255.f, 0.f, 00.f);

	//만약 UI에 그려져야한다면 true 월드에 그려져야한다면 false 로 설정할 것
	//UI 로 그리게 될 경우 위의 모든 좌표는 API 좌표 기준으로 셋팅할 것
	//World로 그리게 될 경우 위의 모든 좌표는 월드 좌표 기준으로 셋팅할 것
	tDesc.m_bIsUI = true;

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;
	/*tDesc.vUp = _float3(-1, -1, 0);*/

	//파티클의 랜덤 설정을 Min과 Max를 정해준다.
	tDesc.ParticleStartRandomPosMin = _float3(-640.f, 0.f, 0.f);
	tDesc.ParticleStartRandomPosMax = _float3(640.f, 0.f, 0.f);

	tDesc.MustDraw = true;
	//오브젝트 뒤에 가려지지 않게 만듬

	tDesc.IsParticleFameEndtoDie = false;
	//프레임이 한번만 돌것인지 정함

	tDesc.m_bUIDepth = -9999;

	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);
}

HRESULT CUI_Result::Ready_Layer_RankStar(const _tchar * pLayerTag)
{
	CUI_RankStar::RANKSTARDESC	tagRankStarDesc;
	//윈도우 좌표계로 해야함
	//if (TempMinutes <= 1)
	//{
	//	RankNumber = 5;
	//}
	//else if (TempMinutes <= 2)
	//{
	//	RankNumber = 4;
	//}
	//else if (TempMinutes <= 3)
	//{
	//	RankNumber = 3;
	//}
	//else if (TempMinutes <= 4)
	//{
	//	RankNumber = 2;
	//}
	//else
	//{
	//	RankNumber = 1;
	//}
	for (_uint i = 0; i < RankNumber; ++i)
	{
		tagRankStarDesc.WindowRectPos = _float3(450.f+(i*90), 168.f, 0); //렉트의 위치
		tagRankStarDesc.Time = 1.f + i;									//별이 나타나는 시간
		if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(m_eNowSceneNum, pLayerTag, TEXT("ProtoType_GameObject_UI_RankStar"), &tagRankStarDesc))
			return E_FAIL;
	}


	//여기서부터 시작해야됨
	//씬 스테이지2 참고하면 될 듯?
	//획득골드+랭크스타도 만들기 플레이어에 컴 인벤토리로 되어 있고
	//	거기에 셋 골드해서 넣어주면 됨
	//	Get_GameObject_By_LayerIndex이거 사용해서 플레이어 불러오자
	//	1인자에는 스태틱쓰고 2인자에는 레이어 플레이어쓰자 어차피 플레이어는
	//	하나밖에 없으니 디폴트0으로 되어 있는거 하면 될 듯
	//	획득골드량은 시간에 따라서 꼴리는대로 넣어주면 됨

	//	그리고 랭크스타는 미리 갯수 만들어놔서
	//	이동해서 불러오기 또는!!!
	//	클리어시 레디 레이어 랭크스타 함수 호출해서 이 때 만들어줘도 됨
	//	이게 괜찮을듯

	return S_OK;
}

void CUI_Result::Set_RankStar()
{
	RankNumber += 1;
}

HRESULT CUI_Result::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	//m_eNowSceneNum = SCENEID::SCENE_LOADING;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_UI_Result_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Result::SetUp_RenderState()
{


	//m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_XRGB(255, 255, 255));
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	return S_OK;
}

HRESULT CUI_Result::Release_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

HRESULT CUI_Result::SetUp_Pont()
{
	//잊지마라 폰트 매니저는 렌더에서만 사용가능하다.
	if (m_fTimer > m_fMaxTime || m_bClear == true)
	{

		//텍스쳐 소리를 관리함
		if (m_fFrameTexture > m_iOldFrameTexture + 1 && SoundSwitch == false)
		{
			GetSingle(CGameInstance)->PlaySound(L"EH_Text.wav", CHANNEL_UI);
			m_iOldFrameTexture = m_fFrameTexture;
		}
		if (m_fTimer > m_fMaxTime)
		{
			/*보관용
			wstring TempString;

			_tchar tempGoldArr[64];
			_itow_s(AcquisitionGold, tempGoldArr, 10);


			TempString = wstring(tempGoldArr) + L" Gold";
			GetSingle(CGameInstance)->Render_UI_Font(TempString, { 475.f,285.f }, { 20.f,60.f }, _float3(123, 104, 238), m_fFrameTexture );

			*/
			wstring TempString;

			///////획득 골드량을 표시하는중
			_tchar tempGoldArr[64];
			_itow_s(AcquisitionGold, tempGoldArr, 10);

			
			TempString = wstring(tempGoldArr) + L" Gold";
			GetSingle(CGameInstance)->Render_UI_Font(TempString, { 475.f,285.f }, { 25.f,60.f }, _float3(123, 104, 238), m_fFrameTexture);

			//TempString = L"Gold";
			//GetSingle(CGameInstance)->Render_UI_Font(TempString, { 600.f,285.f }, { 30.f,60.f }, _float3(123, 104, 238), m_fFrameTexture *2.5);

			////베스트 기록을 표시하는중
			_uint TempBestMinutes = (_uint)TempBestClear / 60;
			_uint TempBestSeconds = (_uint)TempBestClear % 60;
			_tchar tempBestMinArr[64];
			_itow_s(TempBestMinutes, tempBestMinArr, 10);

			_tchar tempBestSecArr[64];
			_itow_s(TempBestSeconds, tempBestSecArr, 10);

			TempString = wstring(tempBestMinArr) + L" : " + wstring(tempBestSecArr);// + L" Seconds";

			if (m_fFrameTexture > 10)
			{
				GetSingle(CGameInstance)->Render_UI_Font(TempString, { 475.f,360.f }, { 25.f,60.f }, _float3(123, 104, 238), m_fFrameTexture -10);
			}

			if (FAILED(Release_RenderState()))
				return E_FAIL;


			//////타이머를 표시하는중
			TempString = L"Mission Failed";

			if (m_fFrameTexture > 18)
			{
				GetSingle(CGameInstance)->Render_UI_Font(TempString, { 475.f,435.f }, { 25.f,60.f }, _float3(123, 104, 238), m_fFrameTexture -18);

				if (SoundSwitch == false)
				{

					GetSingle(CGameInstance)->PlaySound(L"EH_Mission_Failed.wav", CHANNEL_UI);
					Set_Particle_Cry();
					SoundSwitch = true;
				}

			}
			if (FAILED(Release_RenderState()))
				return E_FAIL;


		}
		else
		{
			wstring TempString;

			///////획득 골드량을 표시하는중
			_tchar tempGoldArr[64];
			_itow_s(AcquisitionGold, tempGoldArr, 10);

			TempString = wstring(tempGoldArr) + L" Gold";
			GetSingle(CGameInstance)->Render_UI_Font(TempString, { 500.f,285.f }, { 25.f,60.f }, _float3(123, 104, 238), m_fFrameTexture);

			////베스트 기록을 표시하는중
			_uint TempBestMinutes = (_uint)TempBestClear / 60;
			_uint TempBestSeconds = (_uint)TempBestClear % 60;
			_tchar tempBestMinArr[64];
			_itow_s(TempBestMinutes, tempBestMinArr, 10);

			_tchar tempBestSecArr[64];
			_itow_s(TempBestSeconds, tempBestSecArr, 10);

			TempString = wstring(tempBestMinArr) + L" : " + wstring(tempBestSecArr);

			if (m_fFrameTexture > 10)
			{
				GetSingle(CGameInstance)->Render_UI_Font(TempString, { 500.f,360.f }, { 25.f,60.f }, _float3(123, 104, 238), m_fFrameTexture - 10);
				if (m_bParticle_Switch == false)
				{

					Set_Particle_Firecracker2();

					m_bParticle_Switch = true;
				}

			}

			if (FAILED(Release_RenderState()))
				return E_FAIL;

			//////타이머를 표시하는중
			_tchar tempMinArr[64];
			_itow_s(TempMinutes, tempMinArr, 10);

			_tchar tempSecArr[64];
			_itow_s(TempSeconds, tempSecArr, 10);

			TempString = wstring(tempMinArr) + L" : " + wstring(tempSecArr);

			if (m_fFrameTexture > 18)
			{
				GetSingle(CGameInstance)->Render_UI_Font(TempString, { 500.f,435.f }, { 25.f,60.f }, _float3(123, 104, 238), m_fFrameTexture -18);

				if (SoundSwitch == false)
				{

					GetSingle(CGameInstance)->PlaySound(L"EH_Mission_Clear.wav", CHANNEL_UI);
					Set_Particle_Firecracker();
					SoundSwitch = true;
				}
			}
			if (FAILED(Release_RenderState()))
				return E_FAIL;

			//_tchar tempArr[64];
			//_itow_s(m_fTimer, tempArr, 10);

			//wstring TempString = wstring(tempArr) + L"Sec " + L"Eunhyek";

			//GetSingle(CGameInstance)->Render_UI_Font(TempString, { 500.f,400.f }, { 20.f,30.f }, _float3(83, 250, 120));

			//if (FAILED(Release_RenderState()))
			//	return E_FAIL;
		}

	}

	return S_OK;
}
HRESULT CUI_Result::SetUp_Player()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CPlayer* Player = (CPlayer*)pGameInstance->Get_GameObject_By_LayerIndex(SCENEID::SCENE_STATIC, TAG_LAY(Layer_Player)); // 플레이어 정보

	if(m_fMaxTime>m_fTimer)
	{
		/////////퀘스트 정보 전달 -진우/////////
		if (!(Player->getBestClear(m_eNowSceneNum)))
		{
			if(m_eNowSceneNum==SCENE_BOSS)
				GetSingle(CQuest)->Set_QuestIndexIncrease(QUEST_4, 1);
			else
				GetSingle(CQuest)->Set_QuestIndexIncrease(QUEST_3, 1);

		}
		////////////////////////////////////////
		Player->SetBestClear(m_eNowSceneNum, m_fTimer);
		
	}
	TempBestClear = Player->getBestClear(m_eNowSceneNum);

	CInventory* Inventory = (CInventory*)pGameInstance->Get_Commponent_By_LayerIndex(SCENEID::SCENE_STATIC, TAG_LAY(Layer_Player), TAG_COM(Com_Inventory)); //플레이어의 인벤토리 정보
	AcquisitionGold = RankNumber * RankStarGold;

	Inventory->Set_Gold(AcquisitionGold);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CUI_Result::Button_Picking()
{
	CMyButton* ButtonStart = (CMyButton*)Find_Button(L"Button_Result_Start");
	CTransform* ButtonTransform = (CTransform*)ButtonStart->Get_Component(TEXT("Com_Transform"));
	//ButtonTransform->Set_MatrixState(CTransform::STATE_POS, _float3(100.f, 200.f, 0));
	m_vButtonDesc = { (g_iWinCX >> 1) - 150,(g_iWinCY >> 1) + 150, 250 ,100 };
	_float4 vUIDesc = m_vButtonDesc;


	m_vButtonDesc.y = TempY + 150; //보간 처리중

	RECT TempButton;
	TempButton.top = LONG(vUIDesc.y - vUIDesc.w *0.5f);
	TempButton.bottom = LONG(vUIDesc.y + vUIDesc.w *0.5f);
	TempButton.right = LONG(vUIDesc.x + vUIDesc.z*0.5f);
	TempButton.left = LONG(vUIDesc.x - vUIDesc.z*0.5f);
	m_fStartButton = TempButton;
	ButtonStart->Set_Rect(m_vButtonDesc, TempButton);

	//
	//
	ButtonStart->Set_UI_Transform(ButtonTransform, m_vButtonDesc);

	CMyButton* ButtonCancel = (CMyButton*)Find_Button(L"Button_Result_Cancel");
	ButtonTransform = (CTransform*)ButtonCancel->Get_Component(TEXT("Com_Transform"));
	//ButtonTransform->Set_MatrixState(CTransform::STATE_POS, _float3(200.f, -100.f, 0));
	m_vButtonDesc = { (g_iWinCX >> 1) + 150,(g_iWinCY >> 1) + 150, 250 ,100 };
	vUIDesc = m_vButtonDesc;

	m_vButtonDesc.y = TempY + 150; //보간 처리중

	TempButton.top = LONG(vUIDesc.y - vUIDesc.w *0.5f);
	TempButton.bottom = LONG(vUIDesc.y + vUIDesc.w *0.5f);
	TempButton.right = LONG(vUIDesc.x + vUIDesc.z*0.5f);
	TempButton.left = LONG(vUIDesc.x - vUIDesc.z*0.5f);
	m_fCancelButton = TempButton;
	ButtonCancel->Set_Rect(m_vButtonDesc, TempButton);
	ButtonCancel->Set_UI_Transform(ButtonTransform, m_vButtonDesc);
}

void CUI_Result::Set_Clear(_bool _bClear)
{
	m_bClear = _bClear;
}

void CUI_Result::Set_Clear_Wait_AnimTime(_bool bIsClear, _float MaxAnimTime)
{
	m_bStopSwitch = true;
	m_fForPlayerAnimationMaxTime = MaxAnimTime;
	m_bAfterAnimIsClear = bIsClear;
}

CUI_Result * CUI_Result::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_Result* pInstance = new CUI_Result(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CUI_Result_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_Result::Clone(void * pArg)
{
	CUI_Result* pInstance = new CUI_Result((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CUI_Result_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_Result::Free()
{
	__super::Free();

	for (auto& pair : m_UIButtonList)
	{
		Safe_Release(pair.second);

	}
	m_UIButtonList.clear();

	for (auto& pair : m_UIPrototypes)
	{
		Safe_Release(pair.second);
	}
	m_UIPrototypes.clear();

	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
