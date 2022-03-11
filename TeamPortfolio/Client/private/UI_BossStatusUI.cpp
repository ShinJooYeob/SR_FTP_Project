#include "stdafx.h"
#include "Player.h"
#include "BossMonster.h"
#include "UI_Result.h"
#include "..\public\UI_BossStatusUI.h"





CUI_BossStatusUI::CUI_BossStatusUI(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_BossStatusUI::CUI_BossStatusUI(const CUI_BossStatusUI & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_BossStatusUI::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_BossStatusUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	FAILED_CHECK(SetUp_UIDesc());

	m_fDepth = 300;


	return S_OK;
}

_int CUI_BossStatusUI::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	


	FAILED_CHECK(Update_MouseButton(fDeltaTime));

	FAILED_CHECK(Update_Animation(fDeltaTime));

	return _int();

}





_int CUI_BossStatusUI::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;



	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return _int();
}

_int CUI_BossStatusUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	//시간 바
	FAILED_CHECK(Zero_SetUp_RenderState());
	//바 움직임
	FAILED_CHECK(First_SetUp_RenderState());
	//스테이터스 전체 이미지
	FAILED_CHECK(Second_SetUp_RenderState());




	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CUI_BossStatusUI::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CUI_BossStatusUI::Zero_SetUp_RenderState()
{

	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	if (m_NowColor.x < 0) m_NowColor.x = 0; if (m_NowColor.x > 255) m_NowColor.x = 255;
	if (m_NowColor.y < 0) m_NowColor.y = 0; if (m_NowColor.y > 255) m_NowColor.y = 255;
	if (m_NowColor.z < 0) m_NowColor.z = 0; if (m_NowColor.z > 255) m_NowColor.z = 255;

	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_XRGB(_uint(m_NowColor.x), _uint(m_NowColor.y), _uint(m_NowColor.z)));

	if (FAILED(Set_UI_Transform(m_ComTransform, TransRect_to_UIDesc(m_vBarRect[2]))))
		return E_FAIL;
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(7)))
		return E_FAIL;
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;




	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);



	return S_OK;
}

HRESULT CUI_BossStatusUI::First_SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;



	//HP BAR 
	if (FAILED(Set_UI_Transform(m_ComTransform, TransRect_to_UIDesc(m_vBarRect[0]))))
		return E_FAIL;
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(1)))
		return E_FAIL;
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;



	if (FAILED(Set_UI_Transform(m_ComTransform, TransRect_to_UIDesc(m_vBarRect[1]))))
		return E_FAIL;
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(2)))
		return E_FAIL;
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 180);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//디비전 라인
	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[3])))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(3)))
		return E_FAIL;
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BossStatusUI::Second_SetUp_RenderState()
{

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 180);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//상태바
	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[0])))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(0)))
		return E_FAIL;
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;




	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 180);
	//플레이어 얼굴 이미지

	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[1])))
		return E_FAIL;
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (m_PlayerPictureClicked)
	{
		if (FAILED(m_ComTexture->Bind_Texture(5)))
			return E_FAIL;
		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;

	}
	else {
		if (FAILED(m_ComTexture->Bind_Texture(4)))
			return E_FAIL;
		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;

		const _tchar* TextureTag = m_pPlayer->Get_NowTextureTag();

		if (TextureTag != nullptr)
		{
			FAILED_CHECK(m_ComTexture_Player->Change_TextureLayer_Wait(TextureTag));
		}
		

		if (FAILED(m_ComTexture_Player->Bind_Texture_AutoFrame(g_fDeltaTime)))
			return E_FAIL;
		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;

	}

	//보스 얼굴 이미지
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 32);

	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[2])))
		return E_FAIL;
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (m_BossPictureClicked)
	{	
		if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[2])))
			return E_FAIL;

		if (FAILED(m_ComTransform->Bind_WorldMatrix()))
			return E_FAIL;

		if (FAILED(m_ComTexture->Bind_Texture(6)))
			return E_FAIL;
		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;


	}
	else {
		if (FAILED(m_ComTexture->Bind_Texture(4)))
			return E_FAIL;
		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;


		const _tchar* TextureTag = m_pBoss->Get_NowTextureTag();

		if (TextureTag != nullptr)
		{
			FAILED_CHECK(m_ComTexture_Boss->Change_TextureLayer_Wait(TextureTag));
		}


		if (FAILED(m_ComTexture_Boss->Bind_Texture_AutoFrame(g_fDeltaTime)))
			return E_FAIL;
		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;

	}

	//////////////////////////////////////////////////////////////////////////

	
	return S_OK;
}

HRESULT CUI_BossStatusUI::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	return S_OK;
}


HRESULT CUI_BossStatusUI::Set_Player(CGameObject * pPlayer)
{
	if (pPlayer == nullptr)
		return E_FAIL;

	m_pPlayer = (CPlayer*)(pPlayer);

	return S_OK;
}

