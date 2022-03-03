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
	HRESULT Set_MiniRender();

	HRESULT Set_WorldMat(_Matrix world);
	HRESULT Set_Default(wstring name);

	HRESULT Set_Scaled(_float3 scale);
	HRESULT Set_Rotation(_float3 rot);
	HRESULT Set_Position(_float3 Position);

	HRESULT Set_MyCamDistance() { Compute_CamDistance(m_ComTransform); return S_OK; }
	HRESULT	Set_CubeID(_uint id) { m_tOutputData.CubeID = id; return S_OK; }

	_float3 Get_Pos() { return m_ComTransform->Get_MatrixState(CTransform::STATE_POS); }
	_Matrix Get_Matrix() { return m_ComTransform->Get_WorldMatrix(); }
	_float3 Get_Scale() { return m_ComTransform->Get_MatrixScale(); }

public:	// For. Texture
	HRESULT Set_StateKey_TextureNum_Bind(const _tchar* state, int num = 0);
	void	Set_Visble(_bool b) { m_isVisble = b; }
	HRESULT Texture_CurrentBind();


public: // For. OutputData , 로드랑 세이브시에만 사용

	void Set_OUTPUTData_Save();
	const OUTPUT_OBJECTINFO& Get_OutputData() const { return m_tOutputData; }


	const _tchar* GetName() const {return m_ObjName;}
	void LoadData(const OUTPUT_OBJECTINFO& data);

	_bool PickObject();
	_bool PickObject(_float3* pOut, _float3* pVertex);

	_float3 GetPickPos()
	{
		return m_PickOut;
	}
	_float3* GetPickVertex3()
	{
		/*for (int i=0;i<3;i++)
		{
			D3DXVec3TransformNormal(&m_PickVertex[i], &m_PickVertex[i], &m_ComTransform->Get_WorldMatrix());
		}*/

		return m_PickVertex;
	}


private:
	HRESULT			SetUp_Components();
	HRESULT			Set_ViBuffer_Change(); // 나중에 다시 쓸 듯 

private:
	_bool					m_isRect = true;
	_bool					m_isCubeRot = false;
	_bool					m_isVisble = true; // 랜더링 유무

	_float3					m_PickOut;
	_float3					m_PickVertex[3];

	CTransform*				m_ComTransform = nullptr;
	CVIBuffer*				m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;

	TCHAR					m_ObjName[64];
	OUTPUT_OBJECTINFO		m_tOutputData;


public:
	static CObjectTool_ToolObject* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;
};

END