#include "stdafx.h"
#include "..\public\MonsterParent.h"

CMonsterParent::CMonsterParent(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{

}

CMonsterParent::CMonsterParent(const CMonsterParent& rhs)
	: CGameObject(rhs)
{

}


HRESULT CMonsterParent::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonsterParent::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));



	return S_OK;
}

_int CMonsterParent::Update(_float fDeltaTime)
{
	FAILED_CHECK(__super::Update(fDeltaTime));

	return _int();
}

_int CMonsterParent::LateUpdate(_float fDeltaTime)
{
	FAILED_CHECK(__super::LateUpdate(fDeltaTime));

	return _int();
}

_int CMonsterParent::Render()
{

	FAILED_CHECK(__super::Render());
	return S_OK;
}

_int CMonsterParent::LateRender()
{
	FAILED_CHECK(__super::LateRender());
	
	return S_OK();
}

HRESULT CMonsterParent::SetUp_Components()
{

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_Test), TAG_COM(Com_Shader), (CComponent**)&m_ComShader)))
		return E_FAIL;

	// 정의 안한것은 자식클래스에서 정의된다.

	return S_OK;
}

void CMonsterParent::Free()
{
	__super::Free();
	
}
