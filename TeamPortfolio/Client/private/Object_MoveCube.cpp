#include "stdafx.h"
#include "..\public\Object_MoveCube.h"

CObject_MoveCube::CObject_MoveCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_MoveCube::CObject_MoveCube(const CObject_MoveCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_MoveCube::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_MoveCube::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(0.f, 0.f, 0.f));

	m_Layer_Tag = L"Layer_Cube";

	m_ComTexture->Change_TextureLayer(TEXT("TerrainCube"));

	return S_OK;
}

_int CObject_MoveCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	//m_ComTransform->MovetoDir( _float3(0, -0.1f, 0), fTimeDelta);
	m_Temp = false;
	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FLEXIBLE, this);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_UP) & DIS_Press)
	{
		m_ComTransform->Move_Forward(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_DOWN) & DIS_Press)
	{
		m_ComTransform->Move_Backward(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_LEFT) & DIS_Press)
	{
		m_ComTransform->Move_Left(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_RIGHT) & DIS_Press)
	{
		m_ComTransform->Move_Right(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Press)
	{
		m_fNowJumpPower = m_fJumpPower;
		m_bIsJumped = true;

		m_ComTransform->MovetoTarget(m_ComTransform->Get_MatrixState(CTransform::STATE_POS) + _float3(0, 1.f, 0), fTimeDelta);
	}

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CObject_MoveCube::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;

	//if (FAILED(SetUp_OnTerrain(fTimeDelta)))
	//	return -1;
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CObject_MoveCube::Render()
{
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(0)))// 몇번째 인덱스인지 명시 0부터 시작
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_ComVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

_int CObject_MoveCube::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_MoveCube::Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	const _tchar* test = pDestObjects->Get_Layer_Tag();

	if (!m_Temp && !lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_FixCube")))
	{
		//Collision_Not_Moving(pDestObjects, fCollision_Distance,fDeltaTime);
		m_pCollisionCom->Collision_Pushed(m_ComTransform, fCollision_Distance, fDeltaTime);
		if (abs(fCollision_Distance.x) > abs(fCollision_Distance.y) && abs(fCollision_Distance.z) > abs(fCollision_Distance.y))
		{
			m_Temp = true;
			_float3 vPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
			vPos.y = ((CTransform*)(pDestObjects->Get_Component(TAG_COM(Com_Transform))))->Get_MatrixState(CTransform::STATE_POS).y + 1.f;
			m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vPos);
		}
	}
	else if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_PushCube")))
	{

		//m_pCollisionCom->Collision_Pushed(m_ComTransform, fCollision_Distance, fDeltaTime);
	}

	else if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_GravityCube")))
	{
		//플레이어 3초 전으로 돌아가겠끔

		int i = 0;
		//m_pCollisionCom->Collision_Pushed(m_ComTransform, fCollision_Distance, fDeltaTime);
		//m_pCollisionCom->Collision_Suck_In(m_ComTransform, fCollision_Distance, fDeltaTime);
	}
	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_LeftCube")))
	{
		m_pCollisionCom->Collision_Pushed(m_ComTransform, fCollision_Distance, fDeltaTime);
	}
	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_RightCube")))
	{
		m_pCollisionCom->Collision_Pushed(m_ComTransform, fCollision_Distance, fDeltaTime);
	}
	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_VanishCube")))
	{
		m_pCollisionCom->Collision_Pushed(m_ComTransform, fCollision_Distance, fDeltaTime);
	}
	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_AppearCube")))
	{
		m_pCollisionCom->Collision_Pushed(m_ComTransform, fCollision_Distance, fDeltaTime);
	}

	return _int();
}

_int CObject_MoveCube::Collision_Not_Moving(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	//_float3 vDestPos = ((CTransform*)(pDestObjects->Get_Component(TEXT("Com_Transform"))))->Get_MatrixState(CTransform::STATE_POS);
	_float3 vSourcePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

	if (abs(fCollision_Distance.x) > abs(fCollision_Distance.y))
	{
		if (abs(fCollision_Distance.y) > abs(fCollision_Distance.z))
		{
			m_ComTransform->MovetoDir(_float3(0, 0, fCollision_Distance.z), fDeltaTime);
		}
		else
		{
			m_ComTransform->MovetoDir(_float3(0, fCollision_Distance.y, 0), fDeltaTime);
		}
	}
	else
	{
		if (abs(fCollision_Distance.x) > abs(fCollision_Distance.z))
		{
			m_ComTransform->MovetoDir(_float3(0, 0, fCollision_Distance.z), fDeltaTime);
		}
		else
		{
			m_ComTransform->MovetoDir(_float3(fCollision_Distance.x, 0, 0), fDeltaTime);
		}
	}

	return _int();
}

HRESULT CObject_MoveCube::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Cube_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;


	///////////////////////////////////////////////////////
	/* For.Com_Collision */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////
	return S_OK;
}

HRESULT CObject_MoveCube::SetUp_OnTerrain(_float fDeltaTime)
{
	CGameInstance* pGameInstance = GetSingle(CGameInstance);


	CGameObject* pTerrain = pGameInstance->Get_GameObject_By_LayerIndex(SCENEID::SCENE_STAGESELECT, TEXT("Layer_Terrain"));

	if (pTerrain == nullptr)
		return E_FAIL;

	CVIBuffer_Terrain* pTerrainBuffer = (CVIBuffer_Terrain*)(pTerrain->Find_Components(TEXT("Com_VIBuffer")));
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

			vResultPos.y = (Temp > vResultPos.y) ? Temp : vResultPos.y;
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

HRESULT CObject_MoveCube::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	////m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	////m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	////m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	////m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	///*
	//_float4		vSourColor, vDestColor;

	//(vSourColor.rgb) * vSourColor.a + (vDestColor.rgb) * (1.f - vSourColor.a);*/


	//////////////////////////
	//m_pGraphicDevice->SetTexture(0, NULL);
	//m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	///////////////////////////////////////////////////////////////////////////


	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	return S_OK;
}

HRESULT CObject_MoveCube::Release_RenderState()
{

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	/////////////////////////////
	//m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	/////////////////////////////////

	return S_OK;
}

CObject_MoveCube * CObject_MoveCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_MoveCube* pInstance = new CObject_MoveCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_MoveCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_MoveCube::Clone(void * pArg)
{
	CObject_MoveCube* pInstance = new CObject_MoveCube((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_MoveCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_MoveCube::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}