#include "stdafx.h"
#include "..\public\ParsedObject.h"


CParsedObject::CParsedObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CParsedObject::CParsedObject(const CParsedObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CParsedObject::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CParsedObject::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	return S_OK;
}

_int CParsedObject::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;


	return _int();
}

_int CParsedObject::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;


	return _int();
}

_int CParsedObject::Render()
{

	return _int();
}

_int CParsedObject::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

_int CParsedObject::Obsever_On_Trigger(CGameObject * pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{

	return _int();
}

CParsedObject * CParsedObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParsedObject* pInstance = new CParsedObject(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CParsedObject::Clone(void * pArg)
{
	CParsedObject* pInstance = new CParsedObject((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CParsedObject::Free()
{
	__super::Free();
}
