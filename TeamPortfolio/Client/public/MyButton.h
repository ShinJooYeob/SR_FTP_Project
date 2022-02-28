#pragma once


#include "UI.h"


BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CMyButton final :public CUI
{

private:
	explicit CMyButton(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CMyButton(const CMyButton& rhs);
	virtual ~CMyButton() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	virtual _int LateUpdate(_float fDeltaTime)override;
	void	Set_Rect(_float4 RectPos, RECT fRect);

	virtual _int Render()override;
	virtual _int LateRender()override;



private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;




private:
	_float			m_fFrame = 0.f;
	_bool			m_isClicked = false;
	TCHAR*			m_pButtonName;
	_float4			m_vUIDesc;
	_bool			m_bBigger;
	_bool			m_bOnMouse;

public:
	void			Set_ButtonName(TCHAR* pButtonName);
private:
	HRESULT			SetUp_Components();

public:
	static CMyButton* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

