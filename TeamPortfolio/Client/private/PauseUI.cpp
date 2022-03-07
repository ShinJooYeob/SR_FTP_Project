#include "stdafx.h"
#include "..\public\PauseUI.h"
#include "Scene.h"





CPauseUI::CPauseUI(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CPauseUI::CPauseUI(const CPauseUI & rhs)
	: CUI(rhs)
{
}

HRESULT CPauseUI::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPauseUI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	ZeroMemory(m_vUIDesc, sizeof(_float4) * 8);

	if (FAILED(SetUp_Components()))
		return E_FAIL;
	
	FAILED_CHECK(SetUp_UIDesc());

	m_bIsClicked = false;
	m_bIsPauseAnimFinished = true;


	return S_OK;
}
void CPauseUI::Set_UI_TransformRect( _float4 vRect)
{
	//vRect.x,y,z,w=top,left,bottom,right
	_float4 vResult{};
	/*너비*/vResult.z = vRect.w-vRect.y;
	/*높이*/vResult.w = vRect.z-vRect.x;
	/*x좌표*/vResult.x = vRect.y + vResult.z*0.5f;
	/*y좌표*/vResult.y = vRect.x + vResult.w*0.5f;
	Set_UI_Transform(m_ComTransform, vResult);
	
}

_int CPauseUI::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;




	FAILED_CHECK(Update_MouseButton(fDeltaTime));
	
	FAILED_CHECK(Update_Animation(fDeltaTime));

	return _int();

}





_int CPauseUI::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;



	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return _int();
}

_int CPauseUI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;



	if (!m_bIsClicked) 
	{
		FAILED_CHECK(First_SetUp_RenderState());
	}
	else
	{
		FAILED_CHECK(Second_SetUp_RenderState());

	}




	if (FAILED(Release_RenderState()))
		return E_FAIL;



	return _int();
}

_int CPauseUI::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CPauseUI::First_SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[0])))
		return E_FAIL;
	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;
	if (FAILED(m_ComTexture->Bind_Texture(0)))
		return E_FAIL;
	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CPauseUI::Second_SetUp_RenderState()
{

	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	//플레이어 그림
	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[8])))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (m_bIsOnAnim)
	{
		FAILED_CHECK(m_ComTexture->Change_TextureLayer(L"PlayerCarry"));
		FAILED_CHECK(m_ComTexture->Bind_Texture());
	}
	else
	{
		FAILED_CHECK(m_ComTexture->Change_TextureLayer(L"PlayerWalk"));
		FAILED_CHECK(m_ComTexture->Bind_Texture(_uint(m_fWalkingFrame)));

	}

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	FAILED_CHECK(m_ComTexture->Change_TextureLayer(L"Pause"));

	//판
	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[1])))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;
	if (FAILED(m_ComTexture->Bind_Texture(1)))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;


	//버튼들
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 180);
	for (_uint i = 2; i < 6; i++)
	{
		if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc[i])))
			return E_FAIL;
		if (FAILED(m_ComTransform->Bind_WorldMatrix()))
			return E_FAIL;
		if (FAILED(m_ComTexture->Bind_Texture(2)))
			return E_FAIL;
		if (FAILED(m_ComVIBuffer->Render()))
			return E_FAIL;
	}




	return S_OK;
}


HRESULT CPauseUI::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	return S_OK;
}

HRESULT CPauseUI::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_Pause"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPauseUI::SetUp_UIDesc()
{

	m_vUIDesc[0].x = g_iWinCX - 30.f;
	m_vUIDesc[0].y = 30;
	m_vUIDesc[0].z = 50;
	m_vUIDesc[0].w = 50;

	m_vUIDesc[1].x = g_iWinCX - 222.f;
	m_vUIDesc[1].y = g_iWinCY *0.5f;
	m_vUIDesc[1].z = 384;
	m_vUIDesc[1].w = 512;

	m_vUIDesc[2].x = m_vUIDesc[1].x + 80.f; //m_vUIDesc[1].x + 170.f;// m_vUIDesc[1].x - 10.f;
	m_vUIDesc[2].y = m_vUIDesc[1].y - 170;
	m_vUIDesc[2].z = 50;
	m_vUIDesc[2].w = 30;

	m_vUIDesc[3].x = m_vUIDesc[1].x + 80.f;
	m_vUIDesc[3].y = m_vUIDesc[1].y - 80;
	m_vUIDesc[3].z = 50;
	m_vUIDesc[3].w = 30;

	m_vUIDesc[4].x = m_vUIDesc[1].x + 80.f;
	m_vUIDesc[4].y = m_vUIDesc[1].y + 10;
	m_vUIDesc[4].z = 50;
	m_vUIDesc[4].w = 30;

	m_vUIDesc[5].x = m_vUIDesc[1].x + 80.f;
	m_vUIDesc[5].y = m_vUIDesc[1].y + 100;
	m_vUIDesc[5].z = 50;
	m_vUIDesc[5].w = 30;

	m_vUIDesc[6].x = m_vUIDesc[1].x - 90.f;
	m_vUIDesc[6].y = m_vUIDesc[1].y + 190;
	m_vUIDesc[6].z = 80;
	m_vUIDesc[6].w = 80;

	m_vUIDesc[7].x = m_vUIDesc[1].x + 95.f;
	m_vUIDesc[7].y = m_vUIDesc[1].y + 190;
	m_vUIDesc[7].z = 80;
	m_vUIDesc[7].w = 80;


	m_vUIDesc[8].x = m_vUIDesc[1].x;
	m_vUIDesc[8].y = m_vUIDesc[1].y + 240;
	m_vUIDesc[8].z = 80;
	m_vUIDesc[8].w = 80;

	return S_OK;
}

