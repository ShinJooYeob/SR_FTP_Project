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

HRESULT CScene::Scene_InGame_Chage(_bool Scene_Chage_Switch, _uint _INextScene)
{
	m_bScene_Switch = Scene_Chage_Switch;
	m_INextScene = _INextScene;

	return S_OK;
}

void CScene::Free()
{
	Safe_Release(m_pGraphicDevice);
}
