#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBuffer;
class CTransform;
class CTexture;
END

BEGIN(Tool)
// 툴에서 그림자 셰이더 테스트용
// 나중에 
class CObjectTool_ShaderTestObject final :public CGameObject
{
private:
	explicit CObjectTool_ShaderTestObject(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CObjectTool_ShaderTestObject(const CObjectTool_ShaderTestObject& rhs);
	virtual ~CObjectTool_ShaderTestObject() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

public: // For. Transform
	HRESULT Set_WorldMat(_Matrix world);
	HRESULT Set_Default(wstring name);

	HRESULT Set_Scaled(_float3 scale);
	HRESULT Set_Rotation(_float3 rot);
	HRESULT Set_Position(_float3 Position);



	//_float3 Get_Pos() { return m_ComTransform->Get_MatrixState(CTransform::STATE_POS); }
	//_Matrix Get_Matrix() { return m_ComTransform->Get_WorldMatrix(); }
	//_float3 Get_Scale() { return m_ComTransform->Get_MatrixScale(); }

public:	// For. Texture
	HRESULT Set_StateKey_TextureNum_Bind(const _tchar* state, int num = 0);

private:
	HRESULT			SetUp_Components();
	HRESULT			Set_ViBuffer_Change(); // 나중에 다시 쓸 듯 

private:

	CTransform*				m_ComTransform = nullptr;
	CVIBuffer*				m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;




public:
	static CObjectTool_ShaderTestObject* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;
};

END