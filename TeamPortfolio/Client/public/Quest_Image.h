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
	typedef struct SKILL_INFO
	{
		_float Move_Speed;
		_float Jump_Power;
		_float Dash_CoolDown;
		_int   Price;
	}SKILLINFO;
private:
	explicit CQuest_Image(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CQuest_Image(const CQuest_Image& rhs);
	virtual ~CQuest_Image() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;
	_int Manual_Render(_int eSkill);
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;


public:
	CUI * Find_UI(const _tchar * tagUI);
	CUI * Find_Image(const _tchar * tagUIList);
	HRESULT Ready_Layer_UI_Image(const _tchar * pLayerTag);
	HRESULT Set_Image_Render(const _tchar * tagUIList, _bool bCheck);
	HRESULT Ready_Layer_Button(const _tchar * pLayerTag);



private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CInventory*				m_Player_Inventory = nullptr;



private:	_float			m_fFrame = 0.f;
			SKILLINFO			m_Skill[SKILL_END]; //스킬 정보를 담아놓는 배열
			_int				m_iChosenSkill; //현재 선택한 스킬이 뭔지 알수 있게 이넘 값 넣는 변수
			map<const _tchar*, CUI*>			m_UIButtonList;
			map<const _tchar*, CUI*>			m_UIList;
			map<const _tchar*, CUI*>			m_UIPrototypes;
			bool			m_bIsPress = false;
			_float4			m_vUIDesc;
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
	static CQuest_Image* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

