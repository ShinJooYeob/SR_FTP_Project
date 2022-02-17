#include "..\Public\Picking.h"

CPicking::CPicking()
{
}

HRESULT CPicking::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd, void * pArg)
{
	m_hWnd = hWnd;
	m_pGraphic_Device = pGraphic_Device;
	return S_OK;
}

_bool CPicking::is_On_Rect(const RECT* TargetRect)
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);
	
	return PtInRect(TargetRect, ptMouse);
}



void CPicking::Free()
{

}
