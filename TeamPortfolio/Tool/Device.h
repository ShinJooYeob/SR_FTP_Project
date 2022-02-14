#pragma once

#include "Base.h"
BEGIN(Tool)
class CDevice : public CBase
{
	DECLARE_SINGLETON(CDevice)

private:
	explicit CDevice();
	virtual ~CDevice() = default;
	// com 객체 : 언어와의 통신 규약을 갖고 하드웨어를 통제하여 각종 기능을 제공하는 객체

public:
	CGameInstance*				Get_GameInstance() { return m_GameInstance; }
	LPDIRECT3DDEVICE9			Get_Device(void) { return m_pDevice; }
	LPD3DXSPRITE				Get_Sprite(void) { return m_pSprite; }
	LPD3DXFONT					Get_Font(void) { return m_pFont; }
public:
	HRESULT		InitDevice(void);

	void		Render_Begin(void);
	void		Render_End(HWND hWnd = NULL);

	virtual		void	Free()override;
private:
	LPDIRECT3D9				m_p3D;
	LPDIRECT3DDEVICE9		m_pDevice;

	class CGameInstance*			m_GameInstance;

	// 기타 COM 객체
	LPD3DXSPRITE			m_pSprite;
	LPD3DXFONT				m_pFont;
};

END