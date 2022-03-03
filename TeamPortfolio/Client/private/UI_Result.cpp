#include "stdafx.h"
#include "..\public\UI_Result.h"
#include "MyButton.h"
#include "UI_Image.h"
#include "Scene_Loading.h"
#include "UI_RankStar.h"
#include "Player.h"
#include "Inventory.h"

CUI_Result::CUI_Result(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CUI(pGraphicDevice)
{
}

CUI_Result::CUI_Result(const CUI_Result & rhs)
	: CUI(rhs), m_UIPrototypes(rhs.m_UIPrototypes)
{
	for (auto& pair : m_UIPrototypes)
	{
		Safe_AddRef(pair.second);
	}
}

HRESULT CUI_Result::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	CGameInstance* pGameInstance = GetSingle(CGameInstance);

	//SCENEID::SCENE_STATIC
	//m_eNowSceneNum
	//프로토타입 넣는중
	m_UIPrototypes.emplace(TEXT("ProtoType_GameObject_UI_Button"), CMyButton::Create(m_pGraphicDevice));

	//if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_Component_Texture_UI"), CMyButton::Create(m_pGraphicDevice))))
	//	return E_FAIL;
	
	if (FAILED(pGameInstance->Add_GameObject_Prototype(TEXT("ProtoType_GameObject_UI_RankStar"), CUI_RankStar::Create(m_pGraphicDevice))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Result::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_vUIDesc = _float4(g_iWinCX >> 1, g_iWinCY >> 1, g_iWinCX >> 1, 500);

	m_ComTransform->Set_MatrixState(CTransform::STATE_POS, NOT_EXIST_BLOCK);
	

	if (FAILED(Ready_Layer_Button(TEXT("Layer_Button"))))
		return E_FAIL;

	m_ComTexture->Change_TextureLayer(TEXT("Result"));

	//레벨디자인할 때 만져야할 설정들

	m_fMaxTime = 10.f;	//최대 시간 1초
	RankStarGold = 1000.f;	//등급 하나당 획득 골드량

	return S_OK;
}

_int CUI_Result::Update(_float fDeltaTime)
{
	if (FAILED(__super::Update(fDeltaTime)))
		return E_FAIL;
	m_fFrame = fDeltaTime;

	if (m_bStopSwitch == false)
	{
		m_fTimer += m_fFrame;
		TempMinutes = m_fTimer / 60;
		TempSeconds = (_uint)m_fTimer % 60;
	}


	if (m_fTimer > m_fMaxTime || m_bClear == true)
	{
		if (m_bSetupGenerate == false)
		{
			if (FAILED(Set_UI_Transform(m_ComTransform, m_vUIDesc)))
				return E_FAIL;

			if (FAILED(Ready_Layer_RankStar(TEXT("Layer_RankStar"))))
				return E_FAIL;

			SetUp_Player();
			m_bSetupGenerate = true;
		}

		Button_Picking();


		if (FAILED(Update_UIButtonList(fDeltaTime)))
			return E_FAIL;
		


		m_bStopSwitch = true;

	}



	return _int();
}

_int CUI_Result::LateUpdate(_float fDeltaTime)
{
	if (FAILED(__super::LateUpdate(fDeltaTime)))
		return E_FAIL;
	//RENDER_PRIORITY ,RENDER_UI

	//이걸 함으로써 렌더되지 않게 연산량을 줄여주자!!!
	if (m_fTimer > m_fMaxTime || m_bClear == true)
	{
		if (FAILED(m_ComRenderer->Add_RenderGroup(CRenderer::RENDER_UI, this)))
			return E_FAIL;
	}
	if (FAILED(LateUpdate_UIButtonList(fDeltaTime)))
		return E_FAIL;

	return _int();
}

_int CUI_Result::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_ComTransform->Bind_WorldMatrix()))
		return E_FAIL;

	//if (FAILED(m_ComTexture->Bind_Texture_AutoFrame(m_fFrame)))
	//	return E_FAIL;
	if (FAILED(m_ComTexture->Bind_Texture(0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(m_ComVIBuffer->Render()))
		return E_FAIL;

	//폰트 관련된건 여기에 있음
	SetUp_Pont();


	return _int();
}

_int CUI_Result::LateRender()
{
	if (FAILED(__super::LateRender()))
		return E_FAIL;

	return _int();
}

HRESULT CUI_Result::Ready_Layer_Button(const _tchar * pLayerTag)
{
	CMyButton* temp = (CMyButton*)(Find_UI(TEXT("ProtoType_GameObject_UI_Button"))->Clone(&_float4(9999.f, 9999.f, 250, 100)));
	temp->Set_ButtonName(L"Button_Result_Start");
	m_UIButtonList.emplace(L"Button_Result_Start", (CUI*)temp);

	temp = (CMyButton*)(Find_UI(TEXT("ProtoType_GameObject_UI_Button"))->Clone(&_float4(9999.f, 9999.f, 250, 100)));
	temp->Set_ButtonName(L"Button_Result_Cancel");
	m_UIButtonList.emplace(L"Button_Result_Cancel", (CUI*)temp);

	//m_vButtonDesc = { g_iWinCX >> 1,g_iWinCY >> 1, 250 ,100 };

	return S_OK;
}

HRESULT CUI_Result::Update_UIButtonList(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	int hr;
	for (auto pair : m_UIButtonList) {
		hr = (pair.second->Update(fTimeDelta));
		if (hr != SHOP_END)
			break;
			//스위치는 for문 밖에다 쓰도록 하자!
	}
	switch (hr)
	{
	case RESULT_START:
	{
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press)
		{
			POINT ptMouse;
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);
			m_isClicked = false;
			if (PtInRect(&m_fStartButton, ptMouse))
			{
				m_isClicked = true;
				CMyButton* ButtonStart = (CMyButton*)Find_Button(L"Button_Result_Start");
				CTexture* ButtonTexture = (CTexture*)ButtonStart->Get_Component(TEXT("Com_Texture"));
				ButtonTexture->Change_TextureLayer(L"Button_Result_Start3");
			}
		}
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Up)
		{
			CScene* TempScene = pGameInstance->Get_Scene();
			TempScene->Scene_InGame_Chage(true, pGameInstance->Get_NowSceneNum());
		}
		break;
	}
	case RESULT_CANCEL:
	{
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press)
		{
			POINT ptMouse;
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);
			m_isClicked = false;
			if (PtInRect(&m_fCancelButton, ptMouse))
			{
				m_isClicked = true;
				CMyButton* ButtonCancel = (CMyButton*)Find_Button(L"Button_Result_Cancel");
				CTexture* ButtonTexture = (CTexture*)ButtonCancel->Get_Component(TEXT("Com_Texture"));
				ButtonTexture->Change_TextureLayer(L"Button_Result_Cancel3");
			}
		}
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Up)
		{
			CScene* TempScene = pGameInstance->Get_Scene();
			TempScene->Scene_InGame_Chage(true, SCENEID::SCENE_STAGESELECT);
		}
		break;
	}
	default:
	{
		break;
	}
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CUI_Result::LateUpdate_UIButtonList(_float fTimeDelta)
{

	for (auto pair : m_UIButtonList)
	{
		if (FAILED(pair.second->LateUpdate(fTimeDelta)))
			return E_FAIL;






	}
	return S_OK;
}