HRESULT CUI_BossStatusUI::Set_ResultUI(CGameObject * pResult)
{
	if (pResult == nullptr)
		return E_FAIL;

	m_pResult = (CUI_Result*)pResult;

	return S_OK;
}

void CUI_BossStatusUI::Change_VersusPoint(_float vChangePoint)
{
	m_fNowVersusPoint -= vChangePoint;
	m_fStartPoint = m_fEasingPoint;
	m_bVersusPointChange = true;


	m_fPassedTime = 0;
	m_fTargetPoint = m_fNowVersusPoint;

}

HRESULT CUI_BossStatusUI::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_BossStatusUI"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Texture_Player), TEXT("Com_Texture_Player"), (CComponent**)&m_ComTexture_Player)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Texture_Monster), TEXT("Com_Texture_Boss"), (CComponent**)&m_ComTexture_Boss)))
		return E_FAIL;



	m_pPlayer = (CPlayer*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Player)));
	NULL_CHECK_BREAK(m_pPlayer);
	m_pPlayer->Set_BossStatusUI(this);
	Safe_AddRef(m_pPlayer);


	m_pBoss = (CBossMonster*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Monster)));
	NULL_CHECK_BREAK(m_pBoss);

	m_pBoss->Set_BossStatusUI(this);

	Safe_AddRef(m_pBoss);
	



	return S_OK;
}

HRESULT CUI_BossStatusUI::SetUp_UIDesc()
{
	m_vUIDesc[0].x = g_iWinCX*0.5f;
	m_vUIDesc[0].z = g_iWinCX;
	m_vUIDesc[0].w = m_vUIDesc[0].x * 0.25f;
	m_vUIDesc[0].y = g_iWinCY - m_vUIDesc[0].w * 0.5f + 25;


	m_vUIDesc[1].z = 100;
	m_vUIDesc[1].w = 100;
	m_vUIDesc[1].x = m_vUIDesc[0].x - m_vUIDesc[0].z * 0.5f + m_vUIDesc[1].z *0.5f;
	m_vUIDesc[1].y = m_vUIDesc[0].y;


	m_vUIDesc[2].z = 100;
	m_vUIDesc[2].w = 100;
	m_vUIDesc[2].x = m_vUIDesc[0].x + m_vUIDesc[0].z * 0.5f - m_vUIDesc[1].z *0.5f;
	m_vUIDesc[2].y = m_vUIDesc[0].y;




	m_fEasingPoint = m_fStartPoint = m_fNowVersusPoint = m_fTotalVersusPoint * 0.5f;

	//디비전 라인
	m_vUIDesc[3].z = 40;
	m_vUIDesc[3].w = 40;
	m_vUIDesc[3].x = m_vUIDesc[0].x -370 + (740 * m_fNowVersusPoint/ m_fTotalVersusPoint);
	m_vUIDesc[3].y = m_vUIDesc[0].y + 20;




	//플레이어바
	m_vBarRect[0].left = m_vUIDesc[0].x - m_vUIDesc[0].z * 0.3f;
	m_vBarRect[0].right = m_vUIDesc[3].x;
	m_vBarRect[0].top = m_vUIDesc[0].y -2;
	m_vBarRect[0].bottom = m_vUIDesc[0].y + 34;

	//보스 바
	m_vBarRect[1].left = m_vUIDesc[3].x;
	m_vBarRect[1].right = m_vUIDesc[0].x + m_vUIDesc[0].z * 0.3f;
	m_vBarRect[1].top = m_vBarRect[0].top;
	m_vBarRect[1].bottom = m_vBarRect[0].bottom;


	//타이머 바
	m_vBarRect[2].left = m_vUIDesc[0].x - m_vUIDesc[0].z * 0.31f;
	m_vBarRect[2].right = m_vUIDesc[0].x + m_vUIDesc[0].z * 0.31f;
	m_vBarRect[2].top = m_vUIDesc[0].y - 45;
	m_vBarRect[2].bottom = m_vUIDesc[0].y - 2;




	m_NowColor = m_StartColor = _float3(rand() % 255, rand() % 255, rand() % 255);
	m_TargetColor = _float3(rand() % 255, rand() % 255, rand() % 255);
	m_fTimeBarPassedTime = 0;


	return S_OK;
}

HRESULT CUI_BossStatusUI::Update_MouseButton(_float fTimeDelta)
{
	if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_O) & DIS_Down )
	{
		Change_VersusPoint(1);
	}
	if (GetSingle(CGameInstance)->Get_DIKeyState(DIK_P) & DIS_Down)
	{
		Change_VersusPoint(-1);

	}



	CGameInstance* pInstance = GetSingle(CGameInstance);

	if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
	{
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		if (PtInRect(&TransUIDesc_to_Rect(m_vUIDesc[1]), ptMouse))
		{
			m_PlayerPictureClicked = !m_PlayerPictureClicked;
		}

		if (PtInRect(&TransUIDesc_to_Rect(m_vUIDesc[2]), ptMouse))
		{
			m_BossPictureClicked = !m_BossPictureClicked;
		}
	}

	return S_OK;
}

