#include "stdafx.h"
#include "ObjectTool_ToolObject.h"
#include "VIBuffer.h"
#include "Picking.h"

CObjectTool_ToolObject::CObjectTool_ToolObject(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice),
	m_ComRenderer(nullptr),
	m_ComVIBuffer(nullptr),
	m_ComTransform(nullptr),
	m_ComTexture(nullptr),
	m_ObjName(L"")
{
	m_isVisble = true;

}

CObjectTool_ToolObject::CObjectTool_ToolObject(const CObjectTool_ToolObject & rhs)
	:CGameObject(rhs)
{
}

HRESULT CObjectTool_ToolObject::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CObjectTool_ToolObject::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int CObjectTool_ToolObject::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	return _int();
}

_int CObjectTool_ToolObject::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;

	//렌더링 그룹에 넣어주는 역활
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return 0;
}

HRESULT CObjectTool_ToolObject::Set_MiniRender()
{
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;
	return S_OK;
}



_int CObjectTool_ToolObject::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (!m_isVisble)
		return 0;

	NULL_CHECK_BREAK(m_ComTransform);
	NULL_CHECK_BREAK(m_ComTexture);
	NULL_CHECK_BREAK(m_ComVIBuffer);

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(m_tOutputData.TexDesc.StateIndex)))
		return E_FAIL;

	//렌더링 그룹에 들어가면 순서에 맞게 이 랜더가 호출되고 호출이 됬으면 버텍스 버퍼를 그려줘라
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	return 0;
}

_int CObjectTool_ToolObject::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;
	return 0;
}

HRESULT CObjectTool_ToolObject::Set_Default(wstring objectName)
{	
	OUTPUT_OBJECTINFO info;	
	m_tOutputData = info;
	lstrcpy(m_ObjName, objectName.c_str());
	m_tOutputData.TexDesc.eTextureType = (_uint)m_ComTexture->Get_TextureDESC().eTextureType;
	lstrcpy(m_tOutputData.TexDesc.szTextFilePath, m_ComTexture->Get_TextureDESC().szTextFilePath);

	return S_OK;
}

HRESULT CObjectTool_ToolObject::Set_WorldMat(_Matrix world)
{
	m_ComTransform->Set_Matrix(world);
	return S_OK;
}

HRESULT CObjectTool_ToolObject::Set_Scaled(_float3 scale)
{
	NULL_CHECK_BREAK(m_ComTransform);

	if (scale.x == 0)
		scale.x = 1.f;

	if (scale.y == 0)
		scale.y = 1.f;

	if (scale.z == 0)
		scale.z = 1.f;

	m_ComTransform->Scaled(scale);
	return S_OK;
}