HRESULT CPauseUI::Update_MouseButton(_float fTimeDelta)
{
	CGameInstance* pInstance = GetSingle(CGameInstance);
	if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press)
	{
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);
		if (!m_bIsClicked)
		{
			if (PtInRect(&TransUIDesc_to_Rect(m_vUIDesc[0]), ptMouse))
			{
				m_bIsClicked = true;
				m_bIsOnAnim = true;
				m_fStartPoint = -g_iWinCY*0.5f;
				m_fTargetPoint = g_iWinCY *0.5f;
				m_vUIDesc[1].x = g_iWinCX - 222.f;
				m_fPassedTime = 0;
				GetSingle(CGameInstance)->PlaySound(L"JW_option_button.wav", CHANNEL_UI, 1.f);
				m_bIsPauseAnimFinished = false;
				m_fDepth = 1;

				for (_uint i = 0; i < 4; i++)
				{

					switch (i)
					{
					case 0:
						m_vUIDesc[i + 2].x = (m_vUIDesc[1].x - 10.f) + pInstance->Get_Channel_Volume(CHANNEL_BGM) * 180.f;
						break;
					case 1:
						m_vUIDesc[i + 2].x = (m_vUIDesc[1].x - 10.f) + pInstance->Get_Channel_Volume(CHANNEL_PLAYER) * 180.f;
						break;
					case 2:
						m_vUIDesc[i + 2].x = (m_vUIDesc[1].x - 10.f) + pInstance->Get_Channel_Volume(CHANNEL_UI) * 180.f;
						break;
					case 3:
						m_vUIDesc[i + 2].x = (m_vUIDesc[1].x - 10.f) + pInstance->Get_Channel_Volume(CHANNEL_EFFECT) * 180.f;
						break;

					default:
						break;
					}


				}




			}

		}
		else if(m_bIsPauseAnimFinished)
		{


			if (PtInRect(&TransUIDesc_to_Rect(m_vUIDesc[6]), ptMouse))
			{
				m_bIsPauseAnimFinished = false;
				m_bIsOnAnim = false;
				GetSingle(CGameInstance)->PlaySound(L"JW_option_button.wav", CHANNEL_UI, 2.f);
				m_vUIDesc[1].y = g_iWinCY *0.5f;
				m_fStartPoint = g_iWinCX - 222.f;
				m_fTargetPoint = g_iWinCX + 222.f;
				m_fPassedTime = 0;

			}
			else if (PtInRect(&TransUIDesc_to_Rect(m_vUIDesc[7]), ptMouse))				//////////////스테이지 셀렉 씬으로 돌아가기
			{
				GetSingle(CGameInstance)->PlaySound(L"JW_option_button.wav", CHANNEL_UI, 2.f);
				m_bIsClicked = false;
				m_fDepth = -1;

				GetSingle(CGameInstance)->Get_Scene()->Scene_InGame_Chage(true, SCENE_STAGESELECT);

			}

			if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
			{
				ZeroMemory(m_SoundChek, sizeof(_bool) * 4);
				if (PtInRect(&TransUIDesc_to_Rect(m_vUIDesc[2]), ptMouse))
				{
					m_SoundChek[0] = true;
				}
				else if (PtInRect(&TransUIDesc_to_Rect(m_vUIDesc[3]), ptMouse))
				{
					m_SoundChek[1] = true;
				}
				else if (PtInRect(&TransUIDesc_to_Rect(m_vUIDesc[4]), ptMouse))
				{
					m_SoundChek[2] = true;
				}
				else if (PtInRect(&TransUIDesc_to_Rect(m_vUIDesc[5]), ptMouse))
				{
					m_SoundChek[3] = true;
				}
			}
			else if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Up)
			{
				ZeroMemory(m_SoundChek, sizeof(_bool) * 4);
			}
			else
			{
				for (_uint i = 0 ; i< 4; i++)
				{
					if (m_SoundChek[i])
					{

						m_vUIDesc[i+2].x += (pInstance->Get_DIMouseMoveState(CInput_Device::MMS_X));
						//m_vUIDesc[1].x + 170.f;// m_vUIDesc[1].x - 10.f;
						if (m_vUIDesc[i + 2].x < m_vUIDesc[1].x - 10.f)
							m_vUIDesc[i + 2].x = m_vUIDesc[1].x - 10.f;
						if (m_vUIDesc[i + 2].x > m_vUIDesc[1].x + 170.f)
							m_vUIDesc[i + 2].x = m_vUIDesc[1].x + 170.f;


						switch (i)
						{
						case 0:
							pInstance->Channel_VolumeUp(CHANNEL_BGM, ((m_vUIDesc[i + 2].x - (m_vUIDesc[1].x - 10.f))) / 200.f);
							break;
						case 1:
							pInstance->Channel_VolumeUp(CHANNEL_PLAYER, ((m_vUIDesc[i + 2].x - (m_vUIDesc[1].x - 10.f))) / 200.f);
							break;
						case 2:
							pInstance->Channel_VolumeUp(CHANNEL_UI, ((m_vUIDesc[i + 2].x - (m_vUIDesc[1].x - 10.f))) / 200.f);
							break;
						case 3:
							pInstance->Channel_VolumeUp(CHANNEL_EFFECT, ((m_vUIDesc[i + 2].x - (m_vUIDesc[1].x - 10.f))) / 200.f);
							break;

						default:
							break;
						}




						break;
					}

				}
			}



		}

	}


	return S_OK;
}

