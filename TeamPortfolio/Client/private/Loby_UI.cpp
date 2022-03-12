#include "stdafx.h"
#include "LobyCube.h"
#include "..\public\Loby_UI.h"
#include "Camera_Main.h"




CLoby_UI::CLoby_UI(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CLoby_UI::CLoby_UI(const CLoby_UI & rhs)
	: CUI(rhs)
{
}

HRESULT CLoby_UI::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLoby_UI::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	FAILED_CHECK(SetUp_UIDesc());

	return S_OK;
}
void CLoby_UI::Set_UI_TransformRect( _float4 vRect)
{
	//vRect.x,y,z,w=top,left,bottom,right
	_float4 vResult{};
	/*너비*/vResult.z = vRect.w-vRect.y;
	/*높이*/vResult.w = vRect.z-vRect.x;
	/*x좌표*/vResult.x = vRect.y + vResult.z*0.5f;
	/*y좌표*/vResult.y = vRect.x + vResult.w*0.5f;
	Set_UI_Transform(m_ComTransform, vResult);
	
}

_int CLoby_UI::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;

	
	FAILED_CHECK(Update_Alpha(fDeltaTime));

	FAILED_CHECK(Input_ManuIndex(fDeltaTime));

	FAILED_CHECK(Update_MouseButton(fDeltaTime));

	if (m_iPageIndex == 1)
	{
		if (m_bIDInput)
			String_Input(m_szID);
		else if(m_bPasswordInput)
			String_Input(m_szPassword);

	}

	return _int();

}




_int CLoby_UI::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;


	if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return E_FAIL;

	return _int();
}

HRESULT CLoby_UI::First_SetUp_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	m_vUIDesc_Logo.x = g_iWinCX *0.5f;
	m_vUIDesc_Logo.y = 180;
	m_vUIDesc_Logo.z = 1000;
	m_vUIDesc_Logo.w = 300;



	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc_Logo)))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(0)))
		return E_FAIL;


	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//Sour => 현재 그리려고하는 그림의 색
	//Dest => 직전까지 화면에 그려진 색
	//
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(_uint(m_fAlphaValue), 255, 255, 255));
	//
	//
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);


	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CLoby_UI::Second_SetUp_RenderState()
{
	m_vUIDesc_Index.x = g_iWinCX * 0.5f;
	m_vUIDesc_Index.y = 415 + m_iManuIndex * 55.f;
	m_vUIDesc_Index.z = 300;
	m_vUIDesc_Index.w = 50;

	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc_Index)))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(1)))
		return E_FAIL;

	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_fIndexAlpha > 255)
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 255);
	else 
		m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, _uint(m_fIndexAlpha));

	


	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoby_UI::Thrid_SetUp_RenderState()
{


	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc_FirstSound)))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_ComTexture->Bind_Texture(2)))
		return E_FAIL;


	if (m_fIndexAlpha > 255)
		m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(0, 255, 255, 255));
	else
		m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255 - _uint(m_fIndexAlpha), 255, 255, 255));





	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;



	if(m_iPageIndex == 3)
		m_vUIDesc_SecondSound.y = 565;
	else
		m_vUIDesc_SecondSound.y = 575;


	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc_SecondSound)))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;


	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoby_UI::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	return S_OK;
}

void CLoby_UI::Set_Particle_LoginFailed()
{
	PARTICLEDESC tDesc;
	tDesc.eParticleID = Particle_Fixed;
	tDesc.TotalParticleTime = 1.f;
	tDesc.EachParticleLifeTime = 1.f;

	tDesc.ParticleSize = _float3(334.f, 280.f, 1.f);

	tDesc.Particle_Power = -600;

	tDesc.PowerRandomRange = _float2(0.5f, 1.5f);

	tDesc.MaxParticleCount = 1;

	tDesc.szTextureProtoTypeTag = TEXT("Prototype_Component_Texture_Particle");

	tDesc.szTextureLayerTag = TEXT("Particle_LoginFailed");

	tDesc.m_bIsTextureAutoFrame = false;

	tDesc.FixedTarget = _float3(g_iWinCX>>1, g_iWinCY>>1, 0);

	tDesc.MaxBoundary = _float3(1280, 720, 1);

	tDesc.ParticleColorChage = false;
	tDesc.TargetColor = _float3(46.f, 231.f, 60.f);
	tDesc.TargetColor2 = _float3(255.f, 0.f, 00.f);

	tDesc.m_bIsUI = true;

	tDesc.ParticleStartRandomPosMin = _float3(0.f, 0.f, 0.f);
	tDesc.ParticleStartRandomPosMax = _float3(0.f, 0.f, 0.f);

	tDesc.MustDraw = true;
	tDesc.IsParticleFameEndtoDie = false;

	tDesc.m_bUIDepth = -1;

	GetSingle(CParticleMgr)->Create_ParticleObject(m_eNowSceneNum, tDesc);
}


