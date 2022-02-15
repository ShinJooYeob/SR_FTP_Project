#include "stdafx.h"
#include "..\public\TestCubeMove.h"

CTestCubeMove::CTestCubeMove(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CTestCubeMove::CTestCubeMove(const CTestCubeMove & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTestCubeMove::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CTestCubeMove::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(5.f, 20.f, 5.f));

	return S_OK;
}

_int CTestCubeMove::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

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

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CTestCubeMove::LateUpdate(_float fTimeDelta)
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

_int CTestCubeMove::Render()
{
	if (nullptr == m_ComVIBuffer)
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_ComVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

_int CTestCubeMove::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CTestCubeMove::Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	const _tchar* test = pDestObjects->Get_Layer_Tag();

	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_FixCube")))
	{
		_float3 vDestPos = ((CTransform*)(pDestObjects->Get_Component(TEXT("Com_Transform"))))->Get_MatrixState(CTransform::STATE_POS);
		_float3 vSourcePos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

		if (fCollision_Distance.x > fCollision_Distance.y)
		{
			if (fCollision_Distance.y > fCollision_Distance.z)
			{
				if (vSourcePos.z > vDestPos.z)
				{
					m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSourcePos + _float3(0, 0, fCollision_Distance.z));
				}
				else
				{
					m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSourcePos + _float3(0, 0, -fCollision_Distance.z));
				}
			}
			else
			{
				if (vSourcePos.y > vDestPos.y)
				{
					m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSourcePos + _float3(0, fCollision_Distance.y, 0));
				}
				else
				{
					m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSourcePos + _float3(0, -fCollision_Distance.y, 0));
				}
			}
		}
		else
		{
			if (fCollision_Distance.x > fCollision_Distance.z)
			{
				if (vSourcePos.z > vDestPos.z)
				{
					m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSourcePos + _float3(0, 0, fCollision_Distance.z));
				}
				else
				{
					m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSourcePos + _float3(0, 0, -fCollision_Distance.z));
				}
			}
			else
			{
				if (vSourcePos.x > vDestPos.x)
				{
					m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSourcePos + _float3(fCollision_Distance.x, 0, 0));
				}
				else
				{
					m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSourcePos + _float3(-fCollision_Distance.x, 0, 0));
				}
			}
		}
	}



	return _int();
}

HRESULT CTestCubeMove::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;


	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer_Cube"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;

	///////////////////////////////////////////////////////
	/* For.Com_Collision */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////
	return S_OK;
}

HRESULT CTestCubeMove::SetUp_OnTerrain(_float fDeltaTime)
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

HRESULT CTestCubeMove::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	/*
	_float4		vSourColor, vDestColor;

	(vSourColor.rgb) * vSourColor.a + (vDestColor.rgb) * (1.f - vSourColor.a);*/


	////////////////////////
	m_pGraphicDevice->SetTexture(0, NULL);
	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	/////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CTestCubeMove::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	///////////////////////////////

	return S_OK;
}

CTestCubeMove * CTestCubeMove::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CTestCubeMove* pInstance = new CTestCubeMove(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CPlayer_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CTestCubeMove::Clone(void * pArg)
{
	CTestCubeMove* pInstance = new CTestCubeMove((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CTestCubeMove_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CTestCubeMove::Free()
{
	__super::Free();

	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
