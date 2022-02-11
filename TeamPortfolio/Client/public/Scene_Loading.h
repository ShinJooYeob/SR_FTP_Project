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

private:
	SCENEID m_eNextSceneIndex = SCENEID::SCENE_END;
	class CLoader*		m_pLoader;

public:
	static CScene_Loading* Create(LPDIRECT3DDEVICE9 GraphicDevice, SCENEID eTargetSceneID);
	virtual void Free() override;
};

END
