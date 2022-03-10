#pragma once


#include "UI.h"


BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CQuest_Image final :public CUI
{

private:
	explicit CQuest_Image(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CQuest_Image(const CQuest_Image& rhs);
	virtual ~CQuest_Image() = default;

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
	HRESULT Set_Image_Render(const _tchar * tagUIList, _bool bCheck);
	HRESULT Ready_Layer_Button(const _tchar * pLayerTag);
	HRESULT Make_Bigger(const _tchar * pLayerTag);


private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CInventory*				m_Player_Inventory = nullptr;



private:	_float			m_fFrame = 0.f;
			map<const _tchar*, CUI*>			m_UIButtonList;
			map<const _tchar*, CUI*>			m_UIList;
			map<const _tchar*, CUI*>			m_UIPrototypes;
			bool			m_bIsPress = false;
			_float4			m_vUIDesc[2];
			_tchar*			m_BiggerTag=nullptr;
			_bool			m_bQuestBiggerOn=false;
			_bool			m_bBiggerStart = true;
			_float			m_fTextFrame = 0;
			_uint			m_iAlpha = 0;
public:
	HRESULT			Update_UIButtonList(_float fTimeDelta);
	HRESULT			Update_UIList(_float fTimeDelta);


	HRESULT			LateUpdate_UIList(_float fTimeDelta);
	HRESULT			LateUpdate_UIButtonList(_float fTimeDelta);
private:
	HRESULT			SetUp_Components();



	HRESULT SetUp_FirstRenderState();
	
	HRESULT Release_RenderState();

public:
	static CQuest_Image* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

