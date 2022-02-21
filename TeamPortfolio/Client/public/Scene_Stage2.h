#pragma once

#include "Scene.h"

BEGIN(Client)

class CScene_Stage2 final : public CScene
{
private:
	explicit CScene_Stage2(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual ~CScene_Stage2() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;


private:
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_MainCamera(const _tchar* pLayerTag);

	/////////////////////////ÀºÇõ test
	HRESULT Ready_Layer_Cube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_FixCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_PushCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_GravityCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_SelfRotationCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object_ButtonCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object_InteractiveCube(const _tchar* pLayerTag);
	//////////////////////////////////////////////////////

	D3DLIGHT9		m_Light;

public:
	static CScene_Stage2* Create(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual void Free() override;
};

END