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

	m_pCamera_Main = ((CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGESELECT, TAG_LAY(Layer_Camera_Main))));
	if (m_pCamera_Main == nullptr)
		return E_FAIL;
	Safe_AddRef(m_pCamera_Main);
	return S_OK;
}


_int CPlayer::Update(_float fDeltaTime)
{

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FLEXIBLE, this);

	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;


	m_fFrame += 6.0f * fDeltaTime;

	if (m_fFrame >= 6.0f)
		m_fFrame = 0.f;

	CGameInstance* pInstance = GetSingle(CGameInstance);

	if (pInstance->Get_DIKeyState(DIK_UP) & DIS_Press)
	{
		if (m_BackWardObject != nullptr)
		{
			m_ComTransform->MovetoTarget(m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1.f, 0), fDeltaTime);
			m_fNowJumpPower = 0;

			m_bIsCliming = true;
		}

		
	}

	if (pInstance->Get_DIKeyState(DIK_DOWN) & DIS_Press)
	{
		if (m_bIsCliming) 
		{
			if (m_BackWardObject != nullptr)
			{
				m_ComTransform->MovetoTarget(m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1.f, 0), -fDeltaTime);
				m_fNowJumpPower = 0;

				m_bIsCliming = true;
			}

		}
		else 
		{
			if (m_FootHoldObject != nullptr)
			{
				m_ComTransform->MovetoTarget(m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1.f, 0), -fDeltaTime);
				m_fNowJumpPower = 0;

				m_bIsCliming = true;
			}

		}

	}

	if (m_bCanMoveLeft && pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Press)
	{


		m_ComTransform->Move_Left(fDeltaTime);
	}

	if (m_bCanMoveRight && pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Press)
	{


		m_ComTransform->Move_Right(fDeltaTime);
	}


	if (m_bIsJumped < 2 && pInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
	{
		m_fNowJumpPower = m_fJumpPower;
		m_bIsJumped++;
		m_bIsCliming = false;
		m_ComTransform->MovetoTarget(m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1.f, 0), fDeltaTime);
	}

	
	if (FAILED(Find_FootHold_Object())) {
		return E_FAIL;
	}
	



	return _int();
}

_int CPlayer::LateUpdate(_float fDeltaTime)
{


	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;


	if (FAILED(Set_PosOnFootHoldObject(fDeltaTime)))
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
	
	if (FAILED(m_ComTexture->Bind_Texture((_uint)m_fFrame)))
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

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube), TAG_COM(Com_CollisionBuffer), (CComponent**)&m_ComColiisionBuffer)))
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

HRESULT CPlayer::Find_FootHold_Object()
{
	CGameInstance* pGameInstance = GetSingle(CGameInstance);

	//뷰스페이스 변환 행렬
	_Matrix matVeiwSpace;
	m_pGraphicDevice->GetTransform(D3DTS_VIEW, &matVeiwSpace);

	_float3 vCamPos;
	memcpy(&vCamPos, &(matVeiwSpace.InverseMatrix().m[3][0]), sizeof(_float3));

	//카메라 바라보도록 설정

	_float3 vPlayerPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

	_float3 vNewLook = (vPlayerPos - vCamPos).Get_Nomalize();

	m_ComTransform->LookAt(vPlayerPos + vNewLook);


	if (m_pCamera_Main->Get_bIsTuring())
		return S_OK;

	_float3 vPlayerViewPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS).PosVector_Matrix(matVeiwSpace);

	 list<CGameObject*>* pTerrainLayer= pGameInstance->Get_ObjectList_from_Layer(SCENEID::SCENE_STAGESELECT, TAG_LAY(Layer_Terrain));

	 if (pTerrainLayer == nullptr)
		 return E_FAIL;


	 CGameObject* pYNearObject = nullptr;
	 CGameObject* pZNearObject = nullptr;
	 
	 _float		fNearZLenth = -(_float)0x0fffffff;
	 _float		fFootZLenth	= (_float)0x0fffffff;
	 _float		fFarZLenth = (_float)0x0fffffff;


	 auto ObjectListIter = pTerrainLayer->begin();

	 m_bIsShdow = false;
	 m_bCanMoveLeft = true;
	 m_bCanMoveRight = true;

	 for (; ObjectListIter != pTerrainLayer->end();)
	 {
		 _float3 vTerrainObjectViewPos = ((CTransform*)((*ObjectListIter)->Find_Components(TAG_COM(Com_Transform))))
			 ->Get_MatrixState(CTransform::STATE_POS).PosVector_Matrix(matVeiwSpace);

		 if (vTerrainObjectViewPos.y <= vPlayerViewPos.y + 0.5f && vTerrainObjectViewPos.y > vPlayerViewPos.y - 0.5f)
		 {
			 if (vTerrainObjectViewPos.x + 0.5f >= vPlayerViewPos.x && vTerrainObjectViewPos.x - 0.5f < vPlayerViewPos.x)
			 {
				 if (vTerrainObjectViewPos.z < vPlayerViewPos.z && vTerrainObjectViewPos.z > fNearZLenth)
				 {
					 fNearZLenth = vTerrainObjectViewPos.z;
					 m_bIsShdow = true;
				 }
			 }
			 else if (vTerrainObjectViewPos.z > vPlayerViewPos.z - 0.25f && vTerrainObjectViewPos.z < vPlayerViewPos.z + 0.25f) 
			 {
				 if (vTerrainObjectViewPos.x >= vPlayerViewPos.x + 0.6f  && vTerrainObjectViewPos.x  < vPlayerViewPos.x + 1.1f)
				 {
					 m_bCanMoveRight = false;
				 }
				 else if (vTerrainObjectViewPos.x <= vPlayerViewPos.x - 0.6f  && vTerrainObjectViewPos.x  > vPlayerViewPos.x - 1.1f)
				 {
					 m_bCanMoveLeft = false;
				 }
			 }
		 }
		 ObjectListIter++;
	 }
	 
	 ObjectListIter = pTerrainLayer->begin();

	 for (; ObjectListIter != pTerrainLayer->end();)
	 {
		 _float3 vTerrainObjectViewPos = ((CTransform*)((*ObjectListIter)->Find_Components(TAG_COM(Com_Transform))))
											->Get_MatrixState(CTransform::STATE_POS).PosVector_Matrix(matVeiwSpace);

		 if (vTerrainObjectViewPos.x + 0.5f >= vPlayerViewPos.x && vTerrainObjectViewPos.x - 0.5f < vPlayerViewPos.x)
		 {
			 //바닥 오브젝트 파악
			 if (vTerrainObjectViewPos.y <= vPlayerViewPos.y - 0.75f && vTerrainObjectViewPos.y > vPlayerViewPos.y - 1.4f)
			 {
				 if (vTerrainObjectViewPos.z < fFootZLenth && vTerrainObjectViewPos.z > fNearZLenth)
				 {
					 pYNearObject = (*ObjectListIter);
					 fFootZLenth = vTerrainObjectViewPos.z;
				 }
			 }
			 if (vTerrainObjectViewPos.y <= vPlayerViewPos.y + 0.5f && vTerrainObjectViewPos.y > vPlayerViewPos.y - 0.5f)
			 {
				 //바로 뒤 오브젝트 파악(등반하기 위해서)
				 if (vTerrainObjectViewPos.z >= vPlayerViewPos. z  && vTerrainObjectViewPos.z < fFarZLenth)
				 {
					 pZNearObject = (*ObjectListIter);
					 fFarZLenth = vTerrainObjectViewPos.z;
				 }


			 }
		 }
		 
		 ObjectListIter++;
	 }



	 if (m_FootHoldObject != nullptr)
		 m_ReturnFootHold = m_FootHoldObject;

	 Safe_Release(m_FootHoldObject);
	 m_FootHoldObject = pYNearObject;
	 Safe_AddRef(m_FootHoldObject);

	 Safe_Release(m_BackWardObject);
	 m_BackWardObject = pZNearObject;
	 Safe_AddRef(m_BackWardObject);
	 
	 if (m_BackWardObject == nullptr)
		 m_bIsCliming = false;




	return S_OK;
}

