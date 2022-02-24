#pragma once

#include "Scene.h"

BEGIN(Client)
class CScene_StageSelect final : public CScene
{
private:
	explicit CScene_StageSelect(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual ~CScene_StageSelect() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;


private:
	HRESULT Ready_Layer_SkyBox(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_MainCamera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Shop(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Quest(const _tchar * pLayerTag);
	HRESULT Ready_Layer_UI_Common(const _tchar * pLayerTag);

	HRESULT Ready_Layer_GravityCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_JumpCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object_PortalCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_ElevetorCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_OrbitButton_And_Cube(const _tchar* pLayerTag);



	D3DLIGHT9		m_Light;

public:
	static CScene_StageSelect* Create(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual void Free() override;
};
END
