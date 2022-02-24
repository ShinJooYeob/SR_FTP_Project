#include "stdafx.h"
#include "..\public\Object_InteractiveCube.h"


CObject_InteractiveCube::CObject_InteractiveCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CObject_InteractiveCube::CObject_InteractiveCube(const CObject_InteractiveCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObject_InteractiveCube::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObject_InteractiveCube::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, _float3(0.f, 5.f, 2.f));

	m_ComTexture->Change_TextureLayer(TEXT("OrbitCube"));

	return S_OK;
}

_int CObject_InteractiveCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CObject_InteractiveCube::LateUpdate(_float fTimeDelta)
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

_int CObject_InteractiveCube::Render()
{

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_ComVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

_int CObject_InteractiveCube::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CObject_InteractiveCube::Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	const _tchar* test = pDestObjects->Get_Layer_Tag();

	if (!lstrcmp(pDestObjects->Get_Layer_Tag(), TEXT("Layer_Cube")))
	{
		int t = 0;
	}

	return _int();
}

HRESULT CObject_InteractiveCube::SetUp_Components()
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

HRESULT CObject_InteractiveCube::SetUp_RenderState()
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

HRESULT CObject_InteractiveCube::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

CObject_InteractiveCube * CObject_InteractiveCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CObject_InteractiveCube* pInstance = new CObject_InteractiveCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CObject_InteractiveCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CObject_InteractiveCube::Clone(void * pArg)
{
	CObject_InteractiveCube* pInstance = new CObject_InteractiveCube((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CObject_InteractiveCube");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CObject_InteractiveCube::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
	Safe_Release(m_ComTexture);
}
