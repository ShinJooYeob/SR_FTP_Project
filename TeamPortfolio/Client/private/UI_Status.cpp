#include "stdafx.h"
#include "Player.h"
#include "UI_Result.h"
#include "..\public\UI_Status.h"





CUI_Status::CUI_Status(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_Status::CUI_Status(const CUI_Status & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Status::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Status::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	FAILED_CHECK(SetUp_UIDesc());

	return S_OK;
}
void CUI_Status::Set_UI_TransformRect( _float4 vRect)
{
	//vRect.x,y,z,w=top,left,bottom,right
	_float4 vResult{};
	/*너비*/vResult.z = vRect.w-vRect.y;
	/*높이*/vResult.w = vRect.z-vRect.x;
	/*x좌표*/vResult.x = vRect.y + vResult.z*0.5f;
	/*y좌표*/vResult.y = vRect.x + vResult.w*0.5f;
	Set_UI_Transform(m_ComTransform, vResult);
	
}

_int CUI_Status::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	


	FAILED_CHECK(Update_MouseButton(fDeltaTime));

	FAILED_CHECK(Update_Animation(fDeltaTime));

	return _int();

}





_int CUI_Status::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;





	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return _int();
}

_int CUI_Status::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;



	FAILED_CHECK(First_SetUp_RenderState());
	
	FAILED_CHECK(Second_SetUp_RenderState());

	FAILED_CHECK(Third_SetUp_RenderState());



	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CUI_Status::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CUI_Status::First_SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[0])))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	FAILED_CHECK(m_ComTexture->Change_TextureLayer(L"GradationBar"));

	if (FAILED(m_ComTexture->Bind_Texture(1)))
		return E_FAIL;


	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	///진우추가///
	Set_TotalTimerSec(m_pResult->Get_MaxTime());
	
	_float temp=0.f;
	if (m_pResult->Get_NowTime() >= 30.1&&m_pResult->Get_MaxTimebyTimeUp() != 0)
		temp = - m_pResult->Get_MaxTimebyTimeUp();
	else if (m_pResult->Get_NowTime() < 30.1&&m_pResult->Get_MaxTimebyTimeUp()!=0)
		temp = - m_pResult->Get_NowTime();
	//////////////
	_uint iAlpha = (m_pResult->Get_NowTime()+temp / (_float)m_fTotalTimerTime * 255.f);
	if (iAlpha >= 254)
	{
		iAlpha = 255;
		m_pPlayer->Set_StageEnd(0);
		m_pResult->Set_Clear_Wait_AnimTime(false, 3.f);
	}
	if (iAlpha < 0)
		iAlpha = 0;
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, iAlpha);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(0)))
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Status::Second_SetUp_RenderState()
{
	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[1])))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150);


	const _tchar* TempTag = m_pPlayer->Get_NowTextureTag();

	if (m_bIsStatusChage)
	{


		if (TempTag != nullptr)
		{
			if (!m_fHurtedTime && !lstrcmp(TempTag, L"hurt"))
				m_fHurtedTime += g_fDeltaTime;
		}

		if (FAILED(m_ComTexture->Bind_Texture(3)))
			return E_FAIL;

		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_ComTexture->Bind_Texture(2)))
			return E_FAIL;

		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;


		m_vUIDesc[1].y -= 10;

		if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[1])))
			return E_FAIL;
		if (FAILED(m_ComTransform->Bind_WorldMatrix()))
			return E_FAIL;


		if (TempTag != nullptr)
		{
			m_ComTexture2->Change_TextureLayer_Wait(TempTag);
			if (!m_fHurtedTime && !lstrcmp(TempTag, L"hurt"))
				m_fHurtedTime += g_fDeltaTime;
		}


		if (FAILED(m_ComTexture2->Bind_Texture_AutoFrame(g_fDeltaTime)))
			return E_FAIL;
		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;

		m_vUIDesc[1].y += 10;
	}

	
	return S_OK;
}