HRESULT CObjectTool_ToolObject::Set_Rotation(_float3 rot)
{
	NULL_CHECK_BREAK(m_ComTransform);

	D3DXQUATERNION quat_x, quat_y, quat_z, quat_1, quat_2;
	_Matrix matrix;
	D3DXVECTOR3 axis_x(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 axis_y(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 axis_z(0.0f, 0.0f, 1.0f);
	D3DXQuaternionRotationAxis(&quat_x, &axis_x, rot.x);
	D3DXQuaternionRotationAxis(&quat_y, &axis_y, rot.y);
	D3DXQuaternionRotationAxis(&quat_z, &axis_z, rot.z);

	quat_1 = quat_y * quat_x * quat_z;
	D3DXQuaternionNormalize(&quat_2, &quat_1);
	D3DXMatrixRotationQuaternion(&matrix, &quat_2);

	m_ComTransform->Set_MatrixState(CTransform::STATE_RIGHT, *(_float3*)&matrix.m[0]);
	m_ComTransform->Set_MatrixState(CTransform::STATE_UP, *(_float3*)&matrix.m[1]);
	m_ComTransform->Set_MatrixState(CTransform::STATE_LOOK, *(_float3*)&matrix.m[2]);

	return S_OK;
}

HRESULT CObjectTool_ToolObject::Set_Position(_float3 Position)
{
	NULL_CHECK_BREAK(m_ComTransform);

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, Position);
	return S_OK;
}

HRESULT CObjectTool_ToolObject::Set_StateKey_TextureNum_Bind(const _tchar * state, int num)
{
	NULL_CHECK_BREAK(m_ComTexture);

	m_tOutputData.TexDesc.StateIndex = num;
	lstrcpy(m_tOutputData.TexDesc.szStateKey,state);
	m_ComTexture->Change_TextureLayer(state,num);

	return S_OK;
}

HRESULT CObjectTool_ToolObject::Set_ViBuffer_Change()
{
	return S_OK;

	m_isRect = !m_isRect;
	Safe_Release(m_ComVIBuffer);

	if (m_isRect)
	{
		FAILED_CHECK(__super::Change_Component(
			SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), 
			TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer));

		
		FAILED_CHECK(__super::Change_Component(
			SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Default),
			TAG_COM(Com_Texture), (CComponent**)&m_ComTexture));
	
		m_ComTexture->Bind_Texture(0);

	}
	else
	{
		FAILED_CHECK(__super::Change_Component(
			SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube),
			TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer));


		FAILED_CHECK(__super::Change_Component(
			SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Cube),
			TAG_COM(Com_Texture), (CComponent**)&m_ComTexture));

		m_ComTexture->Bind_Texture(0);
	}
	return S_OK;
}


HRESULT CObjectTool_ToolObject::Texture_CurrentBind()
{
	NULL_CHECK_BREAK(m_ComTexture);
	m_ComTexture->Bind_Texture(m_tOutputData.TexDesc.StateIndex);
	return S_OK;
}

void CObjectTool_ToolObject::Set_OUTPUTData_Save()
{
	// 외부 저장전에 한번 호출
	// 현재 모든 데이터를 OUTPUT 데이터로 옮김

	// Trans
	m_tOutputData.WorldMatData = m_ComTransform->Get_WorldMatrix();
	
	// Texture
	m_tOutputData.TexDesc.eTextureType = m_ComTexture->Get_TextureDESC().eTextureType;
	lstrcpy(m_tOutputData.TexDesc.szTextFilePath, m_ComTexture->Get_TextureDESC().szTextFilePath);
}

void CObjectTool_ToolObject::LoadData(const OUTPUT_OBJECTINFO& data)
{
	// OUTPUT 데이터를 받고 모든 컴포넌트에 업로드 한다.
	m_tOutputData = data;

	// For Transform	
	Set_WorldMat(m_tOutputData.WorldMatData);

	// For Texture
	Set_StateKey_TextureNum_Bind(m_tOutputData.TexDesc.szStateKey, m_tOutputData.TexDesc.StateIndex);

}


bool CObjectTool_ToolObject::PickObject()
{
	_float2 uv;
	if (true == m_ComVIBuffer->Pick(m_ComTransform->Get_InverseWorldMatrix(), &m_PickOut, &uv, m_PickVertex))
		return true;
	
	return false;
}
_bool CObjectTool_ToolObject::PickObject(_float3 * pOut, _float3 * pVertex)
{
	_float2 uv;
	if (true == m_ComVIBuffer->Pick(m_ComTransform->Get_InverseWorldMatrix(), pOut, &uv,pVertex))
		return true;

	return false;
}


HRESULT CObjectTool_ToolObject::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	m_isRect = true;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Cube), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	
	return S_OK;
}

CObjectTool_ToolObject * CObjectTool_ToolObject::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg)
{
	CObjectTool_ToolObject* pInstance = new CObjectTool_ToolObject(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create BackGround_ProtoType");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CObjectTool_ToolObject::Clone(void * pArg)
{
	CObjectTool_ToolObject* pInstance = new CObjectTool_ToolObject((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create BackGround_Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObjectTool_ToolObject::Free()
{
	__super::Free();

	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);

}