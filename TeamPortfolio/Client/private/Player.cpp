#include "stdafx.h"
#include "..\public\Player.h"
#include "Camera_Main.h"





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

	m_fNowJumpPower = 0;
	m_fJumpPower = 10.f;
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(0,1.f,0));
	//m_ComTransform->Scaled(_float3(2.f, 2.f, 2.f));

	
	m_pCamera_Main = ((CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main))));
	
	if (m_pCamera_Main == nullptr)
		return E_FAIL;
	Safe_AddRef(m_pCamera_Main);

	m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("wakeup"),TEXT("Idle"),4.f);

	return S_OK;
}


_int CPlayer::Update(_float fDeltaTime)
{

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FLEXIBLE, this);

	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	if (m_bIsDead) {
		m_fDeadNPauseTime += fDeltaTime;


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
	

				m_ComTexture->Change_TextureLayer_Wait(TEXT("jump_up"));
			m_bPause = false;
		}
	}
	else {

		if (FAILED(Input_Keyboard(fDeltaTime)))
			return E_FAIL;

		if (FAILED(Animation_Change(fDeltaTime)))
			return E_FAIL;

		if (FAILED(Find_FootHold_Object(fDeltaTime)))
			return E_FAIL;


		//if (FAILED(Set_PosOnFootHoldObject(fDeltaTime)))
		//	return E_FAIL;


	}

	return _int();
}

_int CPlayer::LateUpdate(_float fDeltaTime)
{


	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;

	if (!m_bIsDead && !m_bPause)
	{
		if (FAILED(Set_PosOnFootHoldObject(fDeltaTime)))
			return E_FAIL;
	}
	if (FAILED(Set_CamPosXYZ(fDeltaTime)))
		return E_FAIL;



	//렌더링 그룹에 넣어주는 역활
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_AFTEROBJ, this)))
		return E_FAIL;

	return _int();
}

_int CPlayer::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL; 
	
	 
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;
	
	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame)))
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


	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_FixCube")))
	{
		if (m_pCarryObject == nullptr && GetSingle(CGameInstance)->Get_DIKeyState(DIK_LSHIFT) & DIS_Down)
		{
			m_pCarryObject = pDestObjects;
			m_pCarryObjectTransform = (CTransform*)(m_pCarryObject->Get_Component(TAG_COM(Com_Transform)));

			m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("carryup"),TEXT("carryIdle"));

			m_bIsCliming = false;
			m_bIsRunning = false;
			m_ComTransform->Set_MoveSpeed(1.f);
			m_fJumpPower *= 0.5f;

			Safe_AddRef(m_pCarryObject);
			Safe_AddRef(m_pCarryObjectTransform);
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
		}
		if (m_fDeadNPauseTime < 4.0f)
			m_pCollisionCom->Collision_Suck_In(m_ComTransform, fCollision_Distance, fDeltaTime);
		
	}
	else if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_OrbitButton"))) 
	{
		if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_LSHIFT) & DIS_Down)
			m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("buttonclick"), TEXT("Idle"), 4.f);

	}
	else if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TAG_LAY(Layer_Terrain)))
	{
		m_pCollisionCom->Collision_Pushed(m_ComTransform, fCollision_Distance, fDeltaTime);
	}
	return _int();
}

void CPlayer::Set_PlayerPause(_float TotalPauseTime, const _tchar* TagAnim, _float fFrameTime)
{
	m_bPause = true;
	m_fDeadNPauseTime = 0;
	m_fTotalPauseTime = TotalPauseTime;
	m_szReturnAnimTag = m_ComTexture->Get_NowTextureTag();
	m_ComTexture->Change_TextureLayer_Wait(TagAnim, fFrameTime);
}

