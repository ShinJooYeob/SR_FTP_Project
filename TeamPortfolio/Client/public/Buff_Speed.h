#pragma once

#include "GameObject.h"

BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
END


class CBuff_Speed final :public CGameObject
{
private:
	explicit CBuff_Speed(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CBuff_Speed(const CBuff_Speed& rhs);
	virtual ~CBuff_Speed() = default;

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


private:
	_float			m_fFrame = 0.f;

private:
	HRESULT			SetUp_Components();

	


	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CBuff_Speed* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

