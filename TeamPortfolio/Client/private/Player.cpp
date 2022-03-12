#include "stdafx.h"
#include "..\public\Player.h"
#include "Camera_Main.h"
#include "UI_Result.h"
#include "MonsterParent.h"
#include "UI_BossStatusUI.h"





CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	FAILED_CHECK(SetUp_LoginDesc());

	FAILED_CHECK(SetUp_ParticleDesc());

	if (pArg == nullptr)
	{
		m_fNowJumpPower = 0.000001f;
		m_fJumpPower = 10.f;
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(0, 1.f, 0));
		m_ComTransform->Scaled(_float3(1.5f, 1.5f, 1.5f));


		m_iPlayerLife = 3;
		m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("wakeup"), TEXT("Idle"));
	}
	else {
		FAILED_CHECK(ReInitialize(pArg));
	}


	ZeroMemory(m_tCoolDown, sizeof(COOL) * SKILL_END);
	ZeroMemory(StageBestClear, sizeof(_float) *SCENE_END);

	m_pCamera_Main = ((CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main))));
	
	if (m_pCamera_Main == nullptr)
		return E_FAIL;
	Safe_AddRef(m_pCamera_Main);


	//for (int i = 0; i < SCENE_END; ++i)
	//{
	//	StageBestClear[i] = 100000;
	//}

	m_bIsStageEnd = 0;
	return S_OK;
}


_int CPlayer::Update(_float fDeltaTime)
{

	if (m_eNowSceneNum == SCENE_LOADING)
		return S_FALSE;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FLEXIBLE, this);

	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	if (m_bReHurtTime < 5.f)
		m_bReHurtTime += fDeltaTime;


	if (m_bIsDead) {
		m_fDeadNPauseTime += fDeltaTime;

		// #Tag 플레이어 피격
		if (m_fDeadNPauseTime > 3.f) {
			m_bIsDead = false;
			m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(0, 1.f, 0));
			m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("hurt"), TEXT("Idle"), 8.f);
			m_pCamera_Main->CameraEffect(CCamera_Main::CAM_EFT_HIT, fDeltaTime);
		}

	}
	else if (m_bPause)
	{
		m_fDeadNPauseTime += fDeltaTime;

		if (m_pCarryObject != nullptr && m_fDeadNPauseTime < 2.f)
		{
			_float CarryObjectScale = GetSingle(CGameInstance)->Easing(TYPE_Linear, 1, 0, m_fDeadNPauseTime,2.f);
			CTransform* pCarryObjectTransform = ((CTransform*)(m_pCarryObject->Get_Component(TAG_COM(Com_Transform))));
			pCarryObjectTransform->Scaled({ CarryObjectScale ,CarryObjectScale ,CarryObjectScale });
			pCarryObjectTransform->Set_MatrixState(CTransform::STATE_POS, m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 0.35f, 0));
		}

		if (m_fDeadNPauseTime > m_fTotalPauseTime + 1)
		{
			if (m_pCarryObject != nullptr) {
				((CTransform*)(m_pCarryObject->Get_Component(TAG_COM(Com_Transform))))->Scaled({ 1.f ,1.f,1.f});
			}
			if (m_fNowJumpPower < 0)
				m_fNowJumpPower = -fDeltaTime;
	
			//포탈 이동 후 
			//GetSingle(CGameInstance)->PlaySound(TEXT("JY_Player_jump.wav"), CHANNEL_PLAYER);
			GetSingle(CGameInstance)->PlaySound(TEXT("JY_bouncehigh.wav"), CHANNEL_OBJECT);

				m_ComTexture->Change_TextureLayer_Wait(TEXT("jump_up"));
			m_bPause = false;
		}
	}
	else if(m_pCamera_Main->Get_EffectID() != CCamera_Main::CAM_EFT_ACTION)
	{
		if (!m_bIsStageEnd) {

			if(lstrcmp(m_ComTexture->Get_NowTextureTag(),L"hurt"))
			if (FAILED(Input_Keyboard(fDeltaTime)))
				return E_FAIL;


			if (FAILED(Animation_Change(fDeltaTime)))
				return E_FAIL;



			if (lstrcmp(m_ComTexture->Get_NowTextureTag(), L"exit"))
			{

			if (FAILED(Find_FootHold_Object(fDeltaTime)))
				return E_FAIL;

			if (FAILED(Set_PosOnFootHoldObject(fDeltaTime)))
				return E_FAIL;
			}
		}
		_Matrix matVeiwSpace;
		m_pGraphicDevice->GetTransform(D3DTS_VIEW, &matVeiwSpace);

		_float3 vPlayerPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);


		//카메라 바라보도록 설정
		_float3 vCamLook;
		memcpy(&vCamLook, &(matVeiwSpace.InverseMatrix().m[2][0]), sizeof(_float3));

		if (m_bTextureReverse)
			m_ComTransform->LookAt(vPlayerPos - vCamLook);
		else
			m_ComTransform->LookAt(vPlayerPos + vCamLook);

	}

	return _int();
}

_int CPlayer::LateUpdate(_float fDeltaTime)
{
	if (m_eNowSceneNum == SCENE_LOADING)
		return S_FALSE;

	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;



	if (!m_bIsStageEnd && m_pCamera_Main->Get_EffectID() != CCamera_Main::CAM_EFT_ACTION)
	{

		if (FAILED(Set_CamPosXYZ(fDeltaTime)))
			return E_FAIL;
	}

	m_OldAnimFrameTimeCheckerForSound = (_int)(m_NowAnimFrameTimeCheckerForSound);
	//렌더링 그룹에 넣어주는 역활
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_AFTEROBJ, this)))
		return E_FAIL;

	// 충돌 그룹
	NULL_CHECK_BREAK(m_pCollisionViewCom);
	m_Sphere.mRadius = 10;
	m_Sphere.mCenterPosition = m_pCollisionViewCom->WorldToView(m_ComTransform->Get_MatrixState(CTransform::STATE_POS));
	m_pCollisionViewCom->AddCollisionView(CCom_CollisionViewPort::COLL_PLAYER, this);

	

	return _int();
}

_int CPlayer::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL; 
	
	 
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;
	
	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame, &m_NowAnimFrameTimeCheckerForSound)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CPlayer::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CPlayer::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

 	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TAG_LAY(Layer_Terrain)))
	{
		//if (!(abs(fCollision_Distance.x) > abs(fCollision_Distance.y) && abs(fCollision_Distance.z) > abs(fCollision_Distance.y)))
		m_pCollisionCom->Collision_Pushed(m_ComTransform, fCollision_Distance, fDeltaTime);
	}
	else if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_FixCube")))
	{
		if (m_pCarryObject == nullptr && GetSingle(CGameInstance)->Get_DIKeyState(DIK_LSHIFT) & DIS_Down)
		{
			m_pCarryObject = pDestObjects;
			m_pCarryObjectTransform = (CTransform*)(m_pCarryObject->Get_Component(TAG_COM(Com_Transform)));

			m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("carryup"),TEXT("carryIdle"),12.f);


			m_bIsCliming = false;
			m_bIsRunning = false;
			m_ComTransform->Set_MoveSpeed(1.f);
			m_fJumpPower *= 0.5f;

			Safe_AddRef(m_pCarryObject);
			Safe_AddRef(m_pCarryObjectTransform);


			GetSingle(CGameInstance)->PlaySound(L"JY_Bulb_Flash_0.mp3", CHANNEL_OBJECT);
			//GetSingle(CGameInstance)->PlaySound(L"EH_CarryUp.wav", CHANNEL_OBJECT);
			GetSingle(CGameInstance)->PlaySound(TEXT("JY_Isaac_Hurt_Grunt0.mp3"), CHANNEL_PLAYER, 1.f);

			//GetSingle(CGameInstance)->PlaySound(TEXT("JY_dropheavypickup.wav"), CHANNEL_PLAYER, 1.f);
			
		}
		else if(m_pCarryObject != pDestObjects)
			m_pCollisionCom->Collision_Pushed(m_ComTransform, fCollision_Distance, fDeltaTime);

	}
	else if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_GravityCube")))
	{
		if (!m_bIsDead)
		{
			m_bIsDead = true;
			m_fDeadNPauseTime = 0;
			m_ComTexture->Change_TextureLayer_Wait(TEXT("suckIn"));
			GetSingle(CGameInstance)->PlaySound(TEXT("JY_airpanic.wav"), CHANNEL_PLAYER,2.f);
			GetSingle(CGameInstance)->PlaySound(L"EH_GravityCube.wav", CHANNEL_OBJECT);
		}
		if (m_fDeadNPauseTime < 4.0f)
			m_pCollisionCom->Collision_Suck_In(m_ComTransform, fCollision_Distance, fDeltaTime);
		
	}
	else if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_OrbitButton")))
	{
		if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_LSHIFT) & DIS_Down)
		{
			m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("buttonclick"), TEXT("Idle"), 4.f);
			GetSingle(CGameInstance)->PlaySound(L"EH_Orbit2.wav", CHANNEL_OBJECT);
			//Particle_NewDust();
			Particle_GameOver();
			//
		}
	}
	//
	else if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_Collision_StageEnd")))
	{
		if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_RETURN) & DIS_Press) {
			Set_StageEnd(1);
		}
	}
	else if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_Collision_StageEntry")))
	{
		if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_RETURN) & DIS_Down) {

			Set_StageEnd(2);
		}
	}



	return _int();
}