HRESULT CPlayer::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fMovePerSec = 3.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	//TransformDesc.vPivot = _float3(0, 0.1f, 0);


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


		Safe_Release(m_pCarryObject);
		Safe_Release(m_pCarryObjectTransform);

		m_pCarryObject = nullptr;

		m_ComTransform->Set_MoveSpeed(2.5f);
		m_fJumpPower *= 2.f;


		m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("carrydown"), TEXT("Idle"),12.f);

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
						m_ComTexture->Change_TextureLayer_ReturnToWait(TEXT("pull_down"), TEXT("climing_back"), 12.f, 12.f);

					}


				}
			}
		}
	}

	//좌우 이동
	if (pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Press)
	{
		m_ComTransform->Move_Right(fDeltaTime);
	}

	if (pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Press)
	{
		m_ComTransform->Move_Right(fDeltaTime);
	}

	//점프
	if (m_bIsJumped < 2 && pInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
	{
		if (m_pCarryObject)
			m_ComTexture->Change_TextureLayer_Wait(TEXT("carryjumpup"), 8.f);
		else
			m_ComTexture->Change_TextureLayer_Wait(TEXT("jump_up"), 8.f);

		m_fNowJumpPower = m_fJumpPower;
		m_bIsJumped++;
		m_bIsCliming = false;
		m_ComTransform->MovetoDir(_float3(0, 1.f, 0), fDeltaTime);
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
					}
				}
			}
			else
			{
				//if (lstrcmp(m_ComTexture->Get_NowTextureTag(), TEXT("jump_down"))) 
				if (!(m_ComTexture->Get_IsReturnTexture()))
				{

					if (pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_DoubleDown || pInstance->Get_DIKeyState(DIK_LEFT) & DIS_DoubleDown)
					{
						m_bIsRunning = true;
						m_ComTransform->Set_MoveSpeed(4.f);
					}
					else if(pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Down || pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Down)
					{
						m_bIsRunning = false;
						m_ComTransform->Set_MoveSpeed(2.5f);
					}

					if (m_bIsRunning)
						m_ComTexture->Change_TextureLayer(TEXT("run"));
					else
						m_ComTexture->Change_TextureLayer(TEXT("walk"));

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

	return S_OK;
}

HRESULT CPlayer::Find_FootHold_Object(_float fDeltaTime)
{

	CGameInstance* pGameInstance = GetSingle(CGameInstance);

	//뷰스페이스 행렬
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


	if (m_pCamera_Main->Get_bIsTuring())
		return S_OK;

	_float3 vPlayerViewPos = vPlayerPos.PosVector_Matrix(matVeiwSpace);

	_float Time = 1 - (m_fNowJumpPower / m_fJumpPower);
	_float fGravity = 0;

	if (Time < 0)
		fGravity = (m_fNowJumpPower - Time * Time * m_fJumpPower) * fDeltaTime;
	
	 list<CGameObject*>* pTerrainLayer= pGameInstance->Get_ObjectList_from_Layer(m_eNowSceneNum, TAG_LAY(Layer_Terrain));

	 if (pTerrainLayer == nullptr)
		 return E_FAIL;

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
				 if (vTerrainObjectViewPos.z < vPlayerViewPos.z - 0.25f && vTerrainObjectViewPos.z > fPlayerFrontZ)
				 {
					 fPlayerFrontZ = vTerrainObjectViewPos.z;
					 m_bIsShdow = true;
				 }
				 if(vTerrainObjectViewPos.z > vPlayerViewPos.z + 0.25f && vTerrainObjectViewPos.z < fPlayerBackZ)
					 fPlayerBackZ = vTerrainObjectViewPos.z;
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
		 if (vTerrainWorldPos.y <= vPlayerPos.y - 0.5f && vTerrainWorldPos.y >= vPlayerPos.y - 1.5f + fGravity)
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
			m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 0.35f, 0));
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

				if (m_pCarryObject)
					m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("carryjumpdown"), TEXT("carryIdle"), 8.f);
				else
					m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("jump_down"), TEXT("Idle"), 8.f);

				vResultPos.y = m_vDownstairsNear.y + 0.9f;
				m_fNowJumpPower = 0;
				m_bIsJumped = 0;

			}


		}
		else if (Time > 3.f && m_vReturnStair != NOT_EXIST_BLOCK)
		{
			//피격 이미지 넣어주기/////////////////////////
			m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("hurt"), TEXT("Idle"),8.f);
			//m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("jump_down"), TEXT("Idle"), 8.f);
			vResultPos = m_vReturnStair.PosVector_Matrix(matVeiwSpace);
			vResultPos.y += 1.f;
			m_fNowJumpPower = 0;
			m_bIsJumped = 0;



			m_pCamera_Main->CameraEffect(CCamera_Main::CAM_EFT_HIT,fDeltaTime);
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
				vResultPos.y = m_vDownstairsNear.y + 1.f;
				m_fNowJumpPower = 0;
				m_bIsJumped = 0;
			}
		}
		else if (m_vClimingBlock != NOT_EXIST_BLOCK)
		{
			vResultPos.z = m_vClimingBlock.z - 1.0f;
		}
		else 
		{
			m_bIsCliming = false;
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
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

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



	Safe_Release(m_pCarryObject);
	Safe_Release(m_pCarryObjectTransform);
	Safe_Release(m_pCamera_Main);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComInventory);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
