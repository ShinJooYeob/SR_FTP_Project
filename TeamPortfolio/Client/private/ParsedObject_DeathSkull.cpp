#include "stdafx.h"
#include "..\public\ParsedObject_DeathSkull.h"

float CParsedObject_DeathSkull::mStaticTime = 0;

CParsedObject_DeathSkull::CParsedObject_DeathSkull(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParsedObject(pGraphic_Device)
{
}

CParsedObject_DeathSkull::CParsedObject_DeathSkull(const CParsedObject_DeathSkull & rhs)
	: CParsedObject(rhs)
{
}

HRESULT CParsedObject_DeathSkull::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParsedObject_DeathSkull::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	/* 현재 객체에게 추가되어야할 컴포넌트들을 복제(or 참조)하여 멤버변수에 보관한다.  */
	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr) {
		_float3 vSettingPoint;
		memcpy(&vSettingPoint, pArg, sizeof(_float3));
		m_Layer_Tag = (TEXT("Layer_DeathSkull"));
		m_ComTransform->Scaled(_float3(0.3f, 0.3f, 0.3f));

		m_ComTransform->Set_MatrixState(CTransform::STATE_POS, vSettingPoint);
	}

	FAILED_CHECK(m_ComTexture->Change_TextureLayer(TEXT("DeathSkull")));
	m_pathNum = CParsedObject_DeathSkull::SKUAL_PASS_VERTEX;

	Set_ShaderVal(0.3f,5,8);
	mStaticTime += 1;
	mTime = mStaticTime;

	return S_OK;
}

_int CParsedObject_DeathSkull::Update(_float fTimeDelta)
{
	if (0 > __super::Update(fTimeDelta))
		return -1;

	mTime += fTimeDelta;

	return _int();
}

_int CParsedObject_DeathSkull::LateUpdate(_float fTimeDelta)
{
	if (0 > __super::LateUpdate(fTimeDelta))
		return -1;

	if (nullptr == m_ComRenderer)
		return -1;


	if (GetSingle(CGameInstance)->IsNeedToRender(m_ComTransform->Get_MatrixState(CTransform::STATE_POS)))
		m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	return _int();
}
//
//HRESULT CParsedObject_DeathSkull::SetUp_RenderState()
//{
//	if (nullptr == m_pGraphicDevice)
//		return E_FAIL;
//
//	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
//
//	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
//	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
//
//	return S_OK;
//}
//
//HRESULT CParsedObject_DeathSkull::Release_RenderState()
//{
//	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
//
//
//	return S_OK;
//}

_int CParsedObject_DeathSkull::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	//if (FAILED(m_ComTransform->Bind_WorldMatrix()))
	//	return E_FAIL;

	//if (FAILED(m_ComTexture->Bind_Texture()))
	//	return E_FAIL;

	// FAILED_CHECK(m_ComVIBuffer->Render());



	
	// 셰이더 랜더링
	_Matrix		WorldMatrix, ViewMatrix, ProjMatrix;

	auto * Objectlist = GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENE_STATIC, TAG_LAY(Layer_Player));
	CGameObject* obj = nullptr;
	if (Objectlist)
	{
		obj = Objectlist->back();
		CTransform* playerTrans = (CTransform*)obj->Get_Component(TAG_COM(Com_Transform));
		_float3 pos =  playerTrans->Get_MatrixState(CTransform::STATE_POS);
		m_ComShader->SetUp_ValueOnShader("g_vPlayerPosition", &pos, sizeof(_float3));

		
	}

	WorldMatrix = m_ComTransform->Get_WorldMatrix();
	m_pGraphicDevice->GetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphicDevice->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_ComShader->SetUp_ValueOnShader("g_WorldMatrix", D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix), sizeof(_Matrix));
	m_ComShader->SetUp_ValueOnShader("g_ViewMatrix", D3DXMatrixTranspose(&ViewMatrix, &ViewMatrix), sizeof(_Matrix));
	m_ComShader->SetUp_ValueOnShader("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_Matrix));

	m_ComShader->SetUp_ValueOnShader("g_WaveHeight", &mWaveHeight, sizeof(_float));
	m_ComShader->SetUp_ValueOnShader("g_Time", &mTime, sizeof(_float));
	m_ComShader->SetUp_ValueOnShader("g_Speed", &mSpeed, sizeof(_float));
	m_ComShader->SetUp_ValueOnShader("g_WaveFrequency", &mWaveFrequency, sizeof(_float));

	


	m_ComTexture->Bind_OnShader(m_ComShader, "g_Texture", 0);

	FAILED_CHECK(m_ComShader->Begin_Shader((int)m_pathNum));
	FAILED_CHECK(m_ComVIBuffer->Render());
	FAILED_CHECK(m_ComShader->End_Shader());

	return _int();
}

_int CParsedObject_DeathSkull::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CParsedObject_DeathSkull::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fMovePerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	
	/* For.Com_VIBuffer_Cube */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_DeathSkull"), TAG_COM(Com_VIBuffer), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_Skual), TAG_COM(Com_Shader), (CComponent**)&m_ComShader)))
		return E_FAIL;
	
	/* For. 텍스쳐*/
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_Parsed"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	return S_OK;
}

CParsedObject_DeathSkull * CParsedObject_DeathSkull::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CParsedObject_DeathSkull* pInstance = new CParsedObject_DeathSkull(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_DeathSkull");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CParsedObject_DeathSkull::Clone(void * pArg)
{
	CParsedObject_DeathSkull* pInstance = new CParsedObject_DeathSkull((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CParsedObject_DeathSkull");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CParsedObject_DeathSkull::Free()
{
	__super::Free();

	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComTexture);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComRenderer);
	Safe_Release(m_ComShader);
}
