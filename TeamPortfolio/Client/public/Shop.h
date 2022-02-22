#pragma once


#include "UI.h"


BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CShop final :public CUI
{
	typedef struct SKILL_INFO
	{
		_float Move_Speed;
		_float Jump_Power;
		_float Dash_CoolDown;
		_int   Price;
	}SKILLINFO;
private:
	explicit CShop(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CShop(const CShop& rhs);
	virtual ~CShop() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;


public:
	CUI * Find_UI(const _tchar * tagUI);
	HRESULT Ready_Layer_UI_Image(const _tchar * pLayerTag);
	HRESULT Ready_Layer_Button(const _tchar * pLayerTag);



private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CInventory*				m_Player_Inventory = nullptr;



private:	_float			m_fFrame = 0.f;
			SKILLINFO			m_Skill[SKILL_END]; //��ų ������ ��Ƴ��� �迭
			_int				m_iChosenSkill; //���� ������ ��ų�� ���� �˼� �ְ� �̳� �� �ִ� ����
			map<const _tchar*, CUI*>			m_UIButtonList;
			map<const _tchar*, CUI*>			m_UIList;
			map<const _tchar*, CUI*>			m_UIPrototypes;
			bool			m_bIsPress = false;
			_float4			m_vUIDesc;
public:
	HRESULT			Update_UIButtonList(_float fTimeDelta);
	HRESULT			LateUpdate_UIButtonList(_float fTimeDelta);
private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_Skills();

	HRESULT			Set_Skill_Rect();

	HRESULT			Buy_Skill(_int ChosenSkill);


	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CShop* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