HRESULT CUI_Status::Third_SetUp_RenderState()
{

	FAILED_CHECK(m_ComTexture->Change_TextureLayer(L"walk"));

	if (FAILED(m_ComTexture->Bind_Texture(_uint(m_fWalkFrame))))
		return E_FAIL;

	_int iPlayerLife = m_pPlayer->Get_PlayerLife();
	if (iPlayerLife > 0  && iPlayerLife <= 3)
	{

	for (_uint i = 0; i < (_uint)iPlayerLife; i++)
	{
		if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[i + 2])))
			return E_FAIL;

		if (FAILED(m_ComTransform->Bind_WorldMatrix()))
			return E_FAIL;

		if (i == iPlayerLife - 1 && m_fHurtedTime)
		{
			FAILED_CHECK(m_ComTexture->Change_TextureLayer(L"hurt"));

			if (FAILED(m_ComTexture->Bind_Texture((_uint)(m_fWalkFrame))))
				return E_FAIL;
		}

		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;
	}

	}

	return S_OK;
}

HRESULT CUI_Status::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	return S_OK;
}

HRESULT CUI_Status::Set_TotalTimerSec(_float iTotalSec)
{
	m_fTotalTimerTime  = iTotalSec;
	return S_OK	;
}

HRESULT CUI_Status::Set_Player(CGameObject * pPlayer)
{
	if (pPlayer == nullptr)
		return E_FAIL;

	m_pPlayer = (CPlayer*)(pPlayer);

	return S_OK;
}

HRESULT CUI_Status::Set_ResultUI(CGameObject * pResult)
{
	if (pResult == nullptr)
		return E_FAIL;

	m_pResult = (CUI_Result*)pResult;
	Set_TotalTimerSec(m_pResult->Get_MaxTime());

	return S_OK;
}

HRESULT CUI_Status::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_StatusUI"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Texture_Player), TEXT("Com_Texture_2"), (CComponent**)&m_ComTexture2)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Status::SetUp_UIDesc()
{
	m_vUIDesc[0].x = 315;
	m_vUIDesc[0].y = 30;
	m_vUIDesc[0].z = 400;
	m_vUIDesc[0].w = 20;


	m_vUIDesc[1].x = 70;
	m_vUIDesc[1].y = 70;
	m_vUIDesc[1].z = 100;
	m_vUIDesc[1].w = 100;


	for (_uint i = 0 ; i < 3;i++)
	{
		m_vUIDesc[i + 2].x = 150 + i*50.f;
		m_vUIDesc[i + 2].y = 90;
		m_vUIDesc[i + 2].z = 50;
		m_vUIDesc[i + 2].w = 50;
	}

	FAILED_CHECK(Set_TotalTimerSec(60));

	m_bIsStatusChage = false;


	return S_OK;
}

HRESULT CUI_Status::Update_MouseButton(_float fTimeDelta)
{
	CGameInstance* pInstance = GetSingle(CGameInstance);

	if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
	{
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		if (PtInRect(&TransUIDesc_to_Rect(m_vUIDesc[1]), ptMouse))
		{
			m_bIsStatusChage = !m_bIsStatusChage;
		}
	}
	return S_OK;
}

HRESULT CUI_Status::Update_Animation(_float fTimeDelta)
{


	m_fWalkFrame += fTimeDelta * 6.f;
	if (m_fWalkFrame > 6)
		m_fWalkFrame = 0;


	if (m_fHurtedTime != 0)
	{
		m_fHurtedTime += fTimeDelta;
		if (m_fHurtedTime > 1.2f)
		{
			m_fHurtedTime = 0;
			m_pPlayer->Set_PlayerLife(-1);
		}
	}

	if (m_pPlayer->Get_PlayerLife() <=0 )
	{

		m_pPlayer->Set_StageEnd(0);
		m_pResult->Set_Clear_Wait_AnimTime(false, 3.f);
	}

	return S_OK	;
}

RECT CUI_Status::TransUIDesc_to_Rect(_float4 UIDesc)
{
	RECT tRsult{};

	tRsult.left = (LONG)(UIDesc.x - UIDesc.z*0.5f);
	tRsult.top = (LONG)(UIDesc.y - UIDesc.w*0.5f);
	tRsult.right = (LONG)(UIDesc.x + UIDesc.z*0.5f);
	tRsult.bottom = (LONG)(UIDesc.y + UIDesc.w*0.5f);


	return tRsult;
}





CUI_Status * CUI_Status::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_Status* pInstance = new CUI_Status(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create UIStatus_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_Status::Clone(void * pArg)
{
	CUI_Status* pInstance = new CUI_Status((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create UIStatus_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_Status::Free()
{
	__super::Free();

	
	Safe_Release(m_ComTexture2);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
