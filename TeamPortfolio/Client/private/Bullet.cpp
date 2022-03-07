#include "stdafx.h"
#include "..\public\Bullet.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CMonsterParent(pGraphicDevice)
{

}

CBullet::CBullet(const CBullet& rhs)
	: CMonsterParent(rhs)
{

}


HRESULT CBullet::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBullet::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	return S_OK;
}

_int CBullet::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	return _int();
}

_int CBullet::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;

	return _int();
}

_int CBullet::Render()
{

	if (FAILED(__super::Render()))
		return E_FAIL;
	
	return S_OK;
}

_int CBullet::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;
	
	return S_OK();
}



HRESULT CBullet::SetUp_RenderState()
{
	return S_OK;
}

HRESULT CBullet::Release_RenderState()
{
	return S_OK;

}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CBullet* pInstance = new CBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CBullet");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CBullet * CBullet::Clone(void * pArg)
{
	CBullet* pInstance = new CBullet((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CBullet");
		Safe_Release(pInstance);

	}
	return pInstance;
}

void CBullet::Free()
{
	__super::Free();

}