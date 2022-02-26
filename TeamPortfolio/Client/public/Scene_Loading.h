#pragma once

#include "Scene.h"

BEGIN(Client)
class CScene_Loading final : public CScene
{
public:
	explicit CScene_Loading(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual ~CScene_Loading() = default;


public:
	virtual HRESULT Initialize(SCENEID eSceneID);
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	HRESULT Ready_Layer_Loading(const _tchar* pLayerTag);
	HRESULT Ready_Layer_LoadingBar(const _tchar* pLayerTag);

	HRESULT Ready_Layer_MainCamera(const _tchar* pLayerTag);



private:
	SCENEID m_eNextSceneIndex = SCENEID::SCENE_END;
	class CLoader*		m_pLoader;
	class CUI_LoadingBar* m_pLoadingBar;

public:
	static CScene_Loading* Create(LPDIRECT3DDEVICE9 GraphicDevice, SCENEID eTargetSceneID);
	virtual void Free() override;
};

END
