#include "stdafx.h"
#include "UI_BossStatusUI.h"
#include "..\public\MonsterParent.h"
#include "Camera_Main.h"

CMonsterParent::CMonsterParent(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
	m_ComTransform = nullptr;
	m_ComRenderer = nullptr;
	m_ComVIBuffer = nullptr;
	m_ComTexture = nullptr;
	m_Com_Viewport = nullptr;
	m_ComShader = nullptr;

	m_Sphere.mCenterPosition = _float2(0, 0);
	m_Sphere.mRadius = 10.0f;
	mFrameCount = 0;

}

CMonsterParent::CMonsterParent(const CMonsterParent& rhs)
	: CGameObject(rhs)
{
	
	m_ComTransform = rhs.m_ComTransform;
	m_ComRenderer = rhs.m_ComRenderer;
	m_ComVIBuffer = rhs.m_ComVIBuffer;
	m_ComTexture = rhs.m_ComTexture;
	m_Com_Viewport = rhs.m_Com_Viewport;
	m_ComShader = rhs.m_ComShader;

	Safe_AddRef(m_ComTransform);
	Safe_AddRef(m_ComRenderer);
	Safe_AddRef(m_ComVIBuffer);
	Safe_AddRef(m_ComTexture);
	Safe_AddRef(m_Com_Viewport);
	Safe_AddRef(m_ComShader);
}


HRESULT CMonsterParent::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));
	return S_OK;
}

HRESULT CMonsterParent::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	m_Sphere.mCenterPosition = _float2(0, 0);
	m_Sphere.mRadius = 30.0f;
	mFrameCount = 0;
	mbDying = false;
	

	return S_OK;
}

_int CMonsterParent::Update(_float fDeltaTime)
{
	FAILED_CHECK(__super::Update(fDeltaTime));
	Update_Die(fDeltaTime);
	
	return _int();
}

_int CMonsterParent::LateUpdate(_float fDeltaTime)
{
	FAILED_CHECK(__super::LateUpdate(fDeltaTime));


	return _int();
}

_int CMonsterParent::Render()
{
	FAILED_CHECK(__super::Render());

	if (nullptr == m_ComVIBuffer ||
		nullptr == m_ComTexture)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix_Look_Camera()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(g_fDeltaTime, &mFrameCount)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

_int CMonsterParent::LateRender()
{
	FAILED_CHECK(__super::LateRender());
	
	return S_OK();
}

HRESULT CMonsterParent::SetPos(_float3 pos)
{
	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, pos);

	return S_OK;
}

_float3 CMonsterParent::GetScreenToWorld(_float2 screenPos)
{
	// 임의의 스크린 좌표를 월드 좌표로 변환

	D3DVIEWPORT9	ViewPortDesc;
	m_pGraphicDevice->GetViewport(&ViewPortDesc);

	_float4 vScreenToWorld;
	vScreenToWorld.x = screenPos.x / (ViewPortDesc.Width*0.5f) - 1.f;
	vScreenToWorld.y = screenPos.y / -(ViewPortDesc.Height*0.5f) + 1.f;
	vScreenToWorld.z = 0.0f;
	vScreenToWorld.w = 1.f;


	_Matrix ProjMatrixInverse;
	m_pGraphicDevice->GetTransform(D3DTS_PROJECTION, &ProjMatrixInverse);
	D3DXMatrixInverse(&ProjMatrixInverse, nullptr, &ProjMatrixInverse);
	D3DXVec4Transform(&vScreenToWorld, &vScreenToWorld, &ProjMatrixInverse);

	_Matrix ViewMatrixInverse;
	m_pGraphicDevice->GetTransform(D3DTS_VIEW, &ViewMatrixInverse);
	D3DXMatrixInverse(&ViewMatrixInverse, nullptr, &ViewMatrixInverse);
	D3DXVec4Transform(&vScreenToWorld, &vScreenToWorld, &ViewMatrixInverse);
	
	_float3 newPos = _float3(vScreenToWorld.x, vScreenToWorld.y, vScreenToWorld.z);

	if (abs(newPos.x) > 100)
		newPos.x = 0;
	if (abs(newPos.z) > 100)
		newPos.z = 0;

	return newPos;
}


void CMonsterParent::Update_Die(float deltatime)
{
}

HRESULT CMonsterParent::SetUp_Components()
{

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Texture_Monster), TAG_COM(Com_Texture), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_CollisionView), TAG_COM(Com_CollisionView), (CComponent**)&m_Com_Viewport)))
		return E_FAIL;
	
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_Monster), TAG_COM(Com_Shader), (CComponent**)&m_ComShader)))
		return E_FAIL;
	

	return S_OK;
}

_float3 CMonsterParent::Update_CameraPosition(_float3 localPos)
{
	// 카메라 위치의 로컬이라 가정하고 카메라의 월드를 곱해준다.

	_float3 NewWorldPos = localPos;

	auto CameraList = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENE_STATIC, TAG_LAY(Layer_Camera_Main));
	CCamera* camobj = (CCamera*)CameraList->front();
	
	if (camobj == nullptr)
		return localPos;

	D3DXVec3TransformCoord(&NewWorldPos, &NewWorldPos, &camobj->Get_Camera_Transform()->Get_WorldMatrix());
	return NewWorldPos;
}


void CMonsterParent::Free()
{
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_Com_Viewport);	
	Safe_Release(m_ComShader);

	__super::Free();
	
}
