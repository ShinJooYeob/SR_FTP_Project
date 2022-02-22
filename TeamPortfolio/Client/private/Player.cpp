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

	m_fNowJumpPower = m_fJumpPower = 10.f;
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(0,1.f,0));
	m_ComTransform->Scaled(_float3(0.7f, 0.9f, 1.f));

	m_pCamera_Main = ((CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGESELECT, TAG_LAY(Layer_Camera_Main))));
	
	if (m_pCamera_Main == nullptr)
		return E_FAIL;
	Safe_AddRef(m_pCamera_Main);

	m_ComTexture->Change_TextureLayer(TEXT("Idle"));

	return S_OK;
}


_int CPlayer::Update(_float fDeltaTime)
{

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FLEXIBLE, this);

	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	//if (FAILED(Animation_Change(fDeltaTime)))
	//	return E_FAIL;

	if (FAILED(Input_Keyboard(fDeltaTime)))
		return E_FAIL;

	if (FAILED(Find_FootHold_Object(fDeltaTime))) 
		return E_FAIL;

	if (FAILED(Set_PosOnFootHoldObject(fDeltaTime)))
		return E_FAIL;



	return _int();
}

_int CPlayer::LateUpdate(_float fDeltaTime)
{


	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;



	//렌더링 그룹에 넣어주는 역활
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
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

	return _int();
}