HRESULT CPlayer::Set_PosOnFootHoldObject(_float fDeltaTime)
{


	//CTransform* pCameraTransform = m_pCamera_Main->Get_Camera_Transform();
	_Matrix matVeiwSpace;
	m_pGraphicDevice->GetTransform(D3DTS_VIEW, &matVeiwSpace);

	_float3 vResultPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);


	if (!m_bIsCliming)
	{
		/////////중력 적용
		_float Time = 1 - (m_fNowJumpPower / m_fJumpPower);
		if (m_fNowJumpPower != 0) {

			m_fNowJumpPower -= fDeltaTime * m_fJumpPower * 2.f;
			vResultPos.y = vResultPos.y + (m_fNowJumpPower - Time*Time * m_fJumpPower)*fDeltaTime;
		}

		vResultPos = vResultPos.PosVector_Matrix(matVeiwSpace);


		if (m_FootHoldObject != nullptr)
		{
			_float3		vFootHoldObjectViewPos = (((CTransform*)(m_FootHoldObject->Find_Components(TAG_COM(Com_Transform))))
				->Get_MatrixState(CTransform::STATE_POS)).PosVector_Matrix(matVeiwSpace);
			vResultPos.z = vFootHoldObjectViewPos.z;

			if (vResultPos.y < vFootHoldObjectViewPos.y + 1.0f && m_fNowJumpPower < 0) //지형보다 플레이어가 위에 있다면
			{
				vResultPos.y = vFootHoldObjectViewPos.y + 1.f;
				m_fNowJumpPower = 0;
				m_bIsJumped = 0;

			}
		}
		else if (Time > 3.f && m_ReturnFootHold !=nullptr)
		{
			_float3		vFootHoldObjectViewPos = (((CTransform*)(m_ReturnFootHold->Find_Components(TAG_COM(Com_Transform))))
				->Get_MatrixState(CTransform::STATE_POS)).PosVector_Matrix(matVeiwSpace);

			vResultPos = vFootHoldObjectViewPos;
			vResultPos.y += 1.f;

			m_fNowJumpPower = 0;
			m_bIsJumped = 0;
			//m_ReturnFootHold
		}
		else {
			m_fNowJumpPower -= fDeltaTime * m_fJumpPower;
		}
	}
	else {
		vResultPos = vResultPos.PosVector_Matrix(matVeiwSpace);

		if (m_BackWardObject != nullptr) {

			_float3		vBackWardObjectViewPos = (((CTransform*)(m_BackWardObject->Find_Components(TAG_COM(Com_Transform))))
				->Get_MatrixState(CTransform::STATE_POS)).PosVector_Matrix(matVeiwSpace);
			vResultPos.z = vBackWardObjectViewPos.z - 1.1f;
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

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	


	// 일반적인 텍스쳐 찍기로 되돌아 가려면..
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);




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
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


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
	Safe_Release(m_ComColiisionBuffer);
	Safe_Release(m_ComInventory);
	Safe_Release(m_BackWardObject);
	Safe_Release(m_FootHoldObject);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
