#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBuffer_Cube;
class CTransform;
class CTexture;
END

BEGIN(Tool)
// 맵 큐브가 생성되기 전 보여주는 와이어 큐브

class CObjectTool_ToolWire final :public CGameObject
{
private:
	explicit CObjectTool_ToolWire(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CObjectTool_ToolWire(const CObjectTool_ToolWire& rhs);
	virtual ~CObjectTool_ToolWire() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	HRESULT Set_Pos(_float3 pos);
	HRESULT Set_TransformMat(_Matrix m);

public:


private:
	HRESULT			RenderState_Begin();
	HRESULT			RenderState_End();
	HRESULT			SetUp_Components();



private:
	// bool					m_isVisble;
	int						m_TextureNum;

	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Cube*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;


public:
	static CObjectTool_ToolWire* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;
};

END