HRESULT CUI_BossStatusUI::Update_Animation(_float fTimeDelta)
{
	if (!m_bIsStageEnd)
	{
		//////타이머바 애니메이션////////////////////////////////////////////////////////////////////
		m_fTimeBarPassedTime += fTimeDelta;
		m_NowColor = EaseingFloat3(TYPE_Linear, m_StartColor, m_TargetColor, m_fTimeBarPassedTime, 0.7f);
		if (m_fTimeBarPassedTime > 0.7f)
		{
			m_NowColor = m_StartColor = m_TargetColor;
			m_TargetColor = _float3(rand() % 255, rand() % 255, rand() % 255);
			m_fTimeBarPassedTime = 0;
		}


		_float Timer = m_pResult->Get_NowTime() / m_pResult->Get_MaxTime();
		m_vBarRect[2].right = m_vUIDesc[0].x + m_vUIDesc[0].z * 0.31f - (m_vUIDesc[0].z * 0.62f * Timer);

		if (Timer >= 1)
		{
			m_bIsStageEnd = true;
			m_pPlayer->Set_StageEnd(0);
			m_pResult->Set_Clear_Wait_AnimTime(false, 3.f);
		}

	}


	if (m_bVersusPointChange)
	{
		m_fPassedTime += fTimeDelta;
		m_fEasingPoint = GetSingle(CGameInstance)->Easing(TYPE_QuarticOut, m_fStartPoint, m_fTargetPoint, m_fPassedTime);

		if (m_fPassedTime > 1.f)
		{
			m_fStartPoint = m_fTargetPoint;
			m_fEasingPoint = m_fTargetPoint;
			m_bVersusPointChange = false;
		}

		_float Value = m_fEasingPoint / m_fTotalVersusPoint;
		if (Value > 1)
			Value = 1;
		else if (Value < 0)
			Value = 0;

		m_vUIDesc[3].x = m_vUIDesc[0].x  - 370 +  (740 * Value);
		m_vBarRect[0].right = m_vBarRect[1].left = m_vUIDesc[3].x;
	}

	if (!m_bIsStageEnd)
	{

		if (m_fEasingPoint / m_fTotalVersusPoint <= 0)
		{
			m_bIsStageEnd = true;
			m_pPlayer->Set_StageEnd(0);
			m_pResult->Set_Clear_Wait_AnimTime(false, 3.f);	
		}
		else if (m_fEasingPoint / m_fTotalVersusPoint >= 1)
		{
			m_bIsStageEnd = true;
			for (_uint i = 0; i < 5; i++)
			{
				m_pResult->Set_RankStar();
			}
			m_pPlayer->Set_StageEnd(1);
		}
	}


	return S_OK	;
}

RECT CUI_BossStatusUI::TransUIDesc_to_Rect(_float4 UIDesc)
{
	RECT tRsult{};

	tRsult.left = (LONG)(UIDesc.x - UIDesc.z*0.5f);
	tRsult.top = (LONG)(UIDesc.y - UIDesc.w*0.5f);
	tRsult.right = (LONG)(UIDesc.x + UIDesc.z*0.5f);
	tRsult.bottom = (LONG)(UIDesc.y + UIDesc.w*0.5f);


	return tRsult;
}

_float4 CUI_BossStatusUI::TransRect_to_UIDesc(RECT vRect)
{
	//vRect.x,y,z,w=top,left,bottom,right
	_float4 vResult{};

	/*너비*/vResult.z = vRect.right - vRect.left;
	/*높이*/vResult.w = vRect.bottom - vRect.top;
	/*x좌표*/vResult.x = vRect.left + vResult.z *0.5f;
	/*y좌표*/vResult.y = vRect.top + vResult.w * 0.5f;


	return vResult;
}

_float3 CUI_BossStatusUI::EaseingFloat3(EasingTypeID id, _float3 StartPos, _float3 EndPos, _float curTime, _float maxTime)
{
	_float3 newPos = _float3(0, 0, 0);
	newPos.x = GetSingle(CGameInstance)->Easing(id, StartPos.x, EndPos.x, curTime, maxTime);
	newPos.y = GetSingle(CGameInstance)->Easing(id, StartPos.y, EndPos.y, curTime, maxTime);
	newPos.z = GetSingle(CGameInstance)->Easing(id, StartPos.z, EndPos.z, curTime, maxTime);


	return newPos;
}




CUI_BossStatusUI * CUI_BossStatusUI::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_BossStatusUI* pInstance = new CUI_BossStatusUI(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create UIStatus_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_BossStatusUI::Clone(void * pArg)
{
	CUI_BossStatusUI* pInstance = new CUI_BossStatusUI((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create UIStatus_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_BossStatusUI::Free()
{
	__super::Free();

	if (m_bIsClone)
	{
		Safe_Release(m_pPlayer);
		Safe_Release(m_pBoss);
	}
	
	Safe_Release(m_ComTexture_Player);
	Safe_Release(m_ComTexture_Boss);
	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
