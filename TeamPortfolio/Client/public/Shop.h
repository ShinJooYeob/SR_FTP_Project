#pragma once

#include "GameObject.h"
#include "Item.h"

BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CShop final :public CGameObject
{
	typedef struct SKILL_INFO
	{
		_float Move_Speed;
		_float Jump_Power;
		_float Dash_CoolDown;
		_int   Price;
	}SKILL;
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



private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CInventory*				m_Player_Inventory = nullptr;
	
	

private:
	_float			m_fFrame = 0.f;
	SKILL			m_Skill[SKILL_END];//스킬 정보를 담아놓는 배열
public:
	
private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_Skills();

	HRESULT			Buy_Skill(_int eSKILL);
	

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CShop* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

