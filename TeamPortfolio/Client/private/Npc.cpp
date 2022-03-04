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
	return E_NOTIMPL;
}

HRESULT CNpc::Initialize_Clone(void * pArg)
{
	return E_NOTIMPL;
}
