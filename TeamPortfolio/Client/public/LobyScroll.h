#pragma once


#include "UI.h"


BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END


class CLobyScroll final :public CUI
{

private:
	explicit CLobyScroll(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CLobyScroll(const CLobyScroll& rhs);
	virtual ~CLobyScroll() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

	HRESULT First_SetUp_RenderState();
	HRESULT Second_SetUp_RenderState();
	HRESULT Third_SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	void			Set_UI_TransformRect(_float4 vRect);

private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;



private:
	_float4			m_vUIDesc_Frame;
	_float4			m_vUIDesc_IMG;

	_float			m_fStartPoint = 0;
	_float			m_fTargetPoint = 0;
	_float			m_fPassedTime = 0;
	_uint			m_iCount= 0;



private:
	HRESULT			SetUp_Components();

public:
	static CLobyScroll* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};