HRESULT CPlayer::ViewPortHit(CGameObject * hitobj)
{
	if (!lstrcmp(hitobj->Get_Layer_Tag(), TAG_LAY(Layer_Bullet)))
	{
		if (m_BossStatusUI->Get_bCanHit())
		{
			// #TODO 플레이어 피격
			if (!m_bIsDead)
			{
				GetSingle(CGameInstance)->PlaySound(TEXT("JY_Isaac_Hurt_Grunt0.mp3"), CHANNEL_PLAYER);
				GetSingle(CGameInstance)->PlaySound(TEXT("JY_Scared_Whimper_2.mp3"), CHANNEL_PLAYER);

				m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("hurt"), TEXT("Idle"), 8.f);
				m_pCamera_Main->CameraEffect(CCamera_Main::CAM_EFT_HIT, g_fDeltaTime);
				m_BossStatusUI->Change_VersusPoint(1);
			}
			((CMonsterParent*)hitobj)->Die();
		}
	}
	return S_OK;
}

void CPlayer::Set_PlayerPause(_float TotalPauseTime, const _tchar* TagAnim, _float fFrameTime)
{
	m_bPause = true;
	m_fDeadNPauseTime = 0;
	m_fTotalPauseTime = TotalPauseTime;
	m_szReturnAnimTag = m_ComTexture->Get_NowTextureTag();
	m_ComTexture->Change_TextureLayer_Wait(TagAnim, fFrameTime);
}

HRESULT CPlayer::ReInitialize(void * pArg)
{
	if (pArg == nullptr)
		return E_FAIL;
	_float3 vStartPos = *(_float3*)(pArg);

	m_fNowJumpPower = 0.f;
	m_fJumpPower = 10.f;
	m_iPlayerLife = 3;

	m_bIsDead = false;
	m_bReHurtTime = 0;
	m_fDeadNPauseTime = 0;
	m_fTotalPauseTime = 0;
	m_bPause = false;
	m_bIsStageEnd = 0;
	m_bTextureReverse = false;
	m_bIsShdow = false;
	Safe_Release(m_pCarryObject);
	m_pCarryObject = nullptr;
	m_bIsCliming = false;
	m_bIsRunning = false;

	m_vDownstairsNear = NOT_EXIST_BLOCK;
	m_vClimingBlock = NOT_EXIST_BLOCK;
	m_vReturnStair = NOT_EXIST_BLOCK;	

	m_ComTransform->Scaled(_float3(1.5f, 1.5f, 1.5f));
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vStartPos);
	m_ComTransform->Set_MoveSpeed(2.5f);

	if (vStartPos != _float3(0, 1, 0) && vStartPos != _float3(0, 11, 0))
	{
		m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("exit"), TEXT("Idle"), 9.f);
		GetSingle(CGameInstance)->PlaySound(TEXT("JY_exitdoor.wav"), CHANNEL_PLAYER, 1.f); 
	}
	else
	{
		m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("wakeup"), TEXT("Idle"));

	}


	return S_OK;
}

const _tchar * CPlayer::Get_NowTextureTag()
{
	return m_ComTexture->Get_NowTextureTag();
}

HRESULT CPlayer::Set_StageEnd(_int IsKindsOfEnd)
{
	if (!m_bIsStageEnd) {


		switch (IsKindsOfEnd)
		{
		case 0: // Stage End By Die

			m_bIsStageEnd = 2;
			m_ComTexture->Change_TextureLayer_Wait(TEXT("die"), 10.f);
			GetSingle(CGameInstance)->PlaySound(TEXT("JY_isaacdies.mp3"), CHANNEL_PLAYER, 1.f);
			GetSingle(CGameInstance)->Stop_ChannelSound(CHANNEL_BGM);


			break;

		case 1:	// Stage End By StageClear
		{

			m_bIsStageEnd = 1;
			m_ComTexture->Change_TextureLayer(TEXT("victory"), 10.f);
			m_pCamera_Main->Set_VictoryTurnAxis(m_ComTransform->Get_MatrixState(CTransform::STATE_POS), m_vCameraPivot);
			m_pCamera_Main->CameraEffect(CCamera_Main::CAM_EFT_VICTORY, g_fDeltaTime, 5.f);

			CUI_Result* TempResult = ((CUI_Result*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(m_eNowSceneNum, L"Layer_UI_Result")));
			TempResult->Set_Clear_Wait_AnimTime(true, 7.f);
			GetSingle(CGameInstance)->Stop_ChannelSound(CHANNEL_BGM);
			GetSingle(CGameInstance)->PlaySound(TEXT("JY_opentreasure.wav"), CHANNEL_PLAYER);

			_float3 PlayerPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
			_float4 Arg = { PlayerPos.x,PlayerPos.y,PlayerPos.z,_float(TempResult->Get_RankStar())};


			if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENE_STATIC, L"Layer_ClearEffect", TEXT("ProtoType_GameObject_Effect_StageClear")
			,&Arg))
				return E_FAIL;
			break;
		}

		case 2:
			m_bIsStageEnd = 3;
			m_ComTexture->Change_TextureLayer_Wait(TEXT("enter"), 8.f);
			GetSingle(CGameInstance)->PlaySound(TEXT("JY_enterdoor.wav"), CHANNEL_PLAYER, 1.f);

			break;
		default:
			break;
		}
	}
	return S_OK;

}

HRESULT CPlayer::Get_PlayerLoginDesc(LOGINDESC * pOutLoginData)
{

	if (m_ComInventory == nullptr)
		return E_FAIL;


	pOutLoginData->szID = m_LoginDesc.szID;
	pOutLoginData->szPassword = m_LoginDesc.szPassword;

	for (_uint i = 0 ; i< SCENE_END; i++)
	{
		pOutLoginData->iArrStageBestClear[i] = StageBestClear[i];
	}


	for (_uint i = 0;i<SKILL_END;i++)
	{
		pOutLoginData->iArrSkillLevel[i] = m_ComInventory->Get_Skill_Level(i);
	}

	pOutLoginData->iGold = m_ComInventory->Get_Gold();

	for (_uint i = 0; i<QUEST_END; i++)
	{
		pOutLoginData->iArrQuestProgress[i] = GetSingle(CQuest)->Get_QuestCurrent(i);
	}

	return S_OK;
}

HRESULT CPlayer::Set_PlayerLoginDesc(LOGINDESC pOutLoginData)
{
	return S_OK;
}

void CPlayer::SetBestClear(_uint _Stage, _float _timer)
{

	if (!StageBestClear[_Stage] ||StageBestClear[_Stage] > _timer)
	{
		StageBestClear[_Stage] = _timer;
	}
}

