#include "stdafx.h"
#include "..\public\ParsedObject_Bell.h"


CParsedObject_Bell::CParsedObject_Bell(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParsedObject(pGraphic_Device)
{
}

CParsedObject_Bell::CParsedObject_Bell(const CParsedObject_Bell & rhs)
	: CParsedObject(rhs)
{
}

HRESULT CParsedObject_Bell::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParsedObject_Bell::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		m_Layer_Tag = (TEXT("Layer_Bell"));
		m_ComTransform->Scaled(_float3(0.7f, 0.7f, 0.7f));
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSettingPoint);
	}

	FAILED_CHECK(m_ComTexture->Change_TextureLayer(TEXT("Bell")));

	return S_OK;
}

_int CParsedObject_Bell::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;



	return _int();
}

_int CParsedObject_Bell::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


	////////////////////////////////////////////////시간단위 fTimeDelta는 1초를 뜻함
	//if (seconds > 3.f)
	//{

	//	//////////////////////////////////////////////쓰고 싶은 보간타입,    시작각도,    몇도를 돌릴지,  몇초부터~,  몇초동안~
	//	_float TempAngle = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, m_RotAngle, m_RotAngle + 90, seconds - 3.f, 2.0f);


	//	if (seconds > 5.f)
	//	{
	//		seconds = 0;
	//		m_RotAngle = m_RotAngle + 90;  //각도를 돌렸으니 m_RotAngle에 넣어준다. 이걸 넣어주지 않는다면 계속 초기화가 된다.
	//		TempAngle = m_RotAngle;//시간이 끝날 때 오차가 발생하기 때문에 타겟앵글로 한번 더 예외처리를 해준다. 지금 세컨드를 float으로 받고 델타도 사실 일정하지 않기 때문
	//							   //GetSingle(CGameInstance)->PlaySound(L"EH_SelfRotationCube.wav", CHANNEL_OBJECT);
	//	}

	//	///////////////////////////임의의 축,       라디안
	//	m_ComTransform->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(TempAngle));

//}

	if (seconds < 0.5)
	{
		seconds += (fTimeDelta);
		m_distance += fTimeDelta * 16;
		if (seconds > 0.5)
		{
			m_distance = 8;
		}
	}
	else if(seconds < 1.5)
	{
		seconds += (fTimeDelta);
		m_distance -= fTimeDelta * 16;
		if (seconds > 1.5)
		{
			m_distance = -8;
		}

	}
	else if (seconds < 2)
	{
		seconds += (fTimeDelta);
		m_distance += fTimeDelta * 16;
		if (seconds > 2)
		{
			m_distance = 0;
		}

	}
	else
	{
		seconds = 0;
	}

	m_ComTransform->Rotation_CW(_float3(1, 0, 0), D3DXToRadian(m_distance));

	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS),7))
		m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}

_int CParsedObject_Bell::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	if (FAILED(m_ComTexture->Bind_Texture()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	FAILED_CHECK(m_ComVIBuffer->Render());

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return _int();
}

_int CParsedObject_Bell::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CParsedObject_Bell::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}


static _uint iBellCreateCounter = 0;

HRESULT CParsedObject_Bell::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_Parsed"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Bell"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;



	///////////////////////////////////////////////////////
	/* For.Com_Collision */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
		return E_FAIL;
	//////////////////////////////////////////////



	if (iBellCreateCounter == 0)
	{
		m_ComVIBuffer->Fix_Vertex_By_Postion(_float3(0, -3.5f, 0));
	}





	iBellCreateCounter++;

	return S_OK;
}

HRESULT CParsedObject_Bell::SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CParsedObject_Bell::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	///////////////////////////
	///////////////////////////////

	return S_OK;
}

CParsedObject_Bell * CParsedObject_Bell::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParsedObject_Bell* pInstance = new CParsedObject_Bell(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_Bell");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CParsedObject_Bell::Clone(void * pArg)
{
	CParsedObject_Bell* pInstance = new CParsedObject_Bell((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_Bell");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CParsedObject_Bell::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
}
