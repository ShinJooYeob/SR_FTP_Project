#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CPicking : public CBase
{
private:
	explicit CPicking();
	virtual ~CPicking() = default;

public:
	 HRESULT Initialize(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd,void * pArg);
	 _bool CPicking::is_On_Rect(const RECT* TargetRect);
private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	HWND					m_hWnd;
public:
	
	virtual void Free() override;


};

END