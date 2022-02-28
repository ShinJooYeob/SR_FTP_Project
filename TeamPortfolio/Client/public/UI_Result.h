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

public:
	void		Button_Picking();

	void		Set_Clear(_bool _bClear);

	//진우형 프레임워크
private:
	HRESULT		Ready_Layer_Button(const _tchar * pLayerTag);//버튼 클론을 만들고 있음
	HRESULT		Update_UIButtonList(_float fTimeDelta); //이게 버튼의 업데이트를 돌려주고 있다.
	HRESULT		LateUpdate_UIButtonList(_float fTimeDelta); //이게 버튼의 레이트업데이트를 돌려주고 있다.
	CUI *		Find_UI(const _tchar * tagUI); //UI 겹치는게 있는지 체크
	CUI *		Find_Button(const _tchar * tagUIList); //이게 클론을 찾음

	HRESULT		Ready_Layer_RankStar(const _tchar * pLayerTag);

private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;

private:
	map<const _tchar*, CUI*>			m_UIButtonList;
	map<const _tchar*, CUI*>			m_UIPrototypes;

	_float			m_fFrame = 0.f;
	_float4			m_vUIDesc; //결과창용

	_float4			m_vButtonDesc; // 버튼용
	RECT			m_fStartButton;
	RECT			m_fCancelButton;

	_bool			m_isClicked = false;

	_bool			m_bStopSwitch = false; //타이며 멈춤 역할
	_float			m_fTimer = 0.f; // 타이머
	_float			m_fMaxTime = 0.f; // 최대 시간
	_bool			m_bClear = false; //클리어 여부
	_float			TempMinutes = 0.f; //몇분인지 담는중
	_float			TempSeconds = 0.f; //몇초인지 담는중



public:
	static CUI_Result* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;
};

END