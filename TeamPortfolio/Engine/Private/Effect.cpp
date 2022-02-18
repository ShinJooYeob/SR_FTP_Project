#include "..\Public\Effect.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

CEffect::CEffect(const CEffect & rhs)
	:CGameObject(rhs)
{
}

HRESULT CEffect::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CEffect::Update(_float fTimeDelta)
{
	return _int();
}

_int CEffect::LateUpdate(_float fTimeDelta)
{
	return _int();
}

_int CEffect::Render()
{
	return _int();
}

_int CEffect::LateRender()
{
	return _int();
}

void CEffect::Free()
{
	__super::Free();
}