void CPlayer::Particle_NewDust()
{
	PARTICLEDESC tDesc;
	//파티클이 흩날리는 종류 설정
	tDesc.eParticleID = Particle_Ball;

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 1.7f;

	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 0.3f;

	//파티클의 사이즈를 설정
	tDesc.ParticleSize = _float3(3.f, 3.f, 3.f);
	//파티클의 파워(이동속도)를 결정
	tDesc.Particle_Power = 5;
	//파티클의 파워(이동속도)의 랜덤 범위를 결정
	tDesc.PowerRandomRange = _float2(0.8f, 1.2f);
	//파티클이 한번에 최대 몇개까지 보일 것인지 설정
	tDesc.MaxParticleCount = 10;

	//파티클 텍스처 컴포넌트 이름을 설정 (기본적으로 자기 씬에 컴포넌트가 있는지 검사하고 스테틱에있는지도 검사함)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("Particle_NewDust");
	//텍스처 오토프레임을 사용할 것인지 말 것인지 결정
	tDesc.m_bIsTextureAutoFrame = true;


	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	tDesc.FollowingTarget = m_ComTransform;
	//tDesc.FixedTarget = _float3(0,3,0);


	//파티클의 최대 이탈 범위(range)를 설정해 줌 
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	tDesc.MaxBoundary = _float3(0.7f, 0.7f, 0.7f);


	//텍스처의 색상을 변경할 수 있는 기능 온오프
	//만약 true로 사용할 경우 텍스처의 원래 색상은 무시되고 타겟 색상으로 반짝반짝 거리게 설정됨
	//true로 사용할 경우 반드시 타겟 컬러를 설정해 줄 것
	tDesc.ParticleColorChage = false;
	tDesc.TargetColor = _float3(255.f, 255.f, 0.f);
	tDesc.TargetColor2 = _float3(244.f, 40.f, 11.f);


	//만약 UI에 그려져야한다면 true 월드에 그려져야한다면 false 로 설정할 것
	//UI 로 그리게 될 경우 위의 모든 좌표는 API 좌표 기준으로 셋팅할 것
	//World로 그리게 될 경우 위의 모든 좌표는 월드 좌표 기준으로 셋팅할 것
	tDesc.m_bIsUI = false;
	//UI Depth 설정 (UI 가려지는거 순서 결정할때 쓰는 변수)
	tDesc.m_bUIDepth = 1;

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;

	tDesc.vUp = _float3(0, 0, 0);

	tDesc.ParticleStartRandomPosMin = _float3(-0.3f, -0.3f, -0.3f);
	tDesc.ParticleStartRandomPosMax = _float3(0.3f, 0.3f, 0.3f);

	tDesc.MustDraw = true;
	//오브젝트 뒤에 가려지지 않게 만듬

	tDesc.IsParticleFameEndtoDie = false;
	//프레임이 한번만 돌것인지 정함
	tDesc.AlphaBlendON = true;
	tDesc.m_fAlphaTestValue = 0.f;

	//Create_ParticleObject를 호출하여 스테이지 아이디와 지금까지 설정한 desc를 넣어주면 됨
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);
}

void CPlayer::Particle_Dust(CTransform* _pTransform)
{
	PARTICLEDESC tDesc;
	//파티클이 흩날리는 종류 설정
	tDesc.eParticleID = Particle_Fixed;

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 0.f;
	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 100.f;

	//파티클의 사이즈를 설정
	tDesc.ParticleSize = _float3(3.f, 3.f, 3.f);
	//파티클의 파워(이동속도)를 결정
	tDesc.Particle_Power = 1;
	//파티클의 파워(이동속도)의 랜덤 범위를 결정
	tDesc.PowerRandomRange = _float2(1.f, 1.f);

	//파티클이 한번에 최대 몇개까지 보일 것인지 설정
	tDesc.MaxParticleCount = 1;

	//파티클 텍스처 컴포넌트 이름을 설정 (기본적으로 자기 씬에 컴포넌트가 있는지 검사하고 스테틱에있는지도 검사함)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("Particle_Dust");
	//텍스처 오토프레임을 사용할 것인지 말 것인지 결정
	tDesc.m_bIsTextureAutoFrame = true;
	tDesc.fAutoFrameMul = 2.5f;
	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨

	tDesc.FollowingTarget = _pTransform;//(CTransform*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player))->Get_Component(TAG_COM(Com_Transform)));
	//_float3 ParticlePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
	//tDesc.FixedTarget = ParticlePos + _float3(0.2f, 0.f, 0.f);

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
	tDesc.vUp = _float3(0, 0.f, 0);


	tDesc.ParticleStartRandomPosMin = _float3(0.1f, 0.1f, 0.f);
	tDesc.ParticleStartRandomPosMax = _float3(0.1f, 0.1f, 0.f);

	tDesc.MustDraw = true;
	//오브젝트 뒤에 가려지지 않게 만듬

	tDesc.IsParticleFameEndtoDie = true;
	//프레임이 한번만 돌것인지 정함

	//Create_ParticleObject를 호출하여 스테이지 아이디와 지금까지 설정한 desc를 넣어주면 됨
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);
}

void CPlayer::Particle_GameOver()
{
	PARTICLEDESC tDesc;
	//파티클이 흩날리는 종류 설정
	tDesc.eParticleID = Particle_Fixed;

	//총 파티클이 몇초동안 흩날릴 것인지 설정
	tDesc.TotalParticleTime = 0.f;

	//파티클 하나 하나가 몇초동안 흩날릴 것인지 설정
	tDesc.EachParticleLifeTime = 1.f;

	//파티클의 사이즈를 설정
	tDesc.ParticleSize = _float3(2.f, 2.f, 2.f);
	//파티클의 파워(이동속도)를 결정
	tDesc.Particle_Power = 5;
	//파티클의 파워(이동속도)의 랜덤 범위를 결정
	tDesc.PowerRandomRange = _float2(0.8f, 1.2f);
	//파티클이 한번에 최대 몇개까지 보일 것인지 설정
	tDesc.MaxParticleCount = 1;

	//파티클 텍스처 컴포넌트 이름을 설정 (기본적으로 자기 씬에 컴포넌트가 있는지 검사하고 스테틱에있는지도 검사함)
	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");
	//파티클 텍스처 레이어 스테이트키를 변경할 수 있음
	tDesc.szTextureLayerTag = TEXT("Particle_GameOver");
	//텍스처 오토프레임을 사용할 것인지 말 것인지 결정
	tDesc.m_bIsTextureAutoFrame = true;


	//FixedTarget 을 사용하면 고정된 위치에서 계속해서 나오고
	//FollowingTarget을 사용하면 해당 오브젝트를 따라다니면서 파티클이 흩날려짐
	//단 둘중 하나만 사용 가능
	//둘다 사용하고 싶을 경우에는 파티클을 2개 만들어서 사용할 것
	//FollowingTarget의 경우 따라다녀야할 오브젝트의 CTransform 컴포넌트를 넣어주면 됨
	tDesc.FollowingTarget = m_ComTransform;
	//tDesc.FixedTarget = _float3(0,3,0);


	//파티클의 최대 이탈 범위(range)를 설정해 줌 
	//FollowingTarget 이나 FixedTarget 의 좌표 기준으로 해당 범위(+, -)를 벗어나지 않음
	tDesc.MaxBoundary = _float3(10.f, 10.f, 10.f);


	//텍스처의 색상을 변경할 수 있는 기능 온오프
	//만약 true로 사용할 경우 텍스처의 원래 색상은 무시되고 타겟 색상으로 반짝반짝 거리게 설정됨
	//true로 사용할 경우 반드시 타겟 컬러를 설정해 줄 것
	tDesc.ParticleColorChage = false;
	tDesc.TargetColor = _float3(255.f, 255.f, 0.f);
	tDesc.TargetColor2 = _float3(244.f, 40.f, 11.f);


	//만약 UI에 그려져야한다면 true 월드에 그려져야한다면 false 로 설정할 것
	//UI 로 그리게 될 경우 위의 모든 좌표는 API 좌표 기준으로 셋팅할 것
	//World로 그리게 될 경우 위의 모든 좌표는 월드 좌표 기준으로 셋팅할 것
	tDesc.m_bIsUI = false;
	//UI Depth 설정 (UI 가려지는거 순서 결정할때 쓰는 변수)
	tDesc.m_bUIDepth = 1;

	//방향을 설정하고 싶을 때 사용하는 옵션
	//ex) straight를 사용하는데 오브젝트의 오른쪽으로 뿌리고 싶으면 오브젝트의 right를 넣어주면 됨
	//혹은 x축의 양의 방향으로 뿌리고 싶으면 _float3(1,0,0); 이런식으로 넣어주면 됨;

	tDesc.vUp = _float3(0, 0, 0);

	tDesc.ParticleStartRandomPosMin = _float3(-0.5f, 1.f, -0.5f);
	tDesc.ParticleStartRandomPosMax = _float3(0.5f, 2.f, 0.5f);

	tDesc.MustDraw = true;
	//오브젝트 뒤에 가려지지 않게 만듬

	tDesc.IsParticleFameEndtoDie = false;
	//프레임이 한번만 돌것인지 정함
	//tDesc.AlphaBlendON = true;
	tDesc.m_fAlphaTestValue = 0.f;

	//Create_ParticleObject를 호출하여 스테이지 아이디와 지금까지 설정한 desc를 넣어주면 됨
	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);

}

