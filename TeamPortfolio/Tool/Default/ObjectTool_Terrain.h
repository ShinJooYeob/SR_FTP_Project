#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBuffer;
class CVIBuffer_Terrain;
class CTransform;
class CTexture;
END

BEGIN(Tool)


class CObjectTool_Terrain final :public CGameObject
{
private:
	explicit CObjectTool_Terrain(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CObjectTool_Terrain(const CObjectTool_Terrain& rhs);
	virtual ~CObjectTool_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;


private: 
	HRESULT RenderState_Begin();
	HRESULT RenderState_End();


private:
	HRESULT			SetUp_Components();


private:
//	_bool					m_isVisble = true; // 랜더링 유무

	CTransform*				m_ComTransform = nullptr;
	CVIBuffer*				m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;


public:
	static CObjectTool_Terrain* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;
};

END