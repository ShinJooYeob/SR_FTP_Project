#pragma once

#include "Scene.h"

BEGIN(Client)
class CScene_IMGUI final : public CScene
{
private:
	explicit CScene_IMGUI(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual ~CScene_IMGUI() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;


private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

private:
	bool mbIsFrame = false;

public:
	static CScene_IMGUI* Create(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual void Free() override;
};

END