CUI * CUI_Result::Find_UI(const _tchar * tagUI)
{
	auto iter = find_if(m_UIPrototypes.begin(), m_UIPrototypes.end(), CTagFinder(tagUI));

	if (iter == m_UIPrototypes.end())
		return nullptr;

	return iter->second;
}

CUI * CUI_Result::Find_Button(const _tchar * tagUIList)
{
	auto iter = find_if(m_UIButtonList.begin(), m_UIButtonList.end(), CTagFinder(tagUIList));

	if (iter == m_UIButtonList.end())
		return nullptr;

	return iter->second;
}

HRESULT CUI_Result::Ready_Layer_RankStar(const _tchar * pLayerTag)
{
	CUI_RankStar::RANKSTARDESC	tagRankStarDesc;
	//윈도우 좌표계로 해야함
	//if (TempMinutes <= 1)
	//{
	//	RankNumber = 5;
	//}
	//else if (TempMinutes <= 2)
	//{
	//	RankNumber = 4;
	//}
	//else if (TempMinutes <= 3)
	//{
	//	RankNumber = 3;
	//}
	//else if (TempMinutes <= 4)
	//{
	//	RankNumber = 2;
	//}
	//else
	//{
	//	RankNumber = 1;
	//}
	for (_uint i = 0; i < RankNumber; ++i)
	{
		tagRankStarDesc.WindowRectPos = _float3(450.f+(i*90), 168.f, 0); //렉트의 위치
		tagRankStarDesc.Time = 1.f + i;									//별이 나타나는 시간
		if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(m_eNowSceneNum, pLayerTag, TEXT("ProtoType_GameObject_UI_RankStar"), &tagRankStarDesc))
			return E_FAIL;
	}


	//여기서부터 시작해야됨
	//씬 스테이지2 참고하면 될 듯?
	//획득골드+랭크스타도 만들기 플레이어에 컴 인벤토리로 되어 있고
	//	거기에 셋 골드해서 넣어주면 됨
	//	Get_GameObject_By_LayerIndex이거 사용해서 플레이어 불러오자
	//	1인자에는 스태틱쓰고 2인자에는 레이어 플레이어쓰자 어차피 플레이어는
	//	하나밖에 없으니 디폴트0으로 되어 있는거 하면 될 듯
	//	획득골드량은 시간에 따라서 꼴리는대로 넣어주면 됨

	//	그리고 랭크스타는 미리 갯수 만들어놔서
	//	이동해서 불러오기 또는!!!
	//	클리어시 레디 레이어 랭크스타 함수 호출해서 이 때 만들어줘도 됨
	//	이게 괜찮을듯

	return S_OK;
}

