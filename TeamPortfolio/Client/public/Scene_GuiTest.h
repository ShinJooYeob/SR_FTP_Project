#pragma once

#include "Scene.h"

BEGIN(Client)
class CScene_GuiTest final : public CScene
{
private:
	explicit CScene_GuiTest(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual ~CScene_GuiTest() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;


private:
	// 씬에서 수행할 IMGUI 클래스 수행
	HRESULT TestGUI();

private:
	bool mPopUp;

public:
	static CScene_GuiTest* Create(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual void Free() override;
};
END