HRESULT CPlayer::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fMovePerSec = 3.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STAGESELECT, TAG_CP(Prototype_Texture_Player), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
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


	//등반
	if (pInstance->Get_DIKeyState(DIK_UP) & DIS_Press)
	{
		if (m_vClimingBlock != NOT_EXIST_BLOCK)
		{
			m_ComTexture->Change_TextureLayer_Wait(TEXT("climing_back"),12.f);
			m_ComTransform->MovetoDir(_float3(0, 1.f, 0), fDeltaTime);
			m_fNowJumpPower = 0;

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
					_float3 vPlayerPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
					vPlayerPos.y = m_vDownstairsNear.y + 0.5f;
					m_bIsCliming = false;
				}
				else
				{
					m_ComTexture->Change_TextureLayer_Wait(TEXT("climing_back"), 12.f);
					m_ComTransform->MovetoDir(_float3(0, -1.f, 0), fDeltaTime);
					m_bIsCliming = true;
				}
			}
		}
		else
		{
			if (m_vDownstairsNear != NOT_EXIST_BLOCK)
			{
				m_ComTransform->MovetoDir(_float3(0, -1.f, 0), fDeltaTime);

				_Matrix matVeiwSpace;
				m_pGraphicDevice->GetTransform(D3DTS_VIEW, &matVeiwSpace);

				_float3 vPlayerPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS).PosVector_Matrix(matVeiwSpace);

				vPlayerPos.z = m_vDownstairsNear.z - 1.f;

				vPlayerPos = vPlayerPos.PosVector_Matrix(matVeiwSpace.InverseMatrix());
				m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vPlayerPos);

				m_fNowJumpPower = 0;
				m_bIsCliming = true;
			}
		}
	}

	//좌우 이동
	if (m_bCanMoveLeft && pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Press)
	{
		if (m_bIsCliming) {
			m_bTextureReverse = true;
			m_ComTexture->Change_TextureLayer_Wait(TEXT("climing_back"), 12.f);;
			m_ComTransform->Move_Right(fDeltaTime);
		}
		else {

			if (pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Down)
			{
				m_bTextureReverse = true;
				m_ComTexture->Change_TextureLayer(TEXT("walk"));
			}
			else if (pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Up)
				m_ComTexture->Change_TextureLayer(TEXT("Idle"));

			if (m_bTextureReverse)
				m_ComTransform->Move_Right(fDeltaTime);
			else
				m_ComTransform->Move_Left(fDeltaTime);
		}
	}

	if (m_bCanMoveRight && pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Press)
	{

		if (m_bIsCliming) {
			m_bTextureReverse = false;
			m_ComTexture->Change_TextureLayer_Wait(TEXT("climing_back"), 12.f);;
		}
		else {
			if (pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Down) {
				m_bTextureReverse = false;
				m_ComTexture->Change_TextureLayer(TEXT("walk"));

			}
			else if (pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Up)
				m_ComTexture->Change_TextureLayer(TEXT("Idle"));

		}
		m_ComTransform->Move_Right(fDeltaTime);
	}

	//점프
	if (m_bIsJumped < 2 && pInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
	{
		m_ComTexture->Change_TextureLayer_Wait(TEXT("jump_up"),8.f);
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

	if (m_bIsJumped > 0 && (pInstance->Get_DIKeyState(DIK_UP) & DIS_Down)) 
	{

		if (m_vClimingBlock != NOT_EXIST_BLOCK)
		{
			//기어 올라가는 이미지 넣어주기
			m_ComTexture->Change_TextureLayer_Wait(TEXT("Idle"));

		}
	}

	if(!(pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Press) && !(pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Press))
		m_ComTexture->Change_TextureLayer(TEXT("Idle"));

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



	//_float3 NewLook = ((*(_float3*)(&matVeiwSpace.InverseMatrix().m[3][0])) - vPlayerPos).Get_Nomalize();

	//if (m_bTextureReverse)
	//	m_ComTransform->LookAt(vPlayerPos + NewLook);
	//else
	//	m_ComTransform->LookAt(vPlayerPos - NewLook);




	if (m_pCamera_Main->Get_bIsTuring())
		return S_OK;

	_float3 vPlayerViewPos = vPlayerPos.PosVector_Matrix(matVeiwSpace);

	_float Time = 1 - (m_fNowJumpPower / m_fJumpPower);
	_float fGravity = 0;

	if (Time < 0)
		fGravity = (m_fNowJumpPower - Time * Time * m_fJumpPower) * fDeltaTime;
	
	 list<CGameObject*>* pTerrainLayer= pGameInstance->Get_ObjectList_from_Layer(SCENEID::SCENE_STAGESELECT, TAG_LAY(Layer_Terrain));

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
	 m_bCanMoveLeft = true;
	 m_bCanMoveRight = true;
	// m_bCanMoveUp = false;

	 _float3 vTerrainWorldPos;
	 _float3 vTerrainObjectViewPos;

	 for (; ObjectListIter != pTerrainLayer->end();)
	 {
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

			 if (vTerrainObjectViewPos.z > vPlayerViewPos.z - 0.25f && vTerrainObjectViewPos.z < vPlayerViewPos.z + 0.25f)
			 {
				 if (vTerrainObjectViewPos.x > vPlayerViewPos.x + 0.55f  && vTerrainObjectViewPos.x < vPlayerViewPos.x + 1.1f)
				 {
					 m_bCanMoveRight = false;
				 }
				 else if (vTerrainObjectViewPos.x < vPlayerViewPos.x - 0.55f  && vTerrainObjectViewPos.x  > vPlayerViewPos.x - 1.1f)
				 {
					 m_bCanMoveLeft = false;
				 }
			 }
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
		 if(m_vClimingBlock == NOT_EXIST_BLOCK && (*SelectedListiter).y < vPlayerViewPos.y + 0.5f && (*SelectedListiter).y >= vPlayerViewPos.y - 0.5f)
		 {
			 //같은 층에
			 if ((*SelectedListiter).z > fPlayerFrontZ)
			 {
				 m_vClimingBlock = (*SelectedListiter);
				// m_bCanMoveUp = true;
			 }
		 }
		 SelectedListiter++;
	 }



	 if (m_vDownstairsNear != NOT_EXIST_BLOCK)
		 m_vReturnStair = m_vDownstairsNear;




	return S_OK;
}

HRESULT CPlayer::Set_PosOnFootHoldObject(_float fDeltaTime)
{

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
				//m_ComTexture->Change_TextureLayer_Wait(TEXT("jump_down"));
				m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("jump_down"), TEXT("Idle"), 8.f);
				vResultPos.y = m_vDownstairsNear.y + 1.f;
				m_fNowJumpPower = 0;
				m_bIsJumped = 0;

			}


		}
		else if (Time > 3.f && m_vReturnStair != _float3(0, 0, 0))
		{
			//피격 이미지 넣어주기/////////////////////////
			//m_ComTexture->Change_TextureLayer_ReturnTo(TEXT("jump_down"), TEXT("Idle"), 8.f);
			m_pCamera_Main->CameraEffect(CCamera_Main::CAM_EFT_HIT,fDeltaTime);
			vResultPos = m_vReturnStair;
			vResultPos.y += 1.f;

			m_fNowJumpPower = 0;
			m_bIsJumped = 0;
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
	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

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

	Safe_Release(m_pCamera_Main);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComInventory);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
