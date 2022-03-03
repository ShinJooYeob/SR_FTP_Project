#include "stdafx.h"
#include "ObjectTool_ShaderTestObject.h"
#include "VIBuffer.h"


CObjectTool_ShaderTestObject::CObjectTool_ShaderTestObject(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice),
	m_ComRenderer(nullptr),
	m_ComVIBuffer(nullptr),
	m_ComTransform(nullptr),
	m_ComTexture(nullptr)

{

}

CObjectTool_ShaderTestObject::CObjectTool_ShaderTestObject(const CObjectTool_ShaderTestObject & rhs)
	:CGameObject(rhs)
{
}

HRESULT CObjectTool_ShaderTestObject::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObjectTool_ShaderTestObject::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CObjectTool_ShaderTestObject::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	return _int();
}

_int CObjectTool_ShaderTestObject::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;

	//렌더링 그룹에 넣어주는 역활
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return 0;
}



_int CObjectTool_ShaderTestObject::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	NULL_CHECK_BREAK(m_ComTransform);
	NULL_CHECK_BREAK(m_ComTexture);
	NULL_CHECK_BREAK(m_ComVIBuffer);

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	//if (FAILED(m_ComTexture->Bind_Texture(m_tOutputData.TexDesc.StateIndex)))
	//	return E_FAIL;

	//렌더링 그룹에 들어가면 순서에 맞게 이 랜더가 호출되고 호출이 됬으면 버텍스 버퍼를 그려줘라
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	return 0;
}

_int CObjectTool_ShaderTestObject::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;
	return 0;
}

HRESULT CObjectTool_ShaderTestObject::Set_WorldMat(_Matrix world)
{
	m_ComTransform->Set_Matrix(world);
	return S_OK;
}

HRESULT CObjectTool_ShaderTestObject::Set_Scaled(_float3 scale)
{
	NULL_CHECK_BREAK(m_ComTransform);

	if (scale.x == 0)
		scale.x = 1.f;

	if (scale.y == 0)
		scale.y = 1.f;

	if (scale.z == 0)
		scale.z = 1.f;

	m_ComTransform->Scaled(scale);
	return S_OK;
}

HRESULT CObjectTool_ShaderTestObject::Set_Rotation(_float3 rot)
{
	NULL_CHECK_BREAK(m_ComTransform);

	D3DXQUATERNION quat_x, quat_y, quat_z, quat_1, quat_2;
	_Matrix matrix;
	D3DXVECTOR3 axis_x(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 axis_y(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 axis_z(0.0f, 0.0f, 1.0f);
	D3DXQuaternionRotationAxis(&quat_x, &axis_x, rot.x);
	D3DXQuaternionRotationAxis(&quat_y, &axis_y, rot.y);
	D3DXQuaternionRotationAxis(&quat_z, &axis_z, rot.z);

	quat_1 = quat_y * quat_x * quat_z;
	D3DXQuaternionNormalize(&quat_2, &quat_1);
	D3DXMatrixRotationQuaternion(&matrix, &quat_2);

	m_ComTransform->Set_MatrixState(CTransform::STATE_RIGHT, *(_float3*)&matrix.m[0]);
	m_ComTransform->Set_MatrixState(CTransform::STATE_UP, *(_float3*)&matrix.m[1]);
	m_ComTransform->Set_MatrixState(CTransform::STATE_LOOK, *(_float3*)&matrix.m[2]);

	return S_OK;
}



HRESULT CObjectTool_ShaderTestObject::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Cube), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	
	return S_OK;
}

CObjectTool_ShaderTestObject * CObjectTool_ShaderTestObject::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg)
{
	CObjectTool_ShaderTestObject* pInstance = new CObjectTool_ShaderTestObject(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create BackGround_ProtoType");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CObjectTool_ShaderTestObject::Clone(void * pArg)
{
	CObjectTool_ShaderTestObject* pInstance = new CObjectTool_ShaderTestObject((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create BackGround_Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObjectTool_ShaderTestObject::Free()
{
	__super::Free();

	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);

}