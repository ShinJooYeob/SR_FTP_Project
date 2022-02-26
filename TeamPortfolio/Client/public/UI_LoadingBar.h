#pragma once

#include "UI.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CUI_LoadingBar final :public CUI
{
public:
	typedef struct tagLoadingBar {
		_float ProgressBar;
		_float MaxBar;
	}LOADINGBAR;

private:
	explicit CUI_LoadingBar(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_LoadingBar(const CUI_LoadingBar& rhs);
	virtual ~CUI_LoadingBar() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;

	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

	_float4 Set_byRectPos(_float4 tRect);

	void Set_Progress(_float _ProgressCount,_float _MaxCount);


private:
	HRESULT			SetUp_Components();

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;

private:
	_float			m_fFrame = 0.f;
	_float4			m_vUIDesc;

	_float			WinPosX;
	_float			WinPosY;
	_float			WinCX;
	_float			WinCY;

	_float4			vRect;

public:
	static CUI_LoadingBar* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;
};

END