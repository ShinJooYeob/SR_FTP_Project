#pragma once


#include "UI.h"


BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CUI_Common final :public CUI
{

private:
	explicit CUI_Common(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_Common(const CUI_Common& rhs);
	virtual ~CUI_Common() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;

	HRESULT Show_UI(_float fDeltaTime);

	HRESULT Hide_UI(_float fDeltaTime);

	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;


public:

	CUI * Find_UI(const _tchar * tagUI);
	CUI * Find_Image(const _tchar * tagUIList);
	HRESULT Ready_Layer_UI_Image(const _tchar * pLayerTag);
	HRESULT Set_Image_Render(const _tchar * tagUIList, _bool bCheck);
	HRESULT Ready_Layer_Button(const _tchar * pLayerTag);
	HRESULT Make_Bigger(const _tchar * pLayerTag);

	void Set_SkillSlot();


private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CInventory*				m_Player_Inventory = nullptr;



private:	_float			m_fFrame = 0.f;
			_float			m_temp = 0.f;
			map<const _tchar*, CUI*>			m_UIButtonList;
			map<const _tchar*, CUI*>			m_UIList;
			map<const _tchar*, CUI*>			m_UIPrototypes;
			_bool			m_bIsPress = false;
			_bool			m_MouseOn = false;
			_float4			m_vUIDesc;
			_tchar*			m_BiggerTag;
			_float			m_fPosX = -40.f;
			_bool			m_bIsClicked=false;
			
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
	static CUI_Common* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