HRESULT CPlayer::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fMovePerSec = 3.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.vPivot = _float3(0, -0.25f, 0);


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Player), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	_int iMaxSkillNum;
	iMaxSkillNum = SKILL_END;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Inventory), TAG_COM(Com_Inventory), (CComponent**)&m_ComInventory, &iMaxSkillNum)))
		return E_FAIL;
	
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collision), TAG_COM(Com_Collision), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_CollisionView), TAG_COM(Com_CollisionView), (CComponent**)&m_pCollisionViewCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::SetUp_LoginDesc()
{
	FAILED_CHECK(GetSingle(CLoginMgr)->Get_LoginDesc(&m_LoginDesc));

	m_ComInventory->Set_Skill_MaxLevel(SKILL_SPEEDUP, 5);

	for (_uint i = 0; i < SCENE_END; i++)
	{
		StageBestClear[i] = m_LoginDesc.iArrStageBestClear[i];
	};


	for (_uint i = 0; i < QUEST_END; i++)
	{

		GetSingle(CQuest)->Set_QuestIndexIncrease(i, m_LoginDesc.iArrQuestProgress[i]);
	}

	for (_uint i = 0; i < SKILL_END; i++)
	{

		for (_uint j = 0; j < m_LoginDesc.iArrSkillLevel[i]; j++)
		{
			m_ComInventory->Set_Skill_LevelUP(i);
		}
	}


	m_ComInventory->Set_Gold(m_LoginDesc.iGold);


	return S_OK;
}

HRESULT CPlayer::SetUp_ParticleDesc()
{
	
	m_ArrParticleDesc[0].eParticleID = Particle_Fixed;
	m_ArrParticleDesc[0].TotalParticleTime = 0.f;
	m_ArrParticleDesc[0].EachParticleLifeTime = 1000.f;
	m_ArrParticleDesc[0].ParticleSize = _float3(1.5f, 0.75f, 1.5f);
	m_ArrParticleDesc[0].Particle_Power = 1;
	m_ArrParticleDesc[0].PowerRandomRange = _float2(0.9f, 1.1f);
	m_ArrParticleDesc[0].MaxParticleCount = 1;
	m_ArrParticleDesc[0].szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JY_Effect");
	m_ArrParticleDesc[0].szTextureLayerTag = TEXT("Jump_Effect");
	m_ArrParticleDesc[0].m_bIsTextureAutoFrame = true;
	m_ArrParticleDesc[0].fAutoFrameMul = 2.f;
	m_ArrParticleDesc[0].FollowingTarget = m_ComTransform;
	m_ArrParticleDesc[0].MaxBoundary = _float3(10, 10, 10);
	m_ArrParticleDesc[0].ParticleColorChage = false;
	m_ArrParticleDesc[0].TargetColor = _float3(237, 28, 36);
	m_ArrParticleDesc[0].TargetColor2 = _float3(53, 255.f, 11);
	m_ArrParticleDesc[0].m_bIsUI = false;
	m_ArrParticleDesc[0].vUp = _float3(0, 1, 0);
	m_ArrParticleDesc[0].ParticleStartRandomPosMin = _float3(0.0f, -0.5f, 0.0f);
	m_ArrParticleDesc[0].ParticleStartRandomPosMax = _float3(0.f, -0.5f, 0.0f);
	m_ArrParticleDesc[0].MustDraw = true;
	m_ArrParticleDesc[0].IsParticleFameEndtoDie = true;

	m_ArrParticleDesc[1].eParticleID = Particle_Fixed;
	m_ArrParticleDesc[1].TotalParticleTime = 0.f;
	m_ArrParticleDesc[1].EachParticleLifeTime = 1000.f;
	m_ArrParticleDesc[1].ParticleSize = _float3(2.25f, 1.05f, 2.25f);
	m_ArrParticleDesc[1].Particle_Power = 1;
	m_ArrParticleDesc[1].PowerRandomRange = _float2(0.9f, 1.1f);
	m_ArrParticleDesc[1].MaxParticleCount = 1;
	m_ArrParticleDesc[1].szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JY_Effect");
	m_ArrParticleDesc[1].szTextureLayerTag = TEXT("DuobleJump_Effect_1");
	m_ArrParticleDesc[1].m_bIsTextureAutoFrame = true;
	m_ArrParticleDesc[1].fAutoFrameMul = 2.f;
	m_ArrParticleDesc[1].FollowingTarget = m_ComTransform;
	m_ArrParticleDesc[1].MaxBoundary = _float3(10, 10, 10);
	m_ArrParticleDesc[1].ParticleColorChage = false;
	m_ArrParticleDesc[1].TargetColor = _float3(237, 28, 36);
	m_ArrParticleDesc[1].TargetColor2 = _float3(53, 255.f, 11);
	m_ArrParticleDesc[1].m_bIsUI = false;
	m_ArrParticleDesc[1].vUp = _float3(0, 1, 0);
	m_ArrParticleDesc[1].ParticleStartRandomPosMin = _float3(0.0f, -0.5f, 0.0f);
	m_ArrParticleDesc[1].ParticleStartRandomPosMax = _float3(0.f, -0.5f, 0.0f);
	m_ArrParticleDesc[1].MustDraw = true;
	m_ArrParticleDesc[1].IsParticleFameEndtoDie = true;

	m_ArrParticleDesc[2].eParticleID = Particle_Fixed;
	m_ArrParticleDesc[2].TotalParticleTime = 0.f;
	m_ArrParticleDesc[2].EachParticleLifeTime = 1000.f;
	m_ArrParticleDesc[2].ParticleSize = _float3(1.0f, 0.7f, 1.0f);
	m_ArrParticleDesc[2].Particle_Power = 1;
	m_ArrParticleDesc[2].PowerRandomRange = _float2(0.9f, 1.1f);
	m_ArrParticleDesc[2].MaxParticleCount = 1;
	m_ArrParticleDesc[2].szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JY_Effect");
	m_ArrParticleDesc[2].szTextureLayerTag = TEXT("Landing_Effect");
	m_ArrParticleDesc[2].m_bIsTextureAutoFrame = true;
	m_ArrParticleDesc[2].fAutoFrameMul = 4.f;
	m_ArrParticleDesc[2].FollowingTarget = m_ComTransform;
	//m_ArrParticleDesc[2].FixedTarget = _float3(10, 10, 10);
	m_ArrParticleDesc[2].MaxBoundary = _float3(10, 10, 10);
	m_ArrParticleDesc[2].ParticleColorChage = false;
	m_ArrParticleDesc[2].TargetColor = _float3(237, 28, 36);
	m_ArrParticleDesc[2].TargetColor2 = _float3(53, 255.f, 11);
	m_ArrParticleDesc[2].m_bIsUI = false;
	m_ArrParticleDesc[2].vUp = _float3(0, 1, 0);
	m_ArrParticleDesc[2].ParticleStartRandomPosMin = _float3(0.0f, -0.5f, 0.0f);
	m_ArrParticleDesc[2].ParticleStartRandomPosMax = _float3(0.f, -0.5f, 0.0f);
	m_ArrParticleDesc[2].MustDraw = true;
	m_ArrParticleDesc[2].IsParticleFameEndtoDie = true;

	m_ArrParticleDesc[3].eParticleID = Particle_Fixed;
	m_ArrParticleDesc[3].TotalParticleTime = 0.f;
	m_ArrParticleDesc[3].EachParticleLifeTime = 0.8f;
	m_ArrParticleDesc[3].ParticleSize = _float3(1.5f, 2.25f, 1.5f);
	m_ArrParticleDesc[3].Particle_Power = 1;
	m_ArrParticleDesc[3].PowerRandomRange = _float2(0.9f, 1.1f);
	m_ArrParticleDesc[3].MaxParticleCount = 1;
	m_ArrParticleDesc[3].szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JY_Effect");
	m_ArrParticleDesc[3].szTextureLayerTag = TEXT("Running_Effect");
	m_ArrParticleDesc[3].m_bIsTextureAutoFrame = true;
	m_ArrParticleDesc[3].fAutoFrameMul = 1.f;
	m_ArrParticleDesc[3].FollowingTarget = m_ComTransform;
	m_ArrParticleDesc[3].MaxBoundary = _float3(10, 10, 10);
	m_ArrParticleDesc[3].ParticleColorChage = true;
	m_ArrParticleDesc[3].TargetColor = _float3(47, 88, 255);
	m_ArrParticleDesc[3].TargetColor2 = _float3(255, 47, 47);
	m_ArrParticleDesc[3].m_bIsUI = false;
	m_ArrParticleDesc[3].vUp = _float3(0, 1, 0);
	m_ArrParticleDesc[3].ParticleStartRandomPosMin = _float3(0.0f, -0.2f, 0.0f);
	m_ArrParticleDesc[3].ParticleStartRandomPosMax = _float3(0.f, -0.2f, 0.0f);
	m_ArrParticleDesc[3].MustDraw = true;
	m_ArrParticleDesc[3].IsParticleFameEndtoDie = true;


	m_ArrParticleDesc[4].eParticleID = Particle_Straight;
	m_ArrParticleDesc[4].TotalParticleTime = 0.5f;
	m_ArrParticleDesc[4].EachParticleLifeTime = 0.5;
	m_ArrParticleDesc[4].ParticleSize = _float3(0.3f, 0.3f, 0.3f);
	m_ArrParticleDesc[4].Particle_Power = 1;
	m_ArrParticleDesc[4].PowerRandomRange = _float2(0.5f, 1.5f);
	m_ArrParticleDesc[4].MaxParticleCount = 2;
	m_ArrParticleDesc[4].szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_JY_Effect");
	m_ArrParticleDesc[4].szTextureLayerTag = TEXT("Running_Particle");
	m_ArrParticleDesc[4].m_bIsTextureAutoFrame = true;
	m_ArrParticleDesc[4].fAutoFrameMul = 3.f;
	m_ArrParticleDesc[4].FollowingTarget = m_ComTransform;
	m_ArrParticleDesc[4].MaxBoundary = _float3(10, 10, 10);
	m_ArrParticleDesc[4].ParticleColorChage = false;
	m_ArrParticleDesc[4].TargetColor = _float3(237, 28, 36);
	m_ArrParticleDesc[4].TargetColor2 = _float3(53, 255.f, 11);
	m_ArrParticleDesc[4].m_bIsUI = false;
	m_ArrParticleDesc[4].ParticleStartRandomPosMin = _float3(0.0f, -0.5f, 0.0f);
	m_ArrParticleDesc[4].ParticleStartRandomPosMax = _float3(0.f, 0.0f, 0.0f);
	m_ArrParticleDesc[4].MustDraw = true;
	m_ArrParticleDesc[4].IsParticleFameEndtoDie = false;

	m_ArrParticleDesc[5].eParticleID = Particle_Straight;
	m_ArrParticleDesc[5].TotalParticleTime = 1.0f;
	m_ArrParticleDesc[5].EachParticleLifeTime = 1.0;
	m_ArrParticleDesc[5].ParticleSize = _float3(0.3f, 0.3f, 0.3f);
	m_ArrParticleDesc[5].Particle_Power = 1;
	m_ArrParticleDesc[5].PowerRandomRange = _float2(0.5f, 1.5f);
	m_ArrParticleDesc[5].MaxParticleCount = 20;
	m_ArrParticleDesc[5].szTextureProtoTypeTag = TAG_CP(Prototype_Texture_UI);
	m_ArrParticleDesc[5].szTextureLayerTag = TEXT("DUBBLEJUMP");
	m_ArrParticleDesc[5].m_bIsTextureAutoFrame = true;
	m_ArrParticleDesc[5].fAutoFrameMul = 3.f;
	m_ArrParticleDesc[5].FollowingTarget = m_ComTransform;
	m_ArrParticleDesc[5].MaxBoundary = _float3(10, 10, 10);
	m_ArrParticleDesc[5].ParticleColorChage = true;
	m_ArrParticleDesc[5].TargetColor = _float3(237, 28, 36);
	m_ArrParticleDesc[5].TargetColor2 = _float3(53, 255.f, 11);
	m_ArrParticleDesc[5].m_bIsUI = false;
	m_ArrParticleDesc[5].ParticleStartRandomPosMin = _float3(-0.5f, -0.5f, -0.5f);
	m_ArrParticleDesc[5].ParticleStartRandomPosMax = _float3(0.5f, 0.5f, 0.5f);
	m_ArrParticleDesc[5].MustDraw = true;
	m_ArrParticleDesc[5].IsParticleFameEndtoDie = false;
	m_ArrParticleDesc[5].vUp = _float3(0, 1, 0);
	//Create_ParticleObject를 호출하여 스테이지 아이디와 지금까지 설정한 desc를 넣어주면 됨
	//GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);
	return S_OK;
}

