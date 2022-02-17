#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CTexture;
END

BEGIN(Tool)
class CObjectTool_Rect final :public CGameObject
{
private:
	explicit CObjectTool_Rect(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CObjectTool_Rect(const CObjectTool_Rect& rhs);
	virtual ~CObjectTool_Rect() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

public:

	HRESULT Set_Scaled(_float3 scale);
	HRESULT Set_Position(_float3 Position);
	HRESULT Set_Texture(wstring filename);


private:

	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;

private:
	HRESULT			SetUp_Components();

public:
	static CObjectTool_Rect* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;
};

END