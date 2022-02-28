#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CScene abstract :public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene() = default;

public:
	virtual HRESULT Initialize();
	virtual _int Update(_float fDeltaTime);
	virtual _int LateUpdate(_float fDeltaTime);
	virtual _int Render();
	virtual _int LateRender();

	//�������� �ǵ�
	virtual	HRESULT Scene_InGame_Chage(_bool Scene_Chage_Switch, _uint _INextScene) { return S_OK; };

protected:
	LPDIRECT3DDEVICE9 m_pGraphicDevice = nullptr;

	//�������� �ǵ�
	_bool			m_bScene_Switch = false;
	_uint			m_INextScene;
public:
	virtual void Free()override;
};

END