HRESULT CPlayer::Input_Keyboard(_float fDeltaTime)
{
	m_fFrame = fDeltaTime;
	CGameInstance* pInstance = GetSingle(CGameInstance);


	if ((m_pCarryObject != nullptr) && pInstance->Get_DIKeyState(DIK_LSHIFT) & DIS_Up) {

		m_pCarryObjectTransform->Set_MatrixState(CTransform::STATE_POS,
			m_ComTransform->Get_MatrixState(CTransform::STATE_POS) +
			(m_ComTransform->Get_MatrixState(CTransform::STATE_RIGHT) * 0.6f));

		Particle_Dust(m_pCarryObjectTransform);

		Safe_Release(m_pCarryObject);
		Safe_Release(m_pCarryObjectTransform);

		m_pCarryObject = nullptr;

		m_ComTransform->Set_MoveSpeed(2.5f);
		m_fJumpPower *= 2.f;

		m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("carrydown"), TEXT("Idle"),12.f);
		GetSingle(CGameInstance)->PlaySound(TEXT("JY_Box_Drop.mp3"), CHANNEL_OBJECT);
		GetSingle(CGameInstance)->PlaySound(TEXT("JY_Isaac_Hurt_Grunt2.mp3"), CHANNEL_PLAYER, 1.f);

		//GetSingle(CGameInstance)->PlaySound(L"EH_CarryDown.wav", CHANNEL_OBJECT);
	}
	
	
	//등반
	if (!m_pCarryObject) {
		if (pInstance->Get_DIKeyState(DIK_UP) & DIS_Press)
		{
			if (m_vClimingBlock != NOT_EXIST_BLOCK)
			{
				m_ComTransform->MovetoDir(_float3(0, 1.f, 0), fDeltaTime);
				m_fNowJumpPower = 0;
				m_bIsJumped = 0;
				m_bIsCliming = true;
			}

		}

		if (pInstance->Get_DIKeyState(DIK_DOWN) & DIS_Press)
		{
			if (m_bIsCliming)
			{
				if (m_vClimingBlock != NOT_EXIST_BLOCK)
				{
					m_fNowJumpPower = 0;
					if (m_vDownstairsNear != NOT_EXIST_BLOCK)
					{
						m_fNowJumpPower = -fDeltaTime;
						m_bIsCliming = false;
						GetSingle(CGameInstance)->PlaySound(TEXT("JY_climboverladder.wav"), CHANNEL_PLAYER, 2.f);
					}
					else
					{
						if (!(m_ComTexture->Get_IsReturnTexture()))
						{
							m_ComTransform->MovetoDir(_float3(0, -1.f, 0), fDeltaTime);
							m_bIsCliming = true;
						}
					}
				}
		
			}
			else
			{
				if (m_vDownstairsNear != NOT_EXIST_BLOCK)
				{

					if (pInstance->Get_DIKeyState(DIK_DOWN) & DIS_DoubleDown)
					{
						//m_ComTransform->MovetoDir(_float3(0, -1.f, 0), fDeltaTime);

						_Matrix matVeiwSpace;
						m_pGraphicDevice->GetTransform(D3DTS_VIEW, &matVeiwSpace);

						_float3 vPlayerViewPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS).PosVector_Matrix(matVeiwSpace);

						vPlayerViewPos.y = m_vDownstairsNear.y + 0.45f;
						vPlayerViewPos.z = m_vDownstairsNear.z - 0.6f;

						m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vPlayerViewPos.PosVector_Matrix(matVeiwSpace.InverseMatrix()));

						m_fNowJumpPower = 0;
						m_bIsCliming = true;
						//m_ComTexture->Change_TextureLayer_Wait(TEXT("pull_down"),12.f);
						m_ComTexture->Change_TextureLayer_ReturnToWait(TEXT("pull_down"), TEXT("climing_back"), 12.f, 12.f);

						GetSingle(CGameInstance)->PlaySound(TEXT("JY_ledgeshimmy.wav"), CHANNEL_PLAYER);
					}


				}
			}
		}
	}
	///////////////////진우스킬타이머/////////////////
	if (m_ComInventory->Get_Skill_Level(SKILL_POTION) &&pInstance->Get_DIKeyState(DIK_T) & DIS_Down)
	{
		if ((pInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, L"Layer_UI_Result")) != nullptr)
		{
			if (m_tCoolDown[SKILL_POTION].m_bCoolDownStart)
			{
			}
			else {
				/*if (((CUI_Result*)(pInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, L"Layer_UI_Result")))->Get_NowTime() < 30)
					((CUI_Result*)(pInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, L"Layer_UI_Result")))->Set_PlusTimer(((CUI_Result*)(pInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, L"Layer_UI_Result")))->Get_NowTime()-30);
				else*/
				GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, m_ArrParticleDesc[5]);
				GetSingle(CGameInstance)->PlaySound(TEXT("JW_Timeup.ogg"), CHANNEL_PLAYER);
					((CUI_Result*)(pInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, L"Layer_UI_Result")))->Set_PlusTimer(30);
					((CUI_Result*)(pInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, L"Layer_UI_Result")))->Set_MaxTimebyTimeUp(30);
				m_tCoolDown[SKILL_POTION].m_bCoolDownStart = true;
			}
		}
	}
	//////////////////////////////////////////////////
	//좌우 이동
	if (pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Press)
	{
		m_ComTransform->Move_Right(fDeltaTime);
	}

	else if (pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Press)
	{
		m_ComTransform->Move_Right(fDeltaTime);

	}

	//점프
	if (m_bIsJumped < (_uint)(m_ComInventory->Get_Skill_Level(SKILL_DUBBLEJUMP) + 1) && pInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
	{
		if (m_bIsJumped > 0)
		{
			if (m_tCoolDown[SKILL_DUBBLEJUMP].m_bCoolDownStart) 
			{
				return S_OK;
			}
			else {
				m_tCoolDown[SKILL_DUBBLEJUMP].m_bCoolDownStart = true;

			}

		}

		if (m_pCarryObject)
			m_ComTexture->Change_TextureLayer_Wait(TEXT("carryjumpup"), 8.f);
		else
			m_ComTexture->Change_TextureLayer_Wait(TEXT("jump_up"), 8.f);


		if (m_bIsJumped == 0)
			GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, m_ArrParticleDesc[0]);
		else if (m_bIsJumped == 1)
			GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, m_ArrParticleDesc[1]);

		m_fNowJumpPower = m_fJumpPower;
		m_bIsJumped++;
		m_bIsCliming = false;
		m_ComTransform->MovetoDir(_float3(0, 1.f, 0), fDeltaTime);
		GetSingle(CGameInstance)->PlaySound(TEXT("JY_Player_jump.wav"), CHANNEL_PLAYER);
	}
	
	return S_OK;
}

