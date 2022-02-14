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

	m_fJumpPower = 10.f;

	return S_OK;
}


_int CPlayer::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	m_fFrame += 12.0f * fDeltaTime;

	if (m_fFrame >= 12.0f)
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

	if (pInstance->Get_DIKeyState(DIK_LEFT) & DIS_Press)
	{
		m_ComTransform->Move_Left(fDeltaTime);
	}

	if (pInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Press)
	{
		m_ComTransform->Move_Right(fDeltaTime);
	}


	//static _bool IsRTurning = false;
	//static _bool IsLTurning = false;
	//static _float fPassedTime = false;
	//static _float fStarPoint = 0;
	//static _float fTargetPoint = 0;

	//if (pInstance->Get_DIKeyState(DIK_E) & DIS_Down)
	//{
	//	IsRTurning = true;
	//	fPassedTime = 0;
	//	fTargetPoint = fStarPoint+90;


	//}
	//else if(IsRTurning)
	//{

	//	fPassedTime += fDeltaTime;

	//	_float fNowPoint;
	//	fNowPoint = pInstance->TargetLinear(fStarPoint, fTargetPoint,  fPassedTime);

	//	if (fNowPoint >= fTargetPoint)
	//	{
	//		IsRTurning = false;
	//		fNowPoint = fTargetPoint;
	//	}

	//	m_ComTransform->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(fNowPoint));

	//}

	//if (pInstance->Get_DIKeyState(DIK_Q) & DIS_Down)
	//{

	//	IsLTurning = true;
	//	fPassedTime = 0;
	//	fTargetPoint = fStarPoint - 90;

	//}
	//else if(IsLTurning)
	//{
	//	fPassedTime += fDeltaTime;

	//	_float fNowPoint;
	//	fNowPoint = pInstance->TargetQuadIn(fStarPoint, fTargetPoint, fPassedTime);

	//	if (fNowPoint <= fTargetPoint)
	//	{
	//		IsLTurning = false;
	//		fNowPoint = fTargetPoint;
	//	}

	//	m_ComTransform->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(fNowPoint));

	//}
		 
	if (pInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
	{
		m_fNowJumpPower = m_fJumpPower;
		m_bIsJumped = true;

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
	

	//CCamera_Main* pCamera = (CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGESELECT, TEXT("Layer_Camera_Main")));


	//if (FAILED(m_ComTransform->Bind_WorldMatrix_Look_Camera(pCamera->Get_Camera_Position())))
	//	return E_FAIL;
	 
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

HRESULT CPlayer::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	TransformDesc.vPivot = _float3(0, -0.5f, 0);
	


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
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Inventory"), TEXT("Com_Inventory"), (CComponent**)&m_ComInventory, &iMaxSkillNum)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::Find_FootHold_Object()
{
	CGameInstance* pGameInstance = GetSingle(CGameInstance);

	CTransform* pCameraTransform = ((CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGESELECT, TAG_LAY(Layer_Camera_Main))))->Get_Camera_Transform();

	if (pCameraTransform == nullptr)
		return E_FAIL;

	//뷰스페이스 변환 행렬
	_Matrix matVeiwSpace = pCameraTransform->Get_InverseWorldMatrix();
	_float3 vPlayerViewPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS).PosVector_Matrix(matVeiwSpace);

	 list<CGameObject*>* pTerrainLayer= pGameInstance->Get_ObjectList_from_Layer(SCENEID::SCENE_STAGESELECT, TAG_LAY(Layer_Terrain));

	 if (pTerrainLayer == nullptr)
		 return E_FAIL;


	 CGameObject* pYNearObject = nullptr;
	 CGameObject* pZNearObject = nullptr;
	 
	 _float		fNearYLenth = (_float)0x0fffffff;
	 _float		fNearZLenth = -(_float)0x0fffffff;
	  
	 auto ObjectListIter = pTerrainLayer->begin();

	 for (; ObjectListIter != pTerrainLayer->end();)
	 {
		 _float3 vTerrainObjectViewPos = ((CTransform*)((*ObjectListIter)->Find_Components(TAG_COM(Com_Transform))))
											->Get_MatrixState(CTransform::STATE_POS).PosVector_Matrix(matVeiwSpace);

		 if (vTerrainObjectViewPos.x + 0.5f >= vPlayerViewPos.x && vTerrainObjectViewPos.x - 0.5f < vPlayerViewPos.x)
		 {
			 //바닥 오브젝트 파악
			 if (vTerrainObjectViewPos.y <= vPlayerViewPos.y + 0.1f && vTerrainObjectViewPos.y > vPlayerViewPos.y - 1.f)
			 {
				 if (vTerrainObjectViewPos.z < fNearYLenth)
				 {
					 pYNearObject = (*ObjectListIter);
					 fNearYLenth = vTerrainObjectViewPos.z;
				 }
			 }
			 //바로 뒤 오브젝트 파악(등반하기 위해서)
			 if (vTerrainObjectViewPos.y <= vPlayerViewPos.y + 1.f && vTerrainObjectViewPos.y > vPlayerViewPos.y)
			 {
				 if (vTerrainObjectViewPos.z >= vPlayerViewPos. z  && vTerrainObjectViewPos.z > fNearZLenth)
				 {
					 pZNearObject = (*ObjectListIter);
					 fNearZLenth = vTerrainObjectViewPos.z;
				 }
			 }
		 }
		 
		 ObjectListIter++;
	 }


	 Safe_Release(m_FootHoldObject);
	 m_FootHoldObject = pYNearObject;
	 Safe_AddRef(m_FootHoldObject);

	 Safe_Release(m_BackWardObject);
	 m_BackWardObject = pZNearObject;
	 Safe_AddRef(m_BackWardObject);
	 
	 if (m_BackWardObject == nullptr)
		 m_bIsCliming = false;

	 //카메라 바라보도록 설정

	 _float3 vPlayerPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
	 _float3 vCamPos = pCameraTransform->Get_MatrixState(CTransform::STATE_POS);

	 _float3 vNewLook = (vPlayerPos - vCamPos).Get_Nomalize();

	 m_ComTransform->LookAt(vPlayerPos + vNewLook);


	return S_OK;
}

