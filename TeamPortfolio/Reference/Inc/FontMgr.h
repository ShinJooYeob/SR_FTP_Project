#pragma once

#include "Base.h"

BEGIN(Engine)

class CFontMgr : public CBase
{
	DECLARE_SINGLETON(CFontMgr);

private:
	explicit CFontMgr();
	virtual ~CFontMgr() = default;

public:
	HRESULT Initialize_FontMgr(LPDIRECT3DDEVICE9 pGraphicDevice, const _tchar* szFonttxtFile, _float2 WindowSize);

	HRESULT Render_UI_Font(wstring szString, _float2 vOnWindowPos, _float2 vFontSize = _float2(20.f, 30.f), _float3 Color_RGB = { 0,0,0 }, _int UntilDrawIndex = -1);
	HRESULT Render_World_Font(wstring szString, _float3 vOnWorldPos, _float2 vFontSize = _float2(0.3f, 0.45f), _float3 Color_RGB = { 0,0,0 }, _int UntilDrawIndex = -1);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDevice = nullptr;
	class CVIBuffer_Rect*	m_VIBuffer = nullptr;
	class CTexture*			m_texFont = nullptr;
	_float2					m_vWindowSize;


public:
	virtual void Free() override;
};

END