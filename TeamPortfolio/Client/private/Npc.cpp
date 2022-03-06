#include "stdafx.h"
#include "..\public\Npc.h"

CNpc::CNpc(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{

}

CNpc::CNpc(const CNpc& rhs)
	: CGameObject(rhs)
{

}


HRESULT CNpc::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CNpc::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CNpc::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	return _int();
}

_int CNpc::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;

	return _int();
}

_int CNpc::Render()
{

	if (FAILED(__super::Render()))
		return E_FAIL;
	
	return S_OK;
}

_int CNpc::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;
	
	return S_OK();
}

HRESULT CNpc::ReInitialize(void * pArg)
{
	if (FAILED(__super::ReInitialize(pArg)))
		return E_FAIL;
	return S_OK;
}


void CNpc::Free()
{
	__super::Free();
	
}