HRESULT CPlayer::Animation_Change(_float fDeltaTime)
{

	CGameInstance* pInstance = GetSingle(CGameInstance);

	if (pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Press)
		m_bTextureReverse = true;
	else if (pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Press)
		m_bTextureReverse = false;

	if (m_pCarryObject) {

		if (m_fNowJumpPower == 0 && !(m_ComTexture->Get_IsReturnTexture()))
		{

			if (pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Press || pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Press)
			{
				m_ComTexture->Change_TextureLayer_Wait(TEXT("carrywalk"));

				if (_int(m_NowAnimFrameTimeCheckerForSound) != _int(m_OldAnimFrameTimeCheckerForSound) &&
					_int(m_NowAnimFrameTimeCheckerForSound) % 4 == 0)
				{
					

					if (m_bTextureReverse)
						GetSingle(CGameInstance)->PlaySound(TEXT("JY_step01.wav"), CHANNEL_PLAYER, 2.f);
					else
						GetSingle(CGameInstance)->PlaySound(TEXT("JY_step03.wav"), CHANNEL_PLAYER, 2.f);

				}
			}

		}

		return S_OK;
	}


	if (m_fNowJumpPower == 0) 
	{

		if (pInstance->Get_DIKeyState(DIK_UP) & DIS_Press || pInstance->Get_DIKeyState(DIK_DOWN) & DIS_Press ||
			pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Press || pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Press)
		{
			if (m_bIsCliming)
			{
				if (!m_pCarryObject) 
				{
					m_bIsRunning = false;
					m_ComTransform->Set_MoveSpeed(2.5f);

					if (!(m_ComTexture->Get_IsReturnTexture()))
					{
						m_ComTexture->Change_TextureLayer_Wait(TEXT("climing_back"), 12.f);

						if (_int(m_NowAnimFrameTimeCheckerForSound) != _int(m_OldAnimFrameTimeCheckerForSound) &&
							_int(m_NowAnimFrameTimeCheckerForSound) % 3 == 0)
						{
								GetSingle(CGameInstance)->PlaySound(TEXT("JY_climbladder.wav"), CHANNEL_PLAYER);

						}


					}
				}
			}
			else
			{
				//if (lstrcmp(m_ComTexture->Get_NowTextureTag(), TEXT("jump_down"))) 
				if (!(m_ComTexture->Get_IsReturnTexture()))
				{
					if ((m_ComInventory->Get_Skill_Level(SKILL_SPEEDUP)) && ((pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_DoubleDown || pInstance->Get_DIKeyState(DIK_LEFT) & DIS_DoubleDown)))
					{
						if (m_tCoolDown[SKILL_SPEEDUP].m_bCoolDownStart)
						{
							m_bIsRunning = false;
							m_ComTransform->Set_MoveSpeed(2.5f);
						}
						else {
							m_tCoolDown[SKILL_SPEEDUP].m_bCoolDownStart = true;
							m_bIsRunning = true;
							m_ComTransform->Set_MoveSpeed(2.5f + m_ComInventory->Get_Skill_Level(SKILL_SPEEDUP) * 0.25f);
							GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, m_ArrParticleDesc[3]);

						}
					}
					else if(pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Down || pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Down)
					{
						m_bIsRunning = false;
						m_ComTransform->Set_MoveSpeed(2.5f);
					}
					else if (pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Up|| pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Up)
					{
						m_bIsRunning = false;
					}

					if (m_bIsRunning)
						m_ComTexture->Change_TextureLayer(TEXT("run"));
					else 
						m_ComTexture->Change_TextureLayer(TEXT("walk"));

					if (_int(m_NowAnimFrameTimeCheckerForSound) != _int(m_OldAnimFrameTimeCheckerForSound) &&
						_int(m_NowAnimFrameTimeCheckerForSound) % 3 == 0)
					{

						if (m_bTextureReverse)
							GetSingle(CGameInstance)->PlaySound(TEXT("JY_left.wav"), CHANNEL_PLAYER);
						else
							GetSingle(CGameInstance)->PlaySound(TEXT("JY_right.wav"), CHANNEL_PLAYER);

					}

				}
			}

		}
		else {

			if (!m_bIsCliming && !(m_ComTexture->Get_IsReturnTexture()) && !m_ComTexture->Get_IsWaitTexture())
			{
				m_ComTexture->Change_TextureLayer(TEXT("Idle"));
			}
		}


	}



	if (m_bIsRunning)
	{
		m_ArrParticleDesc[4].vUp = m_ComTransform->Get_MatrixState(CTransform::STATE_RIGHT).Get_Inverse();
		GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, m_ArrParticleDesc[4]);
	}

	return S_OK;
}