void CUI_Result::Set_RankStar()
{
	RankNumber += 1;
}

HRESULT CUI_Result::SetUp_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	//m_eNowSceneNum = SCENEID::SCENE_LOADING;

	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_ComRenderer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_ComVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_ComTransform, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TEXT("Prototype_Component_UI_Result_Texture"), TEXT("Com_Texture"), (CComponent**)&m_ComTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Result::SetUp_RenderState()
{


	//m_pGraphicDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_XRGB(255, 255, 255));
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	return S_OK;
}

HRESULT CUI_Result::Release_RenderState()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;
	m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphicDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

HRESULT CUI_Result::SetUp_Pont()
{
	//잊지마라 폰트 매니저는 렌더에서만 사용가능하다.
	if (m_fTimer > m_fMaxTime || m_bClear == true)
	{
		if (m_fTimer > m_fMaxTime)
		{
			wstring TempString;

			///////획득 골드량을 표시하는중
			_tchar tempGoldArr[64];
			_itow_s(AcquisitionGold, tempGoldArr, 10);

			TempString = wstring(tempGoldArr);
			GetSingle(CGameInstance)->Render_UI_Font(TempString, { 475.f,285.f }, { 25.f,50.f }, _float3(123, 104, 238));

			////베스트 기록을 표시하는중
			_uint TempBestMinutes = (_uint)TempBestClear / 60;
			_uint TempBestSeconds = (_uint)TempBestClear % 60;
			_tchar tempBestMinArr[64];
			_itow_s(TempBestMinutes, tempBestMinArr, 10);

			_tchar tempBestSecArr[64];
			_itow_s(TempBestSeconds, tempBestSecArr, 10);

			TempString = wstring(tempBestMinArr) + L"Minutes " + wstring(tempBestSecArr) + L"Seconds";

			GetSingle(CGameInstance)->Render_UI_Font(TempString, { 475.f,360.f }, { 25.f,50.f }, _float3(123, 104, 238));

			if (FAILED(Release_RenderState()))
				return E_FAIL;


			//////타이머를 표시하는중
			TempString = L"Mission Failed";

			GetSingle(CGameInstance)->Render_UI_Font(TempString, { 475.f,435.f }, { 30.f,50.f }, _float3(123, 104, 238));

			if (FAILED(Release_RenderState()))
				return E_FAIL;

		}
		else
		{
			wstring TempString;

			///////획득 골드량을 표시하는중
			_tchar tempGoldArr[64];
			_itow_s(AcquisitionGold, tempGoldArr, 10);

			TempString = wstring(tempGoldArr);
			GetSingle(CGameInstance)->Render_UI_Font(TempString, { 475.f,275.f }, { 20.f,30.f }, _float3(83, 250, 120));

			////베스트 기록을 표시하는중
			_uint TempBestMinutes = (_uint)TempBestClear / 60;
			_uint TempBestSeconds = (_uint)TempBestClear % 60;
			_tchar tempBestMinArr[64];
			_itow_s(TempBestMinutes, tempBestMinArr, 10);

			_tchar tempBestSecArr[64];
			_itow_s(TempBestSeconds, tempBestSecArr, 10);

			TempString = wstring(tempBestMinArr) + L"Minutes " + wstring(tempBestSecArr) + L"Seconds";

			GetSingle(CGameInstance)->Render_UI_Font(TempString, { 475.f,350.f }, { 20.f,30.f }, _float3(83, 250, 120));

			if (FAILED(Release_RenderState()))
				return E_FAIL;

			//////타이머를 표시하는중
			_tchar tempMinArr[64];
			_itow_s(TempMinutes, tempMinArr, 10);

			_tchar tempSecArr[64];
			_itow_s(TempSeconds, tempSecArr, 10);

			TempString = wstring(tempMinArr) + L"Minutes " + wstring(tempSecArr) + L"Seconds";

			GetSingle(CGameInstance)->Render_UI_Font(TempString, { 475.f,400.f }, { 20.f,30.f }, _float3(83, 250, 120));

			if (FAILED(Release_RenderState()))
				return E_FAIL;

			//_tchar tempArr[64];
			//_itow_s(m_fTimer, tempArr, 10);

			//wstring TempString = wstring(tempArr) + L"Sec " + L"Eunhyek";

			//GetSingle(CGameInstance)->Render_UI_Font(TempString, { 500.f,400.f }, { 20.f,30.f }, _float3(83, 250, 120));

			//if (FAILED(Release_RenderState()))
			//	return E_FAIL;
		}

	}

	return S_OK;
}
HRESULT CUI_Result::SetUp_Player()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CPlayer* Player = (CPlayer*)pGameInstance->Get_GameObject_By_LayerIndex(SCENEID::SCENE_STATIC, TAG_LAY(Layer_Player)); // 플레이어 정보

	Player->SetBestClear(m_eNowSceneNum, m_fTimer);

	TempBestClear = Player->getBestClear(m_eNowSceneNum);

	CInventory* Inventory = (CInventory*)pGameInstance->Get_Commponent_By_LayerIndex(SCENEID::SCENE_STATIC, TAG_LAY(Layer_Player), TAG_COM(Com_Inventory)); //플레이어의 인벤토리 정보
	AcquisitionGold = RankNumber * RankStarGold;

	Inventory->Set_Gold(AcquisitionGold);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CUI_Result::Button_Picking()
{
	CMyButton* ButtonStart = (CMyButton*)Find_Button(L"Button_Result_Start");
	CTransform* ButtonTransform = (CTransform*)ButtonStart->Get_Component(TEXT("Com_Transform"));
	//ButtonTransform->Set_MatrixState(CTransform::STATE_POS, _float3(100.f, 200.f, 0));
	m_vButtonDesc = { (g_iWinCX >> 1) - 150,(g_iWinCY >> 1) + 150, 250 ,100 };
	_float4 vUIDesc = m_vButtonDesc;

	RECT TempButton;
	TempButton.top = LONG(vUIDesc.y - vUIDesc.w *0.5f);
	TempButton.bottom = LONG(vUIDesc.y + vUIDesc.w *0.5f);
	TempButton.right = LONG(vUIDesc.x + vUIDesc.z*0.5f);
	TempButton.left = LONG(vUIDesc.x - vUIDesc.z*0.5f);
	m_fStartButton = TempButton;
	ButtonStart->Set_Rect(m_vButtonDesc, TempButton);
	ButtonStart->Set_UI_Transform(ButtonTransform, m_vButtonDesc);

	CMyButton* ButtonCancel = (CMyButton*)Find_Button(L"Button_Result_Cancel");
	ButtonTransform = (CTransform*)ButtonCancel->Get_Component(TEXT("Com_Transform"));
	//ButtonTransform->Set_MatrixState(CTransform::STATE_POS, _float3(200.f, -100.f, 0));
	m_vButtonDesc = { (g_iWinCX >> 1) + 150,(g_iWinCY >> 1) + 150, 250 ,100 };
	vUIDesc = m_vButtonDesc;
	TempButton.top = LONG(vUIDesc.y - vUIDesc.w *0.5f);
	TempButton.bottom = LONG(vUIDesc.y + vUIDesc.w *0.5f);
	TempButton.right = LONG(vUIDesc.x + vUIDesc.z*0.5f);
	TempButton.left = LONG(vUIDesc.x - vUIDesc.z*0.5f);
	m_fCancelButton = TempButton;
	ButtonCancel->Set_Rect(m_vButtonDesc, TempButton);
	ButtonCancel->Set_UI_Transform(ButtonTransform, m_vButtonDesc);
}

void CUI_Result::Set_Clear(_bool _bClear)
{
	m_bClear = _bClear;
}

CUI_Result * CUI_Result::Create(LPDIRECT3DDEVICE9 pGraphicDevice, void * pArg)
{
	CUI_Result* pInstance = new CUI_Result(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Fail to Create CUI_Result_ProtoType");
		Safe_Release(pInstance);

	}


	return pInstance;
}

CGameObject * CUI_Result::Clone(void * pArg)
{
	CUI_Result* pInstance = new CUI_Result((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Fail to Create CUI_Result_Clone");
		Safe_Release(pInstance);

	}


	return pInstance;
}

void CUI_Result::Free()
{
	__super::Free();

	for (auto& pair : m_UIButtonList)
	{
		Safe_Release(pair.second);

	}
	m_UIButtonList.clear();

	for (auto& pair : m_UIPrototypes)
	{
		Safe_Release(pair.second);
	}
	m_UIPrototypes.clear();

	Safe_Release(m_ComTexture);
	Safe_Release(m_ComTransform);
	Safe_Release(m_ComVIBuffer);
	Safe_Release(m_ComRenderer);
}
