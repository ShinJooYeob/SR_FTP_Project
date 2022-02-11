#include "Scene.h"




CScene::CScene(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphicDevice(pGraphic_Device)
{
	Safe_AddRef(pGraphic_Device);
}

HRESULT CScene::Initialize()
{
	return S_OK;
}

_int CScene::Update(_float fDeltaTime)
{
	return _int();
}

_int CScene::LateUpdate(_float fDeltaTime)
{
	return _int();
}

_int CScene::Render()
{
	return _int();
}

_int CScene::LateRender()
{
	return _int();
}

void CScene::Free()
{
	Safe_Release(m_pGraphicDevice);
}
