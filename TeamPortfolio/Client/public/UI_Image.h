#pragma once


#include "UI.h"


BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CUI_Image final :public CUI
{
public:
	enum BIGGER
	{
		BIGGER_DEFAULT, BIGGER_ON, BIGGER_OFF, BIGGER_END
	};

private:
	explicit CUI_Image(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_Image(const CUI_Image& rhs);
	virtual ~CUI_Image() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	
	virtual _int Update(_float fDeltaTime)override;
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;



private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;



private:
	_float			m_fFrame = 0.f;
	TCHAR*			m_pImageName;
	_bool			m_bRender = true;
	_int			m_iBigger = BIGGER_DEFAULT;
	_float4			m_vUIDesc;
	_float			m_fSeconds = 0;
	_int			m_iAlpha = 255;

	_bool			m_bEasingStart=false;
	_float4			m_vEasingDesc;//x 스타트 y타겟 z패스드타임 w총시간
public:
	RECT			Get_Rect() const{ return m_rcRect; }
	_int			Get_ImageBigger() { return m_iBigger; }

	void			Set_ImageUIDescSizeY(_float vUIDescSizeY){ m_vUIDesc.w = m_vUIDesc.w + vUIDescSizeY; }
	void			Set_ImageUIDescX(_float vUIDescx) { m_vUIDesc.x = m_vUIDesc.x + vUIDescx; }
	void			Set_ImageName(TCHAR* pImageName);
	void			Set_ImageRenderFalse() { m_bRender = false; }
	void			Set_ImageRenderTrue() { m_bRender = true; }
	void			Set_ImageBigger(BIGGER eBigger) { m_iBigger = eBigger; m_bEasingStart = false; }
	void			Set_ImageAlpha(_int Alpha) { m_iAlpha = Alpha; }
	//vRect.x,y,z,w=top,left,bottom,right
	void			Set_UI_TransformRect(_float4 vRect);

private:
	HRESULT			SetUp_Components();

public:
	static CUI_Image* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};