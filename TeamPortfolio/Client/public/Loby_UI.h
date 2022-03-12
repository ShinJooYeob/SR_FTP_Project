#pragma once


#include "UI.h"


BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CLoby_UI final :public CUI
{
private:
	explicit CLoby_UI(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CLoby_UI(const CLoby_UI& rhs);
	virtual ~CLoby_UI() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;


	virtual _int Render()override;
	virtual _int LateRender()override;
	_bool	Get_IsSceneChange() {	return m_IsSceneChange;	};


	HRESULT First_SetUp_RenderState();
	HRESULT Second_SetUp_RenderState();
	HRESULT Thrid_SetUp_RenderState();
	HRESULT Release_RenderState();

	void	Set_Particle_LoginFailed();

private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;

	class CLobyCube*			m_pLobyCube = nullptr;


private:
	_float4			m_vUIDesc_Logo;
	_float4			m_vUIDesc_Index;

	_float4			m_vUIDesc_FirstSound;
	_float4			m_vUIDesc_SecondSound;

	_bool			m_bIsFirstSoundClicked = false;
	_bool			m_bIsSecondSoundClicked = false;

	_float			m_fAlphaValue = 0;
	_float			m_fTextFrame = 0;
	

	_float			m_fIndexAlpha = 255.f;

	_uint			m_iManuIndex = 0;
	_int			m_iPageIndex = 0;
	_bool			m_IsSceneChange = false;

	_bool			m_bIsNewResgist = false;

	wstring			m_szID = L"";
	wstring			m_szPassword = L"";

	_bool			m_bIDInput = false;
	_bool			m_bPasswordInput = false;

public:
	//vRect.x,y,z,w=top,left,bottom,right
	void			Set_UI_TransformRect(_float4 vRect);

private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_UIDesc();
	HRESULT			Update_MouseButton(_float fTimeDelta);
	HRESULT			Update_Alpha(_float fTimeDelta);
	HRESULT			Input_ManuIndex(_float fTimeDelta);

	HRESULT			String_Input(wstring& wString);

private:
	RECT			TransUIDesc_to_Rect(_float4 UIDesc);

public:
	static CLoby_UI* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};