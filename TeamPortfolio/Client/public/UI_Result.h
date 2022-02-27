#pragma once

#include "UI.h"


BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CUI_Result final :public CUI
{
private:
	explicit CUI_Result(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_Result(const CUI_Result& rhs);
	virtual ~CUI_Result() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;

	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

private:
	HRESULT		SetUp_Components();
	HRESULT		SetUp_RenderState();
	HRESULT		Release_RenderState();

	//진우형 프레임워크
private:
	HRESULT		Ready_Layer_Button(const _tchar * pLayerTag);//버튼 클론을 만들고 있음
	HRESULT		Update_UIButtonList(_float fTimeDelta); //이게 버튼의 업데이트를 돌려주고 있다.
	HRESULT		LateUpdate_UIButtonList(_float fTimeDelta); //이게 버튼의 레이트업데이트를 돌려주고 있다.
	CUI *		Find_UI(const _tchar * tagUI); //UI 겹치는게 있는지 체크
	CUI *		Find_Button(const _tchar * tagUIList); //이게 클론을 찾음

private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;

private:
	map<const _tchar*, CUI*>			m_UIButtonList;
	map<const _tchar*, CUI*>			m_UIPrototypes;

	_float			m_fFrame = 0.f;
	_float4			m_vUIDesc;

	_bool			m_bStopSwitch = false;
	_float			m_fTimer = 0.f;

public:
	static CUI_Result* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;
};

END