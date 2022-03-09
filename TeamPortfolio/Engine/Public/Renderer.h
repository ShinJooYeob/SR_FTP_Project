#pragma once

#include "Component.h"


BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CRenderer final :public CComponent
{
public:
	enum RENDERGROUP 
	{
		RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_AFTEROBJ, RENDER_UI, RENDER_END
	};

private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual ~CRenderer() = default;

private:
	virtual HRESULT Initialize_Prototype(void * pArg)override;
	virtual HRESULT Initialize_Clone(void * pArg)override;


public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderID ,CGameObject* pGameObject);
	HRESULT Render_RenderGroup();

	HRESULT Add_MainCamemra(class CCamera* pCCamera);

	LPDIRECT3DTEXTURE9 Get_MiniMapTex() { return m_pMinmapTex; }

private:
	list<CGameObject*>			m_RenderObjectList[RENDER_END];
	typedef list<CGameObject*>		RENDEROBJECTS;
private:

	_Matrix						m_ProjMatrix;
	class CCamera*				m_MainCamera = nullptr;




	LPDIRECT3DTEXTURE9			m_pMinmapTex = nullptr;
	LPDIRECT3DSURFACE9			m_pMinmapSurf = nullptr;
	LPDIRECT3DSURFACE9			m_pMinmapTexZ = nullptr;

private:
	HRESULT Update_MinmapTexture();

	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_AfterObj();
	HRESULT Render_UI();

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END	