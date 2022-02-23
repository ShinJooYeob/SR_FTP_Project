#include "stdafx.h"
#include "..\public\Object_OrbitCube.h"


CObject_OrbitCube::CObject_OrbitCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_OrbitCube::CObject_OrbitCube(const CObject_OrbitCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_OrbitCube::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;



	return S_OK;
}

HRESULT CObject_OrbitCube::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;
	//m_pGraphicDevice
	if (pArg != nullptr)
	{
		memcpy(&m_OrbitCubeDesc, pArg, sizeof(ORBITCUBEDESC));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, m_OrbitCubeDesc.fTransform);
	}
	else
	{
		MSGBOX("Fail to Clone CObject_OrbitCube");
	}



	m_RotAxis = _float3(5, 0, 5);



	//이렇게 구한 부모 행렬과 각 각의 월드 행렬과 곱해줘야한다.?




	return S_OK;
}

_int CObject_OrbitCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);



	if (!m_bIsTurning && GetSingle(CGameInstance)->Get_DIKeyState(DIK_K) & DIS_Down)
	{

		//일단 트랜스폼의 위치를 가져옴
		_float3 vTempObjectPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

		//그리고 로테이션 축을 가져온다. 이때 우린 Y축이 필요없다. x와 z만 가져와서 공전 위치를 정해줄테니깐.
		_float3 vRotAxis = m_RotAxis;

		//혹시 모르니 예외처리로 Y에 0을 넣어준다.
		vRotAxis.y = 0;

		//로테이션의 룩을 구해준다.
		_float3 vTempLookVector_Y_Zero = vTempObjectPos - vRotAxis;

		//vTempObjectPos의 y값이 들어왔으니 0으로 다시 맞춰준다.
		vTempLookVector_Y_Zero.y = 0;

		//아크코사인은 각도를 알 수 있다. 일단 이건 집에가서 보자.
		_float fRadianAngle = acosf(vTempLookVector_Y_Zero.Get_Nomalize().Get_Dot(_float3(1, 0, 0)));

		if (vRotAxis.z > vTempObjectPos.z)
			//라디안으로 계산을 하다가 180도를 넘어가면 코사인에 문제가 생긴다. 그래서 예외처리로 360도에서 라디안 각만큼 뺀 값을 이용한다.
			fRadianAngle = 2 * D3DX_PI - fRadianAngle;
		//

		//스타트 앵글을 다시 맞춰준다.
		m_StartAngle = fRadianAngle;

		//타겟은 스타트의 90도를 더한 만큼이기 때문에 이렇게 해준다.
		m_TargetAngle = fRadianAngle + D3DXToRadian(90);

		//이거 디버그해서 문제점 찾으려고 한것뿐 신경쓰지말자.
	/*	_float temp = D3DXToDegree(m_StartAngle);
		_float temp2 = D3DXToDegree(m_TargetAngle);*/

		//보간하려고 함
		m_PassedTime = 0;

		//턴이 온일 경우 이게 들어오지 않음
		m_bIsTurning = true;

	}
	if (m_bIsTurning)//여기에 있는건 Y값을 구하기 위함이다. 왜냐하면 위에선 y를 0으로 만들고 새로 x,z로 좌표를 구했기 때문이다.
	{
		//보간하려고 값 증가시킴
		m_PassedTime += fTimeDelta;

		//객체의 x,z의 좌표를 구한다.
		_float3 vTempObjectPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
		vTempObjectPos.y = 0;

		//이게 그 새로 구한 좌표에 들어갈 예정
		_float3 vRevolutoinPos_Y_Zero = m_RotAxis;
		vRevolutoinPos_Y_Zero.y = 0;

		_float3 vTempLookVector_Y_Zero = vTempObjectPos - vRevolutoinPos_Y_Zero;
		vTempLookVector_Y_Zero.y = 0;

		_float fDist = vTempLookVector_Y_Zero.Get_Distance(_float3(0, 0, 0));

		_float fRadianAngle = GetSingle(CGameInstance)->Easing(TYPE_ElasticIn, m_StartAngle, m_TargetAngle, m_PassedTime);

		if (m_PassedTime > 1.f)
		{
			m_bIsTurning = false;
			m_StartAngle = m_TargetAngle;
			fRadianAngle = m_TargetAngle;
		}


		vTempObjectPos.x = cosf(fRadianAngle) * fDist;
		vTempObjectPos.z = sinf(fRadianAngle) * fDist;
		
		vTempObjectPos += vRevolutoinPos_Y_Zero;

		vTempObjectPos.y = m_ComTransform->Get_MatrixState(CTransform::STATE_POS).y;

		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vTempObjectPos);

		//m_pTransform->LookAt(vRevPos);

	}








	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CObject_OrbitCube::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;

	//seconds += fTimeDelta;

	////시간단위 fTimeDelta는 1초를 뜻함
	//if (seconds > 3.f)
	//{

	//	//////////////////////////////////////////////쓰고 싶은 보간타입,    시작각도,    몇도를 돌릴지,  몇초부터~,  몇초동안~
	//	_float TempAngle = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, m_RotAngle, m_RotAngle + 90, seconds - 3.f, 2.0f);


	//	if (seconds > 5.f)
	//	{
	//		seconds = 0;
	//		m_RotAngle = m_RotAngle + 90;  //각도를 돌렸으니 m_RotAngle에 넣어준다. 이걸 넣어주지 않는다면 계속 초기화가 된다.
	//		TempAngle = m_RotAngle;//시간이 끝날 때 오차가 발생하기 때문에 타겟앵글로 한번 더 예외처리를 해준다. 지금 세컨드를 float으로 받고 델타도 사실 일정하지 않기 때문
	//	}

	//	///////////////////////////임의의 축,       라디안
	//	m_ComTransform->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(TempAngle));

	//}


	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CObject_OrbitCube::Render()
{
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	//m_pGraphicDevice->SetTransform(D3DTS_WORLD, &parentMatrix);


	if (FAILED(m_ComTexture->Bind_Texture()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_ComVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

_int CObject_OrbitCube::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_OrbitCube::Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	const _tchar* test = pDestObjects->Get_Layer_Tag();

	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_Cube")))
	{
		int t = 0;
	}

	return _int();
}

HRESULT CObject_OrbitCube::SetUp_Components()
{
	/* For.트랜스폼 */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	/* For.트랜스폼 */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(SCENE_STAGE2, TEXT("Prototype_Component_Object_OrbitCube_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	/* For.렌더러 */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.버텍스 큐브 */

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	///////////////////////////////////////////////////////
	/* For.콜리전 */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////

	return S_OK;
}

HRESULT CObject_OrbitCube::SetUp_RenderState()
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

	return S_OK;
}

HRESULT CObject_OrbitCube::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

CObject_OrbitCube * CObject_OrbitCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_OrbitCube* pInstance = new CObject_OrbitCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_OrbitCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_OrbitCube::Clone(void * pArg)
{
	CObject_OrbitCube* pInstance = new CObject_OrbitCube((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_OrbitCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_OrbitCube::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
	Safe_Release(m_ComTexture);
}