_int CLoby_UI::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	FAILED_CHECK(First_SetUp_RenderState());

	if (m_iPageIndex == 0) {

		FAILED_CHECK(Second_SetUp_RenderState());
	}
	else if (m_iPageIndex == 3 || m_iPageIndex == 2) 
	{

		FAILED_CHECK(Thrid_SetUp_RenderState());
	}

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	if (m_fTextFrame - 15 > 0)
		GetSingle(CGameInstance)->Render_UI_Font(L"Shin Joo Yeob\n\nJung Jin Woo\n\nUm Ji Hwan\n\nPark Eun Hyuk", { g_iWinCX - 280,g_iWinCY - 270 }, { 20,30 }, { 83,250,120 }, (_int)(m_fTextFrame - 15));

	if (m_iPageIndex == 1 && !m_IsSceneChange)
	{
		GetSingle(CGameInstance)->Render_UI_Font(m_szID, { g_iWinCX *0.5f - 90, 455 }, { 20,30 }, { 15,40,30 });

		_uint stringLen = (_uint)(m_szPassword.length());
		wstring szHideString = L"";
		for (_uint i = 0; i < stringLen;i++)
		{
			szHideString += L"*";
		}

		GetSingle(CGameInstance)->Render_UI_Font(szHideString, { g_iWinCX *0.5f - 90, 573 }, { 20,30 }, { 15,40,30 });
	}


	return _int();
}

_int CLoby_UI::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CLoby_UI::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_Texture_LobyUI"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;
	

	m_pLobyCube = (CLobyCube*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_LobyCube")));

	if (m_pLobyCube == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoby_UI::SetUp_UIDesc()
{
	m_vUIDesc_Logo.x = g_iWinCX *0.5f;
	m_vUIDesc_Logo.y = 180;
	m_vUIDesc_Logo.z = 1000;
	m_vUIDesc_Logo.w = 300;

	m_fAlphaValue = 0;
	m_fTextFrame = 0;
	m_fIndexAlpha = 255.f;
	m_IsSceneChange = false;
	if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc_Logo)))
		return E_FAIL;


	m_vUIDesc_FirstSound.x = g_iWinCX * 0.5f;
	m_vUIDesc_FirstSound.y = 450;
	m_vUIDesc_FirstSound.z = 50;
	m_vUIDesc_FirstSound.w = 30;

	m_vUIDesc_SecondSound.x = g_iWinCX * 0.5f;

	if (m_iPageIndex == 3)
		m_vUIDesc_SecondSound.y = 565;
	else
		m_vUIDesc_SecondSound.y = 575;

	m_vUIDesc_SecondSound.z = 50;
	m_vUIDesc_SecondSound.w = 30;

	return S_OK;
}

