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

	/* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		m_Layer_Tag = (TEXT("Layer_Bell"));
		m_ComTransform->Scaled(_float3(1.f, 1.f, 1.f));
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

	seconds += fTimeDelta;

	//�ð����� fTimeDelta�� 1�ʸ� ����
	if (seconds > 3.f)
	{

		//////////////////////////////////////////////���� ���� ����Ÿ��,    ���۰���,    ��� ������,  ���ʺ���~,  ���ʵ���~
		_float TempAngle = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, m_RotAngle, m_RotAngle + 90, seconds - 3.f, 2.0f);


		if (seconds > 5.f)
		{
			seconds = 0;
			m_RotAngle = m_RotAngle + 90;  //������ �������� m_RotAngle�� �־��ش�. �̰� �־����� �ʴ´ٸ� ��� �ʱ�ȭ�� �ȴ�.
			TempAngle = m_RotAngle;//�ð��� ���� �� ������ �߻��ϱ� ������ Ÿ�پޱ۷� �ѹ� �� ����ó���� ���ش�. ���� �����带 float���� �ް� ��Ÿ�� ��� �������� �ʱ� ����
								   //GetSingle(CGameInstance)->PlaySound(L"EH_SelfRotationCube.wav", CHANNEL_OBJECT);
		}

		///////////////////////////������ ��,       ����
		m_ComTransform->Rotation_CW(_float3(0, 1, 0), D3DXToRadian(TempAngle));

	}


	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS)))
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


HRESULT CParsedObject_Bell::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. �ؽ���*/
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
