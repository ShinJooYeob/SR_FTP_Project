#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CTexture;
END

BEGIN(Tool)
class CObjectTool_ToolObject final :public CGameObject
{
private:
	explicit CObjectTool_ToolObject(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CObjectTool_ToolObject(const CObjectTool_ToolObject& rhs);
	virtual ~CObjectTool_ToolObject() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	HRESULT Set_Defult(wstring objectName);
	HRESULT Set_Scaled(_float3 scale);
	HRESULT Set_Rotation(_float3 rot);
	HRESULT Set_Position(_float3 Position);
	HRESULT Set_Texture(wstring pathdata);
	HRESULT Set_TextureNum_Bind(int num = 0);

	HRESULT	Set_NewOutputData(const OUTPUT_OBJECTINFO& data);

	// 각 정보 동기화
	HRESULT	Set_Output2Component();
	HRESULT	Set_Component2Output();



	HRESULT Set_ViBuffer_Change();
	void	Set_Visble(_bool b) { m_isVisble = b; }

	const OUTPUT_OBJECTINFO& Get_ObjectInfo() { Set_Component2Output(); return m_tOutputData; }
	_float3 Get_Pos() { return m_ComTransform->Get_MatrixState(CTransform::STATE_POS);  }
	_float3 Get_Scale() { return m_ComTransform->Get_MatrixScale(); }

	HRESULT Texture_CurrentBind();

private:
	HRESULT			SetUp_Components();
private:
	_bool					m_isRect = true;
	_bool					m_isCubeRot = false;
	_bool					m_isVisble = true; // 랜더링 유무


	_float3					m_Rot3;// 회전값 일단 생략

	CTransform*				m_ComTransform = nullptr;
	CVIBuffer*				m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;

	OUTPUT_OBJECTINFO		m_tOutputData;

	

public:
	static CObjectTool_ToolObject* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;
};

END