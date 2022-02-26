#pragma once

#include "Base.h"

BEGIN(Client)

class CFontMgr : public CBase
{
	DECLARE_SINGLETON(CFontMgr);

private:
	explicit CFontMgr();
	virtual ~CFontMgr() = default;

public:
	HRESULT Initialize_FontMgr(LPDIRECT3DDEVICE9 pGraphicDevice, const _tchar* szFonttxtFile)

public:
	virtual void Free() override;
};

END