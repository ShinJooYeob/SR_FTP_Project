#include "stdafx.h"
#include "ObjectTool_Rect.h"

CObjectTool_Rect::CObjectTool_Rect(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice),
	m_ComRenderer(nullptr),
	m_ComVIBuffer(nullptr),
	m_ComTransform(nullptr),
	m_ComTexture(nullptr)
{
}

CObjectTool_Rect::CObjectTool_Rect(const CObjectTool_Rect & rhs)
	:CGameObject(rhs)
{
}

HRESULT CObjectTool_Rect::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CObjectTool_Rect::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	// 텍스처 여러개 갈아 끼울 수 있게 하자.

	m_ComTexture->Bind_Texture();

	return S_OK;
}

_int CObjectTool_Rect::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	return _int();
}

_int CObjectTool_Rect::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;

	//렌더링 그룹에 넣어주는 역활
	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this)))
		return E_FAIL;

	return 0;
}

_int CObjectTool_Rect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	NULL_CHECK_BREAK(m_ComTransform);
	NULL_CHECK_BREAK(m_ComTexture);
	NULL_CHECK_BREAK(m_ComVIBuffer);

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture()))
		return E_FAIL;

	//렌더링 그룹에 들어가면 순서에 맞게 이 랜더가 호출되고 호출이 됬으면 버텍스 버퍼를 그려줘라
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	return 0;
}

_int CObjectTool_Rect::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return 0;
}

HRESULT CObjectTool_Rect::Set_Scaled(_float3 scale)
{
	if (scale.x == 0)
		scale.x = 1.f;

	if (scale.y == 0)
		scale.y = 1.f;

	if (scale.z == 0)
		scale.z = 1.f;

	m_ComTransform->Scaled(scale);
	return S_OK;
}

HRESULT CObjectTool_Rect::Set_Position(_float3 Position)
{
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, Position);
	return S_OK;
}

HRESULT CObjectTool_Rect::Set_Texture(MYFILEPATH pathdata)
{
	// 기존에 있던 텍스쳐 날림
	m_ComTexture->ClearTexture();
	m_tImgPath = pathdata;

	CTexture::tagTextureDesc desc = {};
	desc.szFilePath = m_tImgPath.wstrFullPath.c_str();

	if (FAILED(m_ComTexture->Initialize_Prototype(&desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CObjectTool_Rect::Set_Data(OUTPUT_OBJECTINFO data)
{
	Set_Position(data.fPos);
	Set_Scaled(data.fScale);
	MYFILEPATH mypath = {};
	mypath.wFileName = data.strTextureName;
	mypath.wstrFullPath = data.strTexturePath;
	Set_Texture(mypath);

	return S_OK;
}

HRESULT CObjectTool_Rect::Set_ViBuffer_Change()
{
	m_isRect = !m_isRect;
	Safe_Release(m_ComVIBuffer);

	if (m_isRect)
	{
		if (FAILED(__super::Change_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Change_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CObjectTool_Rect::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);
	m_isRect = true;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Texture_Default"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	return S_OK;
}

CObjectTool_Rect * CObjectTool_Rect::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg)
{
	CObjectTool_Rect* pInstance = new CObjectTool_Rect(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create BackGround_ProtoType");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CObjectTool_Rect::Clone(void * pArg)
{
	CObjectTool_Rect* pInstance = new CObjectTool_Rect((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create BackGround_Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObjectTool_Rect::Free()
{
	__super::Free();

	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);

}