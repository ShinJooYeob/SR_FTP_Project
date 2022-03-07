#pragma once


#include "UI.h"


BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CUI_Mouse final :public CUI
{
private:
	explicit CUI_Mouse(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_Mouse(const CUI_Mouse& rhs);
	virtual ~CUI_Mouse() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;



private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;



private:
	_float			m_fFrame = 0.f;
	_float			m_fTime = 0.f;
	_bool			m_bTimeStart = false;
	TCHAR*			m_pImageName;
	MOUSETYPEID		m_eMouseType = MOUSE_END;
public:
	void			Set_MouseID(MOUSETYPEID eMOUSETYPEID) { m_eMouseType= eMOUSETYPEID; }
private:
	HRESULT			SetUp_Components();

public:
	static CUI_Mouse* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

