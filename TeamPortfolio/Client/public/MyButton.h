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
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;



private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CInventory*				m_Player_Inventory = nullptr;



private:	
	_float			m_fFrame = 0.f;
	
public:
	void			Set_ButtonName(TCHAR* pButtonName);
private:
	HRESULT			SetUp_Components();
	HRESULT			Buy_Skill(_int eSKILL);
	SKILL			m_eSkill;

public:
	static CMyButton* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