HRESULT CPlayer::Set_PosOnFootHoldObject(_float fDeltaTime)
{
	CGameInstance* pGameInstance = GetSingle(CGameInstance);

	CTransform* pCameraTransform = ((CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STAGESELECT, TAG_LAY(Layer_Camera_Main))))->Get_Camera_Transform();

	if (pCameraTransform == nullptr)
		return E_FAIL;

	_float3 vResultPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);


	if (!m_bIsCliming)
	{
		/////////중력 적용
		m_fNowJumpPower -= fDeltaTime * m_fJumpPower *2.f;
		_float Time = 1 - (m_fNowJumpPower / m_fJumpPower);
		vResultPos.y = vResultPos.y + (m_fNowJumpPower - Time*Time * m_fJumpPower)*fDeltaTime;

		vResultPos = vResultPos.PosVector_Matrix(pCameraTransform->Get_InverseWorldMatrix());

		if (m_FootHoldObject != nullptr)
		{
			_float3		vFootHoldObjectViewPos = (((CTransform*)(m_FootHoldObject->Find_Components(TAG_COM(Com_Transform))))->Get_MatrixState(CTransform::STATE_POS)).PosVector_Matrix(pCameraTransform->Get_InverseWorldMatrix());

			if (vResultPos.y <= vFootHoldObjectViewPos.y) //지형보다 플레이어가 위에 있다면
			{
				vResultPos.y = vFootHoldObjectViewPos.y;
				vResultPos.z = vFootHoldObjectViewPos.z;

				m_fNowJumpPower = 0;
				m_bIsJumped = false;

			}
		}
	}
	else {
		vResultPos = vResultPos.PosVector_Matrix(pCameraTransform->Get_InverseWorldMatrix());
	}




	vResultPos = vResultPos.PosVector_Matrix(pCameraTransform->Get_InverseWorldMatrix().InverseMatrix());
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vResultPos);



	return S_OK;
}

HRESULT CPlayer::Set_PosOnTerrain(_float fDeltaTime)
{

	CGameInstance* pGameInstance = GetSingle(CGameInstance);

	
	CGameObject* pTerrain= pGameInstance->Get_GameObject_By_LayerIndex(SCENEID::SCENE_STAGESELECT, TEXT("Layer_Terrain"));

	if (pTerrain == nullptr)
		return E_FAIL;

	CVIBuffer_Terrain* pTerrainBuffer =(CVIBuffer_Terrain*)(pTerrain->Find_Components(TEXT("Com_VIBuffer")));
	CTransform*		pTerrainTransform = (CTransform*)(pTerrain->Find_Components(TEXT("Com_Transform")));

	

	_float3 vResultPos;

	_float3 vPlayerPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

	if (!FAILED(pTerrainBuffer->PointInTerrain(&vResultPos, vPlayerPos, pTerrainTransform->Get_InverseWorldMatrix())))
	{
		if (vPlayerPos.y > vResultPos.y) //지형보다 플레이어가 위에 있다면
		{

			m_fNowJumpPower -= fDeltaTime * m_fJumpPower *2.f;

			//경과 시간
			//1 - (m_fNowJumpPower / m_fJumpPower);

			_float Time = 1 - (m_fNowJumpPower / m_fJumpPower);
			
			_float Temp = vPlayerPos.y + (m_fNowJumpPower - Time*Time * m_fJumpPower)*fDeltaTime;

			vResultPos.y = (Temp > vResultPos.y)? Temp : vResultPos.y;
		}
		else //지형에 닿았다면
		{
			m_fNowJumpPower = 0;
			m_bIsJumped = false;
		}
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vResultPos);

	}
	else //지형 밖으로 넘어갔을 경우
	{
		m_fNowJumpPower = 0;
		m_bIsJumped = true;
	}



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

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	return S_OK;
}

HRESULT CPlayer::Release_RenderState()
{
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

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


	Safe_Release(m_FootHoldObject);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
