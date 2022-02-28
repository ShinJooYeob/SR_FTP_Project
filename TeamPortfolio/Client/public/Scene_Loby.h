#pragma once

#include "Scene.h"

BEGIN(Client)
class CScene_Loby final : public CScene
{
private:
	explicit CScene_Loby(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual ~CScene_Loby() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;



private:
	HRESULT Ready_Layer_SkyBox(const _tchar* pLayerTag);
	HRESULT Ready_Layer_LobyCube(const _tchar* pLayerTag);
	HRESULT Ready_Layer_LobyPlayer(const _tchar* pLayerTag);
	HRESULT Ready_Layer_LobyScroll(const _tchar* pLayerTag);
	HRESULT Ready_Layer_LobyUI(const _tchar* pLayerTag);

	//HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	//HRESULT Ready_Layer_UI_Loby(const _tchar * pLayerTag);
	HRESULT Ready_Layer_MainCamera(const _tchar* pLayerTag);

private:
	class CCamera_Main*				m_pMainCam = nullptr;
	class CLoby_UI*				m_pUI = nullptr;
	_float ChangeTime = 0;
public:
	static CScene_Loby* Create(LPDIRECT3DDEVICE9 GraphicDevice);
	virtual void Free() override;
};

END