HRESULT CPlayer::Find_FootHold_Object(_float fDeltaTime)
{


	CGameInstance* pGameInstance = GetSingle(CGameInstance);

	//뷰스페이스 행렬
	_Matrix matVeiwSpace;
	m_pGraphicDevice->GetTransform(D3DTS_VIEW, &matVeiwSpace);

	_float3 vPlayerPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);


	////카메라 바라보도록 설정
	//_float3 vCamLook;
	//memcpy(&vCamLook, &(matVeiwSpace.InverseMatrix().m[2][0]), sizeof(_float3));

	//if (m_bTextureReverse)
	//	m_ComTransform->LookAt(vPlayerPos - vCamLook);
	//else
	//	m_ComTransform->LookAt(vPlayerPos + vCamLook);


	if (m_pCamera_Main->Get_bIsTuring())
		return S_OK;

	_float3 vPlayerViewPos = vPlayerPos.PosVector_Matrix(matVeiwSpace);

	_float Time = 1 - (m_fNowJumpPower / m_fJumpPower);
	_float fGravity = 0;

	if (Time < 0)
		fGravity = (m_fNowJumpPower - Time * Time * m_fJumpPower) * fDeltaTime;
	
	 list<CGameObject*>* pTerrainLayer= pGameInstance->Get_ObjectList_from_Layer(m_eNowSceneNum, TAG_LAY(Layer_Terrain));

	 NULL_CHECK_RETURN(pTerrainLayer,E_FAIL);

	 m_vDownstairsNear = NOT_EXIST_BLOCK;
	 m_vClimingBlock = NOT_EXIST_BLOCK;
	 
	 _float		fPlayerFrontZ	= -(_float)0x0fffffff;
	 _float		fFootNearZ		= (_float)0x0fffffff;
	 _float		fPlayerBackZ	= (_float)0x0fffffff;


	 list<_float3> TerrainViewPos; 
	 TerrainViewPos.clear();

	 auto ObjectListIter = pTerrainLayer->begin();

	 m_bIsShdow = false;
	 //m_bCanMoveLeft = true;
	 //m_bCanMoveRight = true;
	// m_bCanMoveUp = false;

	 _float3 vTerrainWorldPos;
	 _float3 vTerrainObjectViewPos;

	 for (; ObjectListIter != pTerrainLayer->end();)
	 {
		 if (m_pCarryObject == (*ObjectListIter)) {
			 ObjectListIter++;
			 continue;
		 }

		 vTerrainWorldPos = ((CTransform*)((*ObjectListIter)->Find_Components(TAG_COM(Com_Transform))))
			 ->Get_MatrixState(CTransform::STATE_POS);

		 if (vTerrainWorldPos.y < vPlayerPos.y + 0.5f  && vTerrainWorldPos.y >= vPlayerPos.y - 0.5f)
		 {
			 vTerrainObjectViewPos = vTerrainWorldPos.PosVector_Matrix(matVeiwSpace);
			 //같은 높이의
			 if (vPlayerViewPos.x + 0.5f >= vTerrainObjectViewPos.x && vPlayerViewPos.x - 0.5f < vTerrainObjectViewPos.x) 
			 {
				 // 같은 x축의
					 TerrainViewPos.push_back(vTerrainObjectViewPos);
				 if (vTerrainObjectViewPos.z < vPlayerViewPos.z - 0.25f  && vTerrainObjectViewPos.z > fPlayerFrontZ)
				 {
					 fPlayerFrontZ = vTerrainObjectViewPos.z;
					 m_bIsShdow = true;
				 }
				 if (vTerrainObjectViewPos.z > vPlayerViewPos.z + 0.25f && vTerrainObjectViewPos.z < fPlayerBackZ)
				 {
					 fPlayerBackZ = vTerrainObjectViewPos.z;

				 }
			 }

			 //if (vTerrainObjectViewPos.z > vPlayerViewPos.z - 0.25f && vTerrainObjectViewPos.z < vPlayerViewPos.z + 0.25f)
			 //{
				// if (vTerrainObjectViewPos.x > vPlayerViewPos.x + 0.55f  && vTerrainObjectViewPos.x < vPlayerViewPos.x + 1.1f)
				// {
				//	 //m_bCanMoveRight = false;
				// }
				// else if (vTerrainObjectViewPos.x < vPlayerViewPos.x - 0.55f  && vTerrainObjectViewPos.x  > vPlayerViewPos.x - 1.1f)
				// {
				//	 //m_bCanMoveLeft = false;
				// }
			 //}
		 }
		 else if (vTerrainWorldPos.y <= vPlayerPos.y - 0.5f && vTerrainWorldPos.y >= vPlayerPos.y - 1.5f + fGravity)
		 {//아래층의
			 vTerrainObjectViewPos = vTerrainWorldPos.PosVector_Matrix(matVeiwSpace);
			 if (vPlayerViewPos.x + 0.5f >= vTerrainObjectViewPos.x && vPlayerViewPos.x - 0.5f < vTerrainObjectViewPos.x)
			 {//같은 x축

					 TerrainViewPos.push_back(vTerrainObjectViewPos);
				 
			 }
		 }
		 ObjectListIter++;
	 }


	 auto SelectedListiter = TerrainViewPos.begin();

	 for (; SelectedListiter != TerrainViewPos.end();)
	 {
		 if ((*SelectedListiter).y < vPlayerViewPos.y - 0.5f  && (*SelectedListiter).y >= vPlayerViewPos.y - 1.5f + fGravity)
		 {//아래층의
			 if ((*SelectedListiter).z < fFootNearZ  && (*SelectedListiter).z > fPlayerFrontZ && (*SelectedListiter).z < fPlayerBackZ)
			 {
				 m_vDownstairsNear = (*SelectedListiter);
				 fFootNearZ = (*SelectedListiter).z;
			 }
		 }
		 if ((*SelectedListiter).y < vPlayerViewPos.y + 0.5f && (*SelectedListiter).y >= vPlayerViewPos.y - 0.5f)
		 {
			 //같은 층에
			 if ((*SelectedListiter).z > fPlayerFrontZ + 1.f && (*SelectedListiter).z < m_vClimingBlock.z)
			 {
				 m_vClimingBlock = (*SelectedListiter);
			 }
		 }
		 SelectedListiter++;
	 }



	 if (m_vDownstairsNear != NOT_EXIST_BLOCK)
		 m_vReturnStair = m_vDownstairsNear.PosVector_Matrix(matVeiwSpace.InverseMatrix());




	return S_OK;
}

HRESULT CPlayer::Set_PosOnFootHoldObject(_float fDeltaTime)
{


	if (m_pCarryObject) {
		m_pCarryObjectTransform->Set_MatrixState(CTransform::STATE_POS,
			m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 0.5f, 0));
	}

	if (m_pCamera_Main->Get_bIsTuring())
		return S_OK;


	_Matrix matVeiwSpace;
	m_pGraphicDevice->GetTransform(D3DTS_VIEW, &matVeiwSpace);

	_float3 vResultPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);


	/////////중력 계산
	_float Time = 1 - (m_fNowJumpPower / m_fJumpPower);
	_float fGravity = 0;

	if (m_fNowJumpPower != 0)
	{
		m_fNowJumpPower -= fDeltaTime * m_fJumpPower * 1.5f;
		fGravity = (m_fNowJumpPower - Time*Time * m_fJumpPower) * fDeltaTime;
	}


	if (!m_bIsCliming)
	{

		//중력 적용
		vResultPos.y += fGravity;
		vResultPos = vResultPos.PosVector_Matrix(matVeiwSpace);


		if (m_vDownstairsNear != NOT_EXIST_BLOCK)
		{
			vResultPos.z = m_vDownstairsNear.z;
			if (vResultPos.y + fGravity < m_vDownstairsNear.y + 1.0f && m_fNowJumpPower < 0) //플레이어가 지형보다 아래에 있다면
			{
			
				if(!m_bIsStageEnd)
				{
					if (m_pCarryObject)
					{
						m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("carryjumpdown"), TEXT("carryIdle"), 8.f);

					}
					else
					{
						m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("jump_down"), TEXT("Idle"), 8.f);

					}
				}

				vResultPos.y = m_vDownstairsNear.y + 0.95f;
				GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, m_ArrParticleDesc[2]);
				m_fNowJumpPower = 0;
				m_bIsJumped = 0;

			}


		}
		else if (Time > 3.f && m_vReturnStair != NOT_EXIST_BLOCK)
		{
			if (m_bReHurtTime < 1.5f)
			{
				m_bIsDead = true;
				m_fDeadNPauseTime = 4.0f;
				m_bReHurtTime = 0;	

				m_fNowJumpPower = 0;
				m_bIsJumped = 0;
				Time = 0;
			}
			else {

				m_bReHurtTime = 0;
				//피격 이미지 넣어주기/////////////////////////
				
				GetSingle(CGameInstance)->PlaySound(TEXT("JY_Isaac_Hurt_Grunt0.mp3"), CHANNEL_PLAYER);
				GetSingle(CGameInstance)->PlaySound(TEXT("JY_Scared_Whimper_2.mp3"), CHANNEL_PLAYER);
				m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("hurt"), TEXT("Idle"), 8.f);
				//m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("jump_down"), TEXT("Idle"), 8.f);
				vResultPos = m_vReturnStair.PosVector_Matrix(matVeiwSpace);
				vResultPos.y += 1.f;
				m_fNowJumpPower = 0;
				m_bIsJumped = 0;
				Time = 0;
				m_pCamera_Main->CameraEffect(CCamera_Main::CAM_EFT_HIT, fDeltaTime);

			}


		}
		else {
			m_fNowJumpPower -= fDeltaTime * m_fJumpPower;
		}
	}
	else {

		vResultPos = vResultPos.PosVector_Matrix(matVeiwSpace);


		if (m_vDownstairsNear != NOT_EXIST_BLOCK)
		{
			if (vResultPos.y < m_vDownstairsNear.y + 1.0f) //플레이어가 지형보다 아래에 있다면
			{
				m_bIsCliming = false;

				vResultPos.z = m_vDownstairsNear.z;
				m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("jump_down"), TEXT("Idle"), 8.f);
				//GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, m_ArrParticleDesc[2]);
				vResultPos.y = m_vDownstairsNear.y + 0.95f;
				m_fNowJumpPower = 0;
				m_bIsJumped = 0;
				GetSingle(CGameInstance)->PlaySound(TEXT("JY_climboverladder.wav"), CHANNEL_PLAYER, 2.f);
			}
		}
		else if (m_vClimingBlock != NOT_EXIST_BLOCK)
		{
			vResultPos.z = m_vClimingBlock.z - 1.0f;
		}
		else 
		{
			m_bIsCliming = false;
			GetSingle(CGameInstance)->PlaySound(TEXT("JY_climboverladder.wav"), CHANNEL_PLAYER, 2.f);
			m_fNowJumpPower -= fDeltaTime * m_fJumpPower * 1.f;

		}


	}



	vResultPos = vResultPos.PosVector_Matrix(matVeiwSpace.InverseMatrix());
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vResultPos);



	return S_OK;
}