HRESULT CLoby_UI::Update_MouseButton(_float fTimeDelta)
{
	CGameInstance* pInstance = GetSingle(CGameInstance);

	if (m_iPageIndex == 2 || m_iPageIndex == 3) {

		//MOUSEBUTTONSTATE{ MBS_LBUTTON, MBS_RBUTTON, MBS_WHEEL, MBS_END };
		if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press)
		{

			if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
			{
				POINT ptMouse;
				GetCursorPos(&ptMouse);
				ScreenToClient(g_hWnd, &ptMouse);


				m_bIsFirstSoundClicked = false;
				m_bIsSecondSoundClicked = false;


				if (PtInRect(&TransUIDesc_to_Rect(m_vUIDesc_FirstSound), ptMouse))
				{

					m_bIsFirstSoundClicked = true;

				}
				else if (PtInRect(&TransUIDesc_to_Rect(m_vUIDesc_SecondSound), ptMouse))
				{
					m_bIsSecondSoundClicked = true;
				}


			}
			else if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Up)
			{
				m_bIsFirstSoundClicked = false;
				m_bIsSecondSoundClicked = false;
			}


			//enum MOUSEMOVESTATE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };
			if (m_bIsFirstSoundClicked)
			{
				m_vUIDesc_FirstSound.x += (pInstance->Get_DIMouseMoveState(CInput_Device::MMS_X));

				if (m_vUIDesc_FirstSound.x < g_iWinCX * 0.5f - 100)
					m_vUIDesc_FirstSound.x = g_iWinCX * 0.5f - 100;
				if (m_vUIDesc_FirstSound.x > g_iWinCX * 0.5f + 100)
					m_vUIDesc_FirstSound.x = g_iWinCX * 0.5f + 100;

				if (m_iPageIndex == 3)
					pInstance->Channel_VolumeUp(CHANNEL_BGM, ((m_vUIDesc_FirstSound.x - (g_iWinCX * 0.5f - 100))) / 200.f);

				if (m_iPageIndex == 2)
					pInstance->Channel_VolumeUp(CHANNEL_UI, ((m_vUIDesc_FirstSound.x - (g_iWinCX * 0.5f - 100))) / 200.f);


			}
			else if (m_bIsSecondSoundClicked)
			{

				m_vUIDesc_SecondSound.x += (pInstance->Get_DIMouseMoveState(CInput_Device::MMS_X));

				if (m_vUIDesc_SecondSound.x < g_iWinCX * 0.5f - 100)
					m_vUIDesc_SecondSound.x = g_iWinCX * 0.5f - 100;
				if (m_vUIDesc_SecondSound.x > g_iWinCX * 0.5f + 100)
					m_vUIDesc_SecondSound.x = g_iWinCX * 0.5f + 100;

				if (m_iPageIndex == 3)
					pInstance->Channel_VolumeUp(CHANNEL_PLAYER, ((m_vUIDesc_SecondSound.x - (g_iWinCX * 0.5f - 100))) / 200.f);

				if (m_iPageIndex == 2)
					pInstance->Channel_VolumeUp(CHANNEL_EFFECT, ((m_vUIDesc_SecondSound.x - (g_iWinCX * 0.5f - 100))) / 200.f);




			}




		}
	}

	else if (m_iPageIndex == 1)
	{

		if (pInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
		{
			m_bIDInput = false;
			m_bPasswordInput = false;


			POINT ptMouse;
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);



			if (PtInRect(&TransUIDesc_to_Rect(_float4(g_iWinCX *0.5f - 90, 450, 200, 100)), ptMouse))
			{

				m_bIDInput = true;

			}
			else if (PtInRect(&TransUIDesc_to_Rect({ g_iWinCX *0.5f - 90, 570, 200, 100 }), ptMouse))
			{
				m_bPasswordInput = true;
			}


		}

	}
	return S_OK;
}

HRESULT CLoby_UI::Update_Alpha(_float fTimeDelta)
{
	if (m_fAlphaValue < 255) {

		m_fAlphaValue += fTimeDelta * 85.f;
		if (m_fAlphaValue > 255)
			m_fAlphaValue = 255;
	}
	m_fTextFrame += fTimeDelta * 5;

	if (m_fIndexAlpha > 0)
	{
		m_fIndexAlpha -= fTimeDelta * 255.f;
		if (m_fIndexAlpha < 0)
			m_fIndexAlpha = 0;
	}

	return S_OK;
}