HRESULT CPauseUI::Update_Animation(_float fTimeDelta)
{
	if (m_bIsClicked)
	{
		m_fWalkingFrame += fTimeDelta * 12.f;
		if (m_fWalkingFrame > 9)
			m_fWalkingFrame = 0;
	}

	if (!m_bIsPauseAnimFinished)
	{
		m_fPassedTime += fTimeDelta;
		if (m_bIsOnAnim)
		{
			m_vUIDesc[1].y = GetSingle(CGameInstance)->Easing(TYPE_BounceOut, m_fStartPoint, m_fTargetPoint, m_fPassedTime, 1.5f);
			if (m_fPassedTime > 1.5f)
			{
				m_vUIDesc[1].y = m_fTargetPoint;
				m_bIsPauseAnimFinished = true;
				
			}
		}
		else
		{
			m_vUIDesc[1].x = GetSingle(CGameInstance)->Easing(TYPE_Linear, m_fStartPoint, m_fTargetPoint, m_fPassedTime, 1.5f);
			if (m_fPassedTime > 1.5f)
			{
				m_vUIDesc[1].x = m_fTargetPoint;
				m_bIsPauseAnimFinished = true;
				m_bIsClicked = false;
				m_fDepth = -1;
			}

		}



		m_vUIDesc[2].x = (m_vUIDesc[1].x - 10.f) + GetSingle(CGameInstance)->Get_Channel_Volume(CHANNEL_BGM) * 180.f;
		m_vUIDesc[3].x = (m_vUIDesc[1].x - 10.f) + GetSingle(CGameInstance)->Get_Channel_Volume(CHANNEL_PLAYER) * 180.f;
		m_vUIDesc[4].x = (m_vUIDesc[1].x - 10.f) + GetSingle(CGameInstance)->Get_Channel_Volume(CHANNEL_UI) * 180.f;
		m_vUIDesc[5].x = (m_vUIDesc[1].x - 10.f) + GetSingle(CGameInstance)->Get_Channel_Volume(CHANNEL_EFFECT) * 180.f;


		//m_vUIDesc[2].x = m_vUIDesc[1].x + 80.f; //m_vUIDesc[1].x + 170.f;// m_vUIDesc[1].x - 10.f;
		m_vUIDesc[2].y = m_vUIDesc[1].y - 170;
		//m_vUIDesc[3].x = m_vUIDesc[1].x + 80.f;
		m_vUIDesc[3].y = m_vUIDesc[1].y - 80;
		//m_vUIDesc[4].x = m_vUIDesc[1].x + 80.f;
		m_vUIDesc[4].y = m_vUIDesc[1].y + 10;
		//m_vUIDesc[5].x = m_vUIDesc[1].x + 80.f;
		m_vUIDesc[5].y = m_vUIDesc[1].y + 100;

		m_vUIDesc[6].x = m_vUIDesc[1].x - 90.f;
		m_vUIDesc[6].y = m_vUIDesc[1].y + 190;
		m_vUIDesc[7].x = m_vUIDesc[1].x + 95.f;
		m_vUIDesc[7].y = m_vUIDesc[1].y + 190;
		m_vUIDesc[8].x = m_vUIDesc[1].x;
		m_vUIDesc[8].y = m_vUIDesc[1].y + 240;
	}


	return S_OK;
}

RECT CPauseUI::TransUIDesc_to_Rect(_float4 UIDesc)
{
	RECT tRsult{};

	tRsult.left = (LONG)(UIDesc.x - UIDesc.z*0.5f);
	tRsult.top = (LONG)(UIDesc.y - UIDesc.w*0.5f);
	tRsult.right = (LONG)(UIDesc.x + UIDesc.z*0.5f);
	tRsult.bottom = (LONG)(UIDesc.y + UIDesc.w*0.5f);


	return tRsult;
}





CPauseUI * CPauseUI::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CPauseUI* pInstance = new CPauseUI(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create Pause_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CPauseUI::Clone(void * pArg)
{
	CPauseUI* pInstance = new CPauseUI((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create Pause_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CPauseUI::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
