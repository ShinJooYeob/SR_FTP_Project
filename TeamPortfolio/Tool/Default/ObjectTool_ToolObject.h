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

public: // For. Transform
	HRESULT Set_WorldMat(_Matrix world);
	HRESULT Set_Default(wstring name);

	HRESULT Set_Scaled(_float3 scale);
	HRESULT Set_Rotation(_float3 rot);
	HRESULT Set_Position(_float3 Position);

	_float3 Get_Pos() { return m_ComTransform->Get_MatrixState(CTransform::STATE_POS); }
	_float3 Get_Scale() { return m_ComTransform->Get_MatrixScale(); }

public:	// For. Texture
	HRESULT Set_TextureNum_Bind(int num = 0);

	void	Set_Visble(_bool b) { m_isVisble = b; }

	HRESULT Texture_CurrentBind();

public: // For. OutputData , 로드랑 세이브시에만 사용

	void Set_OUTPUTData_Save();
	const OUTPUT_OBJECTINFO& Get_OutputData() const { return m_tOutputData; }
	void LoadData(const OUTPUT_OBJECTINFO& data);
	void Set_NewName(const _tchar* newname);

private:
	HRESULT			SetUp_Components();
	HRESULT			Set_ViBuffer_Change(); // 나중에 다시 쓸 듯 

private:
	_bool					m_isRect = true;
	_bool					m_isCubeRot = false;
	_bool					m_isVisble = true; // 랜더링 유무



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