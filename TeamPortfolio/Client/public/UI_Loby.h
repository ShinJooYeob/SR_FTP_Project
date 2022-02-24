#pragma once


#include "UI.h"


BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
END


class CUI_Loby final :public CUI
{
private:
	explicit CUI_Loby(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_Loby(const CUI_Loby& rhs);
	virtual ~CUI_Loby() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;

	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;


public:

	CUI * Find_UI(const _tchar * tagUI);
	CUI * Find_Image(const _tchar * tagUIList);
	HRESULT Ready_Layer_UI_Image(const _tchar * pLayerTag);

	HRESULT Ready_Layer_Button(const _tchar * pLayerTag);



private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;




private:	_float			m_fFrame = 0.f;
			map<const _tchar*, CUI*>			m_UIButtonList;
			map<const _tchar*, CUI*>			m_UIList;
			map<const _tchar*, CUI*>			m_UIPrototypes;
			_float4								m_vUIDesc;
			_bool								m_bIsPress=false;
public:
	HRESULT			Update_UIButtonList(_float fTimeDelta);
	HRESULT			Update_UIList(_float fTimeDelta);


	HRESULT			LateUpdate_UIList(_float fTimeDelta);
	HRESULT			LateUpdate_UIButtonList(_float fTimeDelta);
private:
	HRESULT			SetUp_Components();



	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CUI_Loby* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

