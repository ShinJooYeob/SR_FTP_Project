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

	/* ���� ��ü���� �߰��Ǿ���� ������Ʈ���� ����(or ����)�Ͽ� ��������� �����Ѵ�.  */
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



	//�̷��� ���� �θ� ��İ� �� ���� ���� ��İ� ��������Ѵ�.?




	return S_OK;
}

_int CObject_OrbitCube::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	m_pCollisionCom->Add_CollisionGroup(CCollision::COLLISIONGROUP::COLLISION_FIX, this);



	if (!m_bIsTurning && GetSingle(CGameInstance)->Get_DIKeyState(DIK_K) & DIS_Down)//�̰� X�� Z��ǥ�� ��ġ�� ���ϰ� � ������ ������ �������� ���ϰ� �ִ�.
	{

		//�ϴ� Ʈ�������� ��ġ�� ������
		_float3 vTempObjectPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);

		//�׸��� �����̼� ���� �����´�. �̶� �츰 Y���� �ʿ����. x�� z�� �����ͼ� ���� ��ġ�� �������״ϱ�.  �߰�����)�������� X,Y,Z�� ����ִ�.
		_float3 vRotAxis = m_RotAxis;

		//�׷��� ������ y�� 0���� �ʱ�ȭ�����ش�.
		vRotAxis.y = 0;

		//�����̼��� ���� �����ش�.    ������Ʈ������ - ������ ���� �ؼ� ������ ���� ������Ʈ�� ���� �� ���Ͱ� ���Դ�.
		_float3 vTempLookVector_Y_Zero = vTempObjectPos - vRotAxis;

		//vTempObjectPos�� y���� �������� 0���� �ٽ� �����ش�.           �߰�����)Y�� �������  X,0,Z�� ����ִ�.
		vTempLookVector_Y_Zero.y = 0;

		//��ũ�ڻ����� ������ �� �� �ִ�. �ϴ� �̰� �������� ����. ������ �ʿ��ϱ� �����̴�. ��� ��ü�� ������ ���ϰ� �����ϰ� 90����� ���� �� ����. �׷��� ������ x���� �������� ���̰��� ���ѰŴ�.
		_float fRadianAngle = acosf(vTempLookVector_Y_Zero.Get_Nomalize().Get_Dot(_float3(1, 0, 0))); //x�� ���ϴ� ���⺤�Ϳ� �츮�� ���� ��ǥ���� �����Ͽ� ��ũ�ڻ������� �� ���̰��� ���Ѵ�!!!

		if (vRotAxis.z > vTempObjectPos.z)
			//�������� ����� �ϴٰ� 180���� �Ѿ�� �ڻ��ο� ������ �����. �׷��� ����ó���� 360������ ���� ����ŭ �� ���� �̿��Ѵ�.
			fRadianAngle = 2 * D3DX_PI - fRadianAngle;
		//

		//������ �������� ��ŸƮ �ޱ۷� �����ش�.
		//��ŸƮ �ޱ��� �ٽ� �����ش�.
		m_StartAngle = fRadianAngle;

		//Ÿ���� ��ŸƮ�� 90���� ���� ��ŭ�̱� ������ �̷��� ���ش�.
		m_TargetAngle = fRadianAngle + D3DXToRadian(90);

		//�̰� ������ؼ� ������ ã������ �Ѱͻ� �Ű澲������.
	/*	_float temp = D3DXToDegree(m_StartAngle);
		_float temp2 = D3DXToDegree(m_TargetAngle);*/

		//�����Ϸ��� ��
		m_PassedTime = 0;

		//���� ���� ��� �̰� ������ ����
		m_bIsTurning = true;

	}
	if (m_bIsTurning)//���⿡ �ִ°� Y���� ���ϱ� �����̴�. �ֳ��ϸ� ������ y�� 0���� ����� ���� x,z�� ��ǥ�� ���ϰ� ������ ���߱� �����̴�.
	{
		//�����Ϸ��� �� ������Ŵ
		m_PassedTime += fTimeDelta;

		//��ü�� x,z�� ��ǥ�� ���Ѵ�.
		_float3 vTempObjectPos = m_ComTransform->Get_MatrixState(CTransform::STATE_POS);
		vTempObjectPos.y = 0;

		//�̰� �� ���� ���� ��ǥ�� �� ���� ��, ������ ��
		_float3 vRevolutoinPos_Y_Zero = m_RotAxis;
		vRevolutoinPos_Y_Zero.y = 0;

		//�ϴ� ���� ���� ������ ���� �� ���͸� ���ϰ� y�� 0�� �ִ´�. ����: ������ X�� Z�� ��ǥ�� �������ַ��� ��, y�������� ���� ������
		_float3 vTempLookVector_Y_Zero = vTempObjectPos - vRevolutoinPos_Y_Zero;
		vTempLookVector_Y_Zero.y = 0;

		_float fDist = vTempLookVector_Y_Zero.Get_Distance(_float3(0, 0, 0));//�Ÿ��� ���ϱ� �����̴�. 0,0,0�� �ϴ� ������ vTempLookVector_Y_Zero�� �����ϰ� �� �Ÿ����� �˱� ���ؼ��̴�.

		_float fRadianAngle = GetSingle(CGameInstance)->Easing(TYPE_ElasticIn, m_StartAngle, m_TargetAngle, m_PassedTime); //����ó�����ְ�

		if (m_PassedTime > 1.f)
		{
			m_bIsTurning = false;
			m_StartAngle = m_TargetAngle;
			fRadianAngle = m_TargetAngle;
		}

		//cosf���� x��ǥ�� sinf���� z��ǥ�� �� �� ������ �ű⿡ �Ÿ��� ���ϸ� �� ��ǥ�� �ּڰ��� ���� �� �ִ�. 
		vTempObjectPos.x = cosf(fRadianAngle) * fDist;
		vTempObjectPos.z = sinf(fRadianAngle) * fDist;
		
		vTempObjectPos += vRevolutoinPos_Y_Zero; //���� ���� ������ �� x�� z�� �־���� �츮�� �����ߴ� ������ ������ �̵��Ѵ�.

		//�츰 y�� 0�̿����Ƿ� �������߿��� y�� �����ͼ� �־��ش�.
		vTempObjectPos.y = m_ComTransform->Get_MatrixState(CTransform::STATE_POS).y; //�츰 vTempObjectPos.y�� 0���� �������Ƿ� Y���� ���� �����´�.
																					//�츰 Y �� �������� x��z�� �̵����������Ƿ� Y�� ó�� �־���� �� �״���̴�.
																					//���� X�� �Ǵ� Z�� �������� �ϰ��� �Ѵٸ� �װ� �������� ���ָ� �ȴ�.

		//�׸��� ���� ������Ʈ�� �� ��Ʈ������ �����ش�.
		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vTempObjectPos); //�׷��� ���� ���� �־��ش�.

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

	////�ð����� fTimeDelta�� 1�ʸ� ����
	//if (seconds > 3.f)
	//{

	//	//////////////////////////////////////////////���� ���� ����Ÿ��,    ���۰���,    ��� ������,  ���ʺ���~,  ���ʵ���~
	//	_float TempAngle = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, m_RotAngle, m_RotAngle + 90, seconds - 3.f, 2.0f);


	//	if (seconds > 5.f)
	//	{
	//		seconds = 0;
	//		m_RotAngle = m_RotAngle + 90;  //������ �������� m_RotAngle�� �־��ش�. �̰� �־����� �ʴ´ٸ� ��� �ʱ�ȭ�� �ȴ�.
	//		TempAngle = m_RotAngle;//�ð��� ���� �� ������ �߻��ϱ� ������ Ÿ�پޱ۷� �ѹ� �� ����ó���� ���ش�. ���� �����带 float���� �ް� ��Ÿ�� ��� �������� �ʱ� ����
	//	}

	//	///////////////////////////������ ��,       ����
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
	/* For.Ʈ������ */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	/* For.Ʈ������ */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	/* For. �ؽ���*/
	if (FAILED(__super::Add_Component(SCENE_STAGE2, TEXT("Prototype_Component_Object_OrbitCube_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	/* For.������ */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	/* For.���ؽ� ť�� */

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;


	///////////////////////////////////////////////////////
	/* For.�ݸ��� */
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