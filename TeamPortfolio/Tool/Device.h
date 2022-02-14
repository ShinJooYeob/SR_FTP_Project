#pragma once

#include "Base.h"
BEGIN(Tool)
class CDevice : public CBase
{
	DECLARE_SINGLETON(CDevice)

private:
	explicit CDevice();
	virtual ~CDevice() = default;
	// com ��ü : ������ ��� �Ծ��� ���� �ϵ��� �����Ͽ� ���� ����� �����ϴ� ��ü

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

	// ��Ÿ COM ��ü
	LPD3DXSPRITE			m_pSprite;
	LPD3DXFONT				m_pFont;
};

END