HRESULT CLoby_UI::Input_ManuIndex(_float fTimeDelta)
{
	CGameInstance* pInstace = GetSingle(CGameInstance);


	if (!m_fIndexAlpha)
	{
		if (pInstace->Get_DIKeyState(DIK_RETURN) & DIS_Down)
		{
			if (m_iPageIndex == 0)
			{
				GetSingle(CGameInstance)->PlaySound(L"JW_loby_enter.wav", CHANNEL_UI,2.f);
				switch (m_iManuIndex)
				{
				case 0:
				{
					m_iPageIndex = 1;
					m_iManuIndex = 0;
					m_fIndexAlpha = 510.f;
					m_szID = L"";
					m_szPassword = L"";

					m_pLobyCube->Strat_Turning(0);
					m_bIsNewResgist = true;
				}
				break;
				case 1:
				{
					m_iPageIndex = 1;
					m_iManuIndex = 1;
					m_szID = L"";
					m_szPassword = L"";

					m_fIndexAlpha = 510.f;
					m_pLobyCube->Strat_Turning(0);
				}
				break;
				case 2:
				{
					m_iPageIndex = 3;
					m_iManuIndex = 2;
					m_fIndexAlpha = 510.f;

					m_vUIDesc_FirstSound.x = g_iWinCX * 0.5f - 100 + 200 * GetSingle(CGameInstance)->Get_Channel_Volume(CHANNEL_BGM);
					m_vUIDesc_SecondSound.x = g_iWinCX * 0.5f - 100 + 200 * GetSingle(CGameInstance)->Get_Channel_Volume(CHANNEL_PLAYER);

					m_pLobyCube->Strat_Turning(1);
				}
				break;
				case 3:
					PostQuitMessage(NULL);
					break;
				default:
					break;
				}

			}

			else if (m_iPageIndex == 1)
			{

				_uint bIsSuccess = false;


				if (m_bIsNewResgist)
				{
					FAILED_CHECK(GetSingle(CLoginMgr)->Load_PlayerData(m_szID, m_szPassword, &bIsSuccess));

					if (bIsSuccess)
					{
						m_bIsNewResgist = false;
						bIsSuccess = false;
					}
					else {

						FAILED_CHECK(GetSingle(CLoginMgr)->Create_PlayerData(m_szID, m_szPassword, &bIsSuccess));
					}
				}
				else {
					FAILED_CHECK(GetSingle(CLoginMgr)->Load_PlayerData(m_szID, m_szPassword, &bIsSuccess));
				}

				if (bIsSuccess == 1 || m_bIsNewResgist) //로그인 성공 or 신규 가입시 씬 전환
				{
					GetSingle(CGameInstance)->PlaySound(L"JW_startgame.wav", CHANNEL_UI);
					CCamera_Main* pMainCam = (CCamera_Main*)(GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
					if (pMainCam == nullptr)
						return E_FAIL;
					pMainCam->CameraEffect(CCamera_Main::CAM_EFT_FADE_IN, fTimeDelta, 1.8f);
					m_pLobyCube->Rot_N_SceneChange(fTimeDelta);
					m_fIndexAlpha = 9999999.f;
					m_IsSceneChange = true;
				}
				else //로그인 실패시
				{
					m_iPageIndex = 0;
					m_fIndexAlpha = 510.f;
					m_iManuIndex = 0;
					Set_Particle_LoginFailed();
					GetSingle(CGameInstance)->PlaySound(L"EH_WindowError.mp3", CHANNEL_UI);
					m_pLobyCube->Strat_Turning(1);

				}

			}
			else if (m_iPageIndex == 2)
			{
				GetSingle(CGameInstance)->PlaySound(L"JW_loby_enter.wav", CHANNEL_UI, 2.f);
				m_iPageIndex = 0;
				m_fIndexAlpha = 510.f;
				m_iManuIndex = 2;
				m_pLobyCube->Strat_Turning(2);
			}
			else if (m_iPageIndex == 3)
			{
				GetSingle(CGameInstance)->PlaySound(L"JW_loby_enter.wav", CHANNEL_UI, 2.f);
				m_iPageIndex = 2;
				m_fIndexAlpha = 510.f;
				m_iManuIndex = 2;

				m_vUIDesc_FirstSound.x = g_iWinCX * 0.5f - 100 + 200 * GetSingle(CGameInstance)->Get_Channel_Volume(CHANNEL_UI);
				m_vUIDesc_SecondSound.x = g_iWinCX * 0.5f - 100 + 200 * GetSingle(CGameInstance)->Get_Channel_Volume(CHANNEL_EFFECT);

				m_pLobyCube->Strat_Turning(1);
			}
		}
		if (pInstace->Get_DIKeyState(DIK_UP) & DIS_Down)
		{
			GetSingle(CGameInstance)->PlaySound(L"JW_lobyselect.wav", CHANNEL_UI,2.f);
			m_fIndexAlpha = 255.f;
			m_iManuIndex--;
			if (m_iManuIndex < 0)
				m_iManuIndex = 0;
		}
		else if (pInstace->Get_DIKeyState(DIK_DOWN) & DIS_Down)
		{
			GetSingle(CGameInstance)->PlaySound(L"JW_lobyselect.wav", CHANNEL_UI,2.f);
			m_fIndexAlpha = 255.f;
			m_iManuIndex++;
			if (m_iManuIndex > 3)
				m_iManuIndex = 3;
		}
	}

	return S_OK;
}

HRESULT CLoby_UI::String_Input(wstring& wString)
{
	_uint iKey = 0;

	CGameInstance* pInstance = GetSingle(CGameInstance);

	if (pInstance->Get_DIKeyState(DIK_BACKSPACE) & DIS_Down)
	{
		wString = wString.substr(0, wString.length() - 1);
	}
	else {
		if (pInstance->Get_DIKeyState(DIK_0) & DIS_Down) iKey = 48;
		else if (pInstance->Get_DIKeyState(DIK_1) & DIS_Down) iKey = 49;
		else if (pInstance->Get_DIKeyState(DIK_2) & DIS_Down) iKey = 50;
		else if (pInstance->Get_DIKeyState(DIK_3) & DIS_Down) iKey = 51;
		else if (pInstance->Get_DIKeyState(DIK_4) & DIS_Down) iKey = 52;
		else if (pInstance->Get_DIKeyState(DIK_5) & DIS_Down) iKey = 53;
		else if (pInstance->Get_DIKeyState(DIK_6) & DIS_Down) iKey = 54;
		else if (pInstance->Get_DIKeyState(DIK_7) & DIS_Down) iKey = 55;
		else if (pInstance->Get_DIKeyState(DIK_8) & DIS_Down) iKey = 56;
		else if (pInstance->Get_DIKeyState(DIK_9) & DIS_Down) iKey = 57;

		else if (pInstance->Get_DIKeyState(DIK_A) & DIS_Down) iKey = 97;
		else if (pInstance->Get_DIKeyState(DIK_B) & DIS_Down) iKey = 98;
		else if (pInstance->Get_DIKeyState(DIK_C) & DIS_Down) iKey = 99;
		else if (pInstance->Get_DIKeyState(DIK_D) & DIS_Down) iKey = 100;
		else if (pInstance->Get_DIKeyState(DIK_E) & DIS_Down) iKey = 101;
		else if (pInstance->Get_DIKeyState(DIK_F) & DIS_Down) iKey = 102;
		else if (pInstance->Get_DIKeyState(DIK_G) & DIS_Down) iKey = 103;
		else if (pInstance->Get_DIKeyState(DIK_H) & DIS_Down) iKey = 104;
		else if (pInstance->Get_DIKeyState(DIK_I) & DIS_Down) iKey = 105;
		else if (pInstance->Get_DIKeyState(DIK_J) & DIS_Down) iKey = 106;
		else if (pInstance->Get_DIKeyState(DIK_K) & DIS_Down) iKey = 107;
		else if (pInstance->Get_DIKeyState(DIK_L) & DIS_Down) iKey = 108;
		else if (pInstance->Get_DIKeyState(DIK_M) & DIS_Down) iKey = 109;
		else if (pInstance->Get_DIKeyState(DIK_N) & DIS_Down) iKey = 110;
		else if (pInstance->Get_DIKeyState(DIK_O) & DIS_Down) iKey = 111;
		else if (pInstance->Get_DIKeyState(DIK_P) & DIS_Down) iKey = 112;
		else if (pInstance->Get_DIKeyState(DIK_Q) & DIS_Down) iKey = 113;
		else if (pInstance->Get_DIKeyState(DIK_R) & DIS_Down) iKey = 114;
		else if (pInstance->Get_DIKeyState(DIK_S) & DIS_Down) iKey = 115;
		else if (pInstance->Get_DIKeyState(DIK_T) & DIS_Down) iKey = 116;
		else if (pInstance->Get_DIKeyState(DIK_U) & DIS_Down) iKey = 117;
		else if (pInstance->Get_DIKeyState(DIK_V) & DIS_Down) iKey = 118;
		else if (pInstance->Get_DIKeyState(DIK_W) & DIS_Down) iKey = 119;
		else if (pInstance->Get_DIKeyState(DIK_X) & DIS_Down) iKey = 120;
		else if (pInstance->Get_DIKeyState(DIK_Y) & DIS_Down) iKey = 121;
		else if (pInstance->Get_DIKeyState(DIK_Z) & DIS_Down) iKey = 122;

		if (iKey >= 97 && pInstance->Get_DIKeyState(DIK_LSHIFT) & DIS_Press)
			iKey -= 32;

		if (iKey != 0)
		{

			_tchar tempKeyChar = iKey;

			wString += tempKeyChar;
		}
	}

	return S_OK;
}






RECT CLoby_UI::TransUIDesc_to_Rect(_float4 UIDesc)
{
	RECT tRsult{};

	tRsult.left =(LONG)( UIDesc.x  - UIDesc.z*0.5f);
	tRsult.top = (LONG)(UIDesc.y - UIDesc.w*0.5f);
	tRsult.right = (LONG)(UIDesc.x + UIDesc.z*0.5f);
	tRsult.bottom = (LONG)(UIDesc.y + UIDesc.w*0.5f);


	return tRsult;
}

CLoby_UI * CLoby_UI::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CLoby_UI* pInstance = new CLoby_UI(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create LobyUI_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CLoby_UI::Clone(void * pArg)
{
	CLoby_UI* pInstance = new CLoby_UI((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create LobyUI_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CLoby_UI::Free()
{
	__super::Free();


	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
