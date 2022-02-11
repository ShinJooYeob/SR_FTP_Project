#pragma once
#include "Base.h"

BEGIN(Engine)

class CGraphic_Device :public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

public:
	enum SCREENMODE{ TYPE_FULLMODE,	TYPE_SCREEN , TYPE_END};

	typedef struct tagGraphicDesc
	{
		SCREENMODE eScreenMode;
		HWND		hWnd;
		_ulong		iWinCX, iWinCY;
	}GRAPHICDESC;

private:
	explicit CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	LPDIRECT3DDEVICE9			Get_Device(void) { return m_pDevice; }
	LPD3DXSPRITE				Get_Sprite(void) { return m_pSprite; }
	LPD3DXFONT					Get_Font(void) { return m_pFont; }
public:
	HRESULT		InitDevice(const GRAPHICDESC& Graphicdesc, LPDIRECT3DDEVICE9* ppOut);
	void		SetParameters(D3DPRESENT_PARAMETERS& d3dpp, const GRAPHICDESC& Graphicdesc); 

	void		Render_Begin(void);
	void		Render_End();


private:
	LPDIRECT3D9				m_p3D;
	LPDIRECT3DDEVICE9		m_pDevice;

	// dx 상에서 2d 이미지를 출력해주는 com객체
	LPD3DXSPRITE			m_pSprite;
	LPD3DXFONT				m_pFont;

public:
	virtual void	Free() override;
};

END
