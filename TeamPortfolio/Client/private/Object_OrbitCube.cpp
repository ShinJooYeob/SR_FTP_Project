#include "stdafx.h"
#include "Object_OrbitButton.h"
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
	m_ComTexture->Change_TextureLayer(TEXT("OrbitCube"));


	//이렇게 구한 부모 행렬과 각 각의 월드 행렬과 곱해줘야한다.?




	return S_OK;
}

_int CObject_OrbitCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);


	if (!m_bIsTurning && m_pButtonCube->Get_IsButtonActive())//여기에 있는건 Y값을 구하기 위함이다. 왜냐하면 위에선 y를 0으로 만들고 새로 x,z로 좌표를 구하고 각도를 구했기 때문이다.
	{
		m_bIsTurning = true;
		//일단 트랜스폼의 위치를 가져옴
		_float3 vTempObjectPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

		//그리고 로테이션 축을 가져온다. 이때 우린 Y축이 필요없다. x와 z만 가져와서 공전 위치를 정해줄테니깐.  추가내용)아직까진 X,Y,Z가 들어있다.
		_float3 vRotAxis = m_OrbitCubeDesc.fRotAxis;

		//그렇기 때문에 y를 0으로 초기화시켜준다.
		vRotAxis.y = 0;

		//로테이션의 룩을 구해준다.    오브젝트포지션 - 임의의 축을 해서 임의의 축이 오브젝트로 향한 룩 벡터가 나왔다.
		_float3 vTempLookVector_Y_Zero = vTempObjectPos - vRotAxis;

		//vTempObjectPos의 y값이 들어왔으니 0으로 다시 맞춰준다.           추가내용)Y가 사라지고  X,0,Z가 들어있다.
		vTempLookVector_Y_Zero.y = 0;

		//아크코사인은 각도를 알 수 있다. 일단 이건 집에가서 보자. 기준이 필요하기 때문이다. 모든 객체가 정면을 향하고 깨끗하게 90도라고 말할 순 없다. 그러기 때문에 x축을 기준으로 사이각을 구한거다.
		_float fRadianAngle = acosf(vTempLookVector_Y_Zero.Get_Nomalize().Get_Dot(_float3(1, 0, 0))); //x로 향하는 방향벡터와 우리가 구한 좌표값을 내적하여 아크코사인으로 그 사이각을 구한다!!!

		if (vRotAxis.z > vTempObjectPos.z)
			//라디안으로 계산을 하다가 180도를 넘어가면 코사인에 문제가 생긴다. 그래서 예외처리로 360도에서 라디안 각만큼 뺀 값을 이용한다.
			fRadianAngle = 2 * D3DX_PI - fRadianAngle;
		//

		//라디안을 구했으니 스타트 앵글로 맞춰준다.
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

	if (m_bIsTurning)
	{

		//보간하려고 값 증가시킴
		m_PassedTime += fTimeDelta;

		//객체의 x,z의 좌표를 구한다.
		_float3 vTempObjectPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
		vTempObjectPos.y = 0;

		//이게 그 새로 구한 좌표에 들어갈 예정 즉, 임의의 축
		_float3 vRevolutoinPos_Y_Zero = m_OrbitCubeDesc.fRotAxis;
		vRevolutoinPos_Y_Zero.y = 0;

		//일단 새로 구한 임의의 축의 룩 벡터를 구하고 y에 0를 넣는다. 이유: 어차피 X와 Z로 좌표만 설정해주려고 즉, y방향으로 돌기 때문에
		_float3 vTempLookVector_Y_Zero = vTempObjectPos - vRevolutoinPos_Y_Zero;
		vTempLookVector_Y_Zero.y = 0;

		_float fDist = vTempLookVector_Y_Zero.Get_Distance(_float3(0, 0, 0));//거리를 구하기 위함이다. 0,0,0을 하는 이유는 vTempLookVector_Y_Zero의 순수하게 그 거리만을 알기 위해서이다.

		_float fRadianAngle = GetSingle(CGameInstance)->Easing(TYPE_ElasticIn, m_StartAngle, m_TargetAngle, m_PassedTime, 2.f); //보간처리해주고

		if (m_PassedTime > 2.f)
		{
			m_bIsTurning = false;
			m_StartAngle = m_TargetAngle;
			fRadianAngle = m_TargetAngle;
		}

		//cosf으로 x좌표를 sinf으로 z좌표를 알 수 있으며 거기에 거리를 곱하면 그 좌표의 주솟값을 얻을 수 있다. 
		vTempObjectPos.x = cosf(fRadianAngle) * fDist;
		vTempObjectPos.z = sinf(fRadianAngle) * fDist;

		vTempObjectPos += vRevolutoinPos_Y_Zero; //새로 구한 임의의 축 x와 z를 넣어줘야 우리가 생각했던 임의의 축으로 이동한다.

												 //우린 y가 0이였으므로 포지션중에서 y만 가져와서 넣어준다.
		vTempObjectPos.y = m_ComTransform->Get_MatrixState(CTransform::STATE_POS).y; //우린 vTempObjectPos.y를 0으로 해줬으므로 Y값을 구해 가져온다.
																					 //우린 Y 축 기준으로 x와z만 이동시켜줬으므로 Y는 처음 넣어줬던 값 그대로이다.
																					 //만약 X축 또는 Z축 기준으로 하고자 한다면 그걸 기준으로 해주면 된다.

																					 //그리고 템프 오프젝트를 셋 매트릭스로 보내준다.
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vTempObjectPos); //그렇게 구한 값을 넣어준다.

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

	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS)))
	m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CObject_OrbitCube::Render()
{
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	//m_pGraphicDevice->SetTransform(D3DTS_WORLD, &parentMatrix);


	if (FAILED(m_ComTexture->Bind_Texture(1)))
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
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Cube_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
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