HRESULT CPlayer::Set_CamPosXYZ(_float fDeltaTime)
{

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	if (m_fJumpPower && m_pCarryObject == nullptr && !m_bPause && !m_bIsDead)
	{
		if (pInstance->Get_DIKeyState(DIK_I) & DIS_Press)
		{
			if (pInstance->Get_DIKeyState(DIK_I) & DIS_Down)
				m_ComTexture->Change_TextureLayer_Wait(TEXT("lookup"),4.f);
			else if (pInstance->Get_DIKeyState(DIK_I) & DIS_Up)
				m_ComTexture->Change_TextureLayer(TEXT("Idle"));
			else {
				m_vCameraPivot.y += fDeltaTime*5.f;
				if (m_vCameraPivot.y > 6.f)
					m_vCameraPivot.y = 6.f;
			}
		}

		else if (pInstance->Get_DIKeyState(DIK_K) & DIS_Press)
		{
			if (pInstance->Get_DIKeyState(DIK_K) & DIS_Down)
				m_ComTexture->Change_TextureLayer_Wait(TEXT("lookdown"), 4.f);
			else if (pInstance->Get_DIKeyState(DIK_K) & DIS_Up)
				m_ComTexture->Change_TextureLayer(TEXT("Idle"));
			else {
				m_vCameraPivot.y -= fDeltaTime*5.f;
				if (m_vCameraPivot.y < -6.f)
					m_vCameraPivot.y = -6.f;
			}
		}
		else if (pInstance->Get_DIKeyState(DIK_J) & DIS_Press)
		{
			if (pInstance->Get_DIKeyState(DIK_J) & DIS_Down)
				m_ComTexture->Change_TextureLayer_Wait(TEXT("lookleft"), 4.f);
			else if (pInstance->Get_DIKeyState(DIK_J) & DIS_Up)
				m_ComTexture->Change_TextureLayer(TEXT("Idle"));
			else {
				m_vCameraPivot.x -= fDeltaTime * 8.f;
				if (m_vCameraPivot.x < -10.f)
					m_vCameraPivot.x = -10.f;
			}

		}
		else if (pInstance->Get_DIKeyState(DIK_L) & DIS_Press)
		{
			if (pInstance->Get_DIKeyState(DIK_L) & DIS_Down)
			m_ComTexture->Change_TextureLayer_Wait(TEXT("lookright"), 4.f);
			else if (pInstance->Get_DIKeyState(DIK_L) & DIS_Up)
				m_ComTexture->Change_TextureLayer(TEXT("Idle"));
			else {
				m_vCameraPivot.x += fDeltaTime * 8.f;
				if (m_vCameraPivot.x > 10.f)
					m_vCameraPivot.x = 10.f;
			}

		}
		else
			m_vCameraPivot = _float3(3.f, 3.f, -14.f);


	}
	else
		m_vCameraPivot = _float3(3.f, 3.f, -14.f);




	CTransform* pCamTransform = m_pCamera_Main->Get_Camera_Transform();
	_Matrix matCamWorld = pCamTransform->Get_WorldMatrix();

	_float3 vPlayerViewPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS).PosVector_Matrix(matCamWorld.InverseMatrix());


	_float3 vNewCamPos = { 0,0,0 };

	if (m_bPause && m_fDeadNPauseTime > m_fTotalPauseTime &&m_fDeadNPauseTime < m_fTotalPauseTime + 1) 
	{
		vNewCamPos.x = pInstance->Easing(TYPE_QuarticIn, 0,
			vPlayerViewPos.x + m_vCameraPivot.x, fDeltaTime, fDeltaTime * 1.8f);
		vNewCamPos.y = pInstance->Easing(TYPE_QuarticIn, 0,
			vPlayerViewPos.y + m_vCameraPivot.y, fDeltaTime, fDeltaTime * 1.8f);
		vNewCamPos.z = pInstance->Easing(TYPE_QuarticIn, 0,
			vPlayerViewPos.z + m_vCameraPivot.z, fDeltaTime, fDeltaTime * 1.8f);
	}
	else {
		vNewCamPos.x = pInstance->Easing(TYPE_QuarticIn, 0,
			vPlayerViewPos.x + m_vCameraPivot.x, fDeltaTime, fDeltaTime * 2.5f);
		vNewCamPos.y = pInstance->Easing(TYPE_QuarticIn, 0,
			vPlayerViewPos.y + m_vCameraPivot.y, fDeltaTime, fDeltaTime * 2.5f);
		vNewCamPos.z = pInstance->Easing(TYPE_QuarticIn, 0,
			vPlayerViewPos.z + m_vCameraPivot.z, fDeltaTime, fDeltaTime *2.5f);

	}
	
	vNewCamPos = vNewCamPos.PosVector_Matrix(matCamWorld);


	pCamTransform->Set_MatrixState(CTransform::STATE_POS, vNewCamPos);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;



	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//Sour => 현재 그리려고하는 그림의 색
	//Dest => 직전까지 화면에 그려진 색

	
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	//m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(120, 255, 255, 255));
	//
	//
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	if (m_bTextureReverse)
		m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 130);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_bIsShdow)
	{
		m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_XRGB(0, 0, 0));
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);

	}

	return S_OK;
}

HRESULT CPlayer::Release_RenderState()
{
	// _float3 Temp = m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1, 0);
	//GetSingle(CGameInstance)->Render_World_Font(L"0ere'sy\nForgive me, girl", Temp,_float2(0.3f,0.45f),_float3(38,240,120));


	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	if (m_bTextureReverse)
		m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	if (m_bIsShdow)
	{
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	}

	return S_OK;
}


CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CPlayer* pInstance = new CPlayer(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CPlayer_ProtoType");
		Safe_Release(pInstance);

	}
	

	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer* pInstance = new CPlayer((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CPlayer_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	if (m_bIsClone)
	{
		_uint IsSuccess = 0;
		GetSingle(CLoginMgr)->Save_PlayerData(this, &IsSuccess);

		if (!IsSuccess)
			MSGBOX("Fail To Save");

	}


	Safe_Release(m_pCarryObject);
	Safe_Release(m_pCarryObjectTransform);
	Safe_Release(m_pCamera_Main);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_pCollisionViewCom);
	Safe_Release(m_ComInventory);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
