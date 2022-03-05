#include "..\Public\GameInstance.h"
#include "Graphic_Device.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "ThreadMgr.h"
#include "ObjectMgr.h"
#include "GameObject.h"
#include "ComponentMgr.h"
#include "EasingMgr.h"
#include "Picking.h"
#include "FrustumMgr.h"
#include "SoundMgr.h"
#include "FontMgr.h"
#include "XMLMgr.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	:m_pThreadMgr(GetSingle(CThreadMgr)), 
	m_pTimerMgr(GetSingle(CTimeMgr)),
	m_pGraphicDevice(GetSingle(CGraphic_Device)),
	m_pSceneMgr(GetSingle(CSceneMgr)),
	m_pObjectMgr(GetSingle(CObjectMgr)),
	m_pComponenetMgr(GetSingle(CComponentMgr)),
	m_pInputDevice(GetSingle(CInput_Device)),
	m_pImguiMgr(GetSingle(CImguiMgr)),
	m_pEasingMgr(GetSingle(CEasingMgr)),
	m_pPickingMgr(GetSingle(CPicking)),
	m_pFrustumMgr(GetSingle(CFrustumMgr)),
	m_pSoundMgr(GetSingle(CSoundMgr)),
	m_pFontMgr(GetSingle(CFontMgr)),
	m_pXMLMgr(GetSingle(CXMLMgr))

{
	m_pThreadMgr->AddRef();
	m_pTimerMgr->AddRef();
	m_pGraphicDevice->AddRef();
	m_pSceneMgr->AddRef();
	m_pObjectMgr->AddRef();
	m_pComponenetMgr->AddRef();
	m_pInputDevice->AddRef();
	m_pEasingMgr->AddRef();
	m_pImguiMgr->AddRef();
	m_pPickingMgr->AddRef();
	m_pFrustumMgr->AddRef();
	m_pSoundMgr->AddRef();
	m_pFontMgr->AddRef();
	m_pXMLMgr->AddRef();
	
}


HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, const CGraphic_Device::GRAPHICDESC & GraphicDesc, _uint iMaxSceneNum, LPDIRECT3DDEVICE9 * ppOut, _float fDoubleInterver)
{
	if (m_pGraphicDevice == nullptr || m_pObjectMgr == nullptr || m_pComponenetMgr == nullptr ||
		m_pSceneMgr == nullptr || m_pPickingMgr == nullptr || m_pFrustumMgr == nullptr || m_pSoundMgr == nullptr || m_pFontMgr == nullptr)
		return E_FAIL;

	//if (FAILED(m_pSeverMgr->ConnectSever()))
	//{
	//	MSGBOX("Failed to Connecting Sever");
	//}

	if (FAILED(m_pGraphicDevice->InitDevice(GraphicDesc, ppOut)))
		return E_FAIL;

	if (FAILED(m_pObjectMgr->Reserve_Container(iMaxSceneNum)))
		return E_FAIL;

	if (FAILED(m_pInputDevice->Ready_Input_Device(hInst, GraphicDesc.hWnd, fDoubleInterver)))
		return E_FAIL;

	if (FAILED(m_pComponenetMgr->Reserve_Container(iMaxSceneNum)))
		return E_FAIL;

	if (FAILED(m_pPickingMgr->Initialize(*ppOut, GraphicDesc.hWnd, nullptr)))
		return E_FAIL;


	FAILED_CHECK(m_pFrustumMgr->Initialize_FrustumMgr(*ppOut));

	FAILED_CHECK(m_pSoundMgr->Initialize_FMOD());

	FAILED_CHECK(m_pFontMgr->Initialize_FontMgr(*ppOut, TEXT("Font2.txt"), { _float(GraphicDesc.iWinCX), _float(GraphicDesc.iWinCY) }));

	FAILED_CHECK(m_pXMLMgr->Initialize_XMLMgr(*ppOut));


	return S_OK;
}

HRESULT CGameInstance::Initialize_Engine_Tool(const CGraphic_Device::GRAPHICDESC & GraphicDesc, _uint iMaxSceneNum, LPDIRECT3DDEVICE9 * ppOut)
{
	if (m_pGraphicDevice == nullptr || m_pObjectMgr == nullptr || m_pComponenetMgr == nullptr || m_pSceneMgr == nullptr)
		return E_FAIL;

	if (FAILED(m_pGraphicDevice->InitDevice(GraphicDesc, ppOut)))
		return E_FAIL;

	if (FAILED(m_pObjectMgr->Reserve_Container(iMaxSceneNum)))
		return E_FAIL;

	if (FAILED(m_pComponenetMgr->Reserve_Container(iMaxSceneNum)))
		return E_FAIL;

	if (FAILED(m_pPickingMgr->Initialize(*ppOut, GraphicDesc.hWnd,nullptr)))
		return E_FAIL;

	return S_OK;
}

_int CGameInstance::Update_Engine(_float fDeltaTime)
{
	if (m_pSceneMgr == nullptr || m_pObjectMgr == nullptr || m_pSoundMgr == nullptr)
		return -1;


	if (FAILED(m_pInputDevice->SetUp_InputDeviceState(fDeltaTime)))
		return -1;

	FAILED_CHECK(m_pSoundMgr->Update_FMOD(fDeltaTime));

	if (m_pSceneMgr->Update(fDeltaTime) < 0)
		return -1;

	if (m_pObjectMgr->Update(fDeltaTime) < 0)
		return -1;

	FAILED_CHECK(SetUp_WorldFrustumPlane());


	return 0;
}

_int CGameInstance::LateUpdate_Engine(_float fDeltaTime)
{
	if (m_pObjectMgr->LateUpdate(fDeltaTime) < 0)
		return -1;
	if (m_pSceneMgr->LateUpdate(fDeltaTime) < 0)
		return -1;

	return 0;
}

_int CGameInstance::Update_Engine_Tool(_float fDeltaTime)
{
	if (m_pSceneMgr == nullptr || m_pObjectMgr == nullptr)
		return -1;


//	if (m_pSceneMgr->Update(fDeltaTime) < 0)
//		return -1;

	if (m_pObjectMgr->Update(fDeltaTime) < 0)
		return -1;




	if (m_pObjectMgr->LateUpdate(fDeltaTime) < 0)
		return -1;

//	if (m_pSceneMgr->LateUpdate(fDeltaTime) < 0)
//		return -1;


	return 0;
}

HRESULT CGameInstance::Clear_Scene_Resource(_uint eSceneNum)
{
	if (m_pObjectMgr == nullptr || m_pComponenetMgr == nullptr)
		return E_FAIL;


	if (FAILED(m_pComponenetMgr->Clear_Scene_Componenets(eSceneNum)))
		return E_FAIL;

	if(FAILED(m_pObjectMgr->Clear_Scene_GameObjects(eSceneNum)))
		return E_FAIL;


	return S_OK;
}

HRESULT CGameInstance::Add_GameObject_Prototype(const _tchar * tagPrototype, CGameObject * pPrototype)
{
	if (m_pObjectMgr == nullptr)
		return E_FAIL;

	return	m_pObjectMgr->Add_GameObject_Prototype(tagPrototype, pPrototype);
}

HRESULT CGameInstance::Add_GameObject_To_Layer(_uint eSceneNum, const _tchar * tagLayer, const _tchar * tagPrototype, void * pArg)
{

	if (m_pObjectMgr == nullptr) 
		return E_FAIL;


	return	m_pObjectMgr->Add_GameObject_To_Layer(eSceneNum, tagLayer, tagPrototype, pArg);
}

CComponent* CGameInstance::Get_Commponent_By_LayerIndex(_uint eSceneNum, const _tchar * tagLayer, const _tchar* tagComponet, _uint iLayerIndex)
{
	if (tagComponet == nullptr || tagLayer == nullptr|| m_pObjectMgr == nullptr)
		return nullptr;

	return m_pObjectMgr->Get_Commponent_By_LayerIndex(eSceneNum, tagLayer, tagComponet,iLayerIndex);
}

CGameObject * CGameInstance::Get_GameObject_By_LayerIndex(_uint eSceneNum, const _tchar * tagLayer, _uint iLayerIndex)
{
	if (tagLayer == nullptr || m_pObjectMgr == nullptr)
		return nullptr;

	return m_pObjectMgr->Get_GameObject_By_LayerIndex(eSceneNum, tagLayer,iLayerIndex);
}

list<CGameObject*>* CGameInstance::Get_ObjectList_from_Layer(_uint eSceneNum, const _tchar * tagLayer)
{
	if (tagLayer == nullptr || m_pObjectMgr == nullptr)
		return nullptr;

	return m_pObjectMgr->Get_ObjectList_from_Layer(eSceneNum, tagLayer);
}

CGameObject * CGameInstance::Change_Camera_Ortho_By_LayerIndex(_uint eSceneNum, const _tchar * tagLayer, _uint iLayerIndex)
{
	if (tagLayer == nullptr || m_pObjectMgr == nullptr)
		return nullptr;

	return m_pObjectMgr->Change_Camera_Ortho_By_LayerIndex(eSceneNum, tagLayer, iLayerIndex);
}

HRESULT CGameInstance::Delete_GameObject_To_Layer_Index(_uint eSceneNum, const _tchar * tagLayer, _uint index)
{
	if (tagLayer == nullptr || m_pObjectMgr == nullptr)
		return E_FAIL;
	

	return Delete_GameObject_To_Layer_Index(eSceneNum, tagLayer, index);
}

HRESULT CGameInstance::Delete_GameObject_To_Layer_Object(_uint eSceneNum, const _tchar * tagLayer, CGameObject * obj)
{
	if (tagLayer == nullptr || m_pObjectMgr == nullptr)
		return E_FAIL;

	return Delete_GameObject_To_Layer_Object(eSceneNum, tagLayer, obj);
}



_float CGameInstance::Get_DeltaTime(const _tchar * tagTimer)
{
	if (nullptr == m_pTimerMgr)
		return -1.f;

	return m_pTimerMgr->Get_DeltaTime(tagTimer);
}

HRESULT CGameInstance::Add_Timer(const _tchar * tagTimer)
{
	if (nullptr == m_pTimerMgr)
		return E_FAIL;

	return m_pTimerMgr->Add_Timer(tagTimer);
}

void CGameInstance::Render_Begin(D3DXCOLOR backbuffer)
{
	if (m_pGraphicDevice == nullptr)
		return;

	m_pGraphicDevice->Render_Begin(backbuffer);
}

void CGameInstance::Render_End()
{
	if (m_pGraphicDevice == nullptr)
		return;

	m_pGraphicDevice->Render_End();
}

HRESULT CGameInstance::Scene_Change(CScene * pScene, _int iNextSceneIdx, _bool bIsNotLoadingScene)
{

	if (m_pSceneMgr == nullptr || m_pObjectMgr == nullptr)
		return E_FAIL;

	if(FAILED(m_pSceneMgr->Scene_Chage(pScene, iNextSceneIdx, bIsNotLoadingScene)))
		return E_FAIL;

	if(FAILED(m_pObjectMgr->Set_StaticGameObject_NowSceneNum(iNextSceneIdx)))
		return E_FAIL;

	return S_OK;
}

_int CGameInstance::Render_Scene()
{
	if (m_pSceneMgr == nullptr)
		return -1;

	if (m_pSceneMgr->Render() < 0)
		return -1;

	if (m_pSceneMgr->LateRender() < 0)
		return -1;

	return 0;
}

_uint CGameInstance::Get_NowSceneNum()
{
	if (m_pSceneMgr == nullptr)
		return -1;


	return m_pSceneMgr->Get_NowSceneNum();
}

void CGameInstance::Set_iNowSceneIndx_ExceptLoad(_int iSceneNum)
{

	if (m_pSceneMgr == nullptr)
		return;


	return m_pSceneMgr->Set_iNowSceneIndx_ExceptLoad(iSceneNum);
}

CScene * CGameInstance::Get_Scene()
{
	return m_pSceneMgr->Get_Scene();
}

HRESULT CGameInstance::Add_Component_Prototype(_uint eSceneIdx, const _tchar * tagPrototypeComponent, CComponent * pComponenet)
{
	if (m_pComponenetMgr == nullptr)
		return E_FAIL;

	return m_pComponenetMgr->Add_Component_Prototype(eSceneIdx, tagPrototypeComponent,pComponenet);
}

CComponent * CGameInstance::Clone_Component(_uint eSceneIdx, const _tchar * tagPrototypeComponent, void * pArg)
{
	if (m_pComponenetMgr == nullptr)
		return nullptr;


	return m_pComponenetMgr->Clone_Component(eSceneIdx, tagPrototypeComponent, pArg);
}

HRESULT CGameInstance::PlayThread(void * _ThreadFunc, void * _pArg)
{
	if (m_pThreadMgr == nullptr)
		return E_FAIL;

	return m_pThreadMgr->PlayThread(_ThreadFunc, _pArg);
}

_byte CGameInstance::Get_DIKeyState(_ubyte eKeyID)
{
	if (m_pInputDevice == nullptr)
		return 0;

	return m_pInputDevice->Get_DIKeyState(eKeyID);
}

_long CGameInstance::Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState)
{
	if (m_pInputDevice == nullptr)
		return 0;

	return m_pInputDevice->Get_DIMouseMoveState(eMouseMoveState);
}

_byte CGameInstance::Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState)
{
	if (m_pInputDevice == nullptr)
		return 0;

	return m_pInputDevice->Get_DIMouseButtonState(eMouseButtonState);
}

_float CGameInstance::Easing(_uint eEasingType, _float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	if (m_pEasingMgr == nullptr)
		return 0;

	return m_pEasingMgr->Easing(eEasingType, fStartPoint, fTargetPoint, fPassedTime, fTotalTime);
}

HRESULT CGameInstance::SetUp_WorldFrustumPlane()
{
	if (m_pFrustumMgr == nullptr)
		return E_FAIL;

	return m_pFrustumMgr->SetUp_WorldFrustumPlane();
}

_bool CGameInstance::IsNeedToRender(_float3 vWorldPosition, _float fLenth)
{
	NULL_CHECK_MSG(m_pFrustumMgr,L"Not Have FrustumMgr");

	return m_pFrustumMgr->IsNeedToRender(vWorldPosition, fLenth);
}

_int CGameInstance::Channel_VolumeUp(CHANNELID eID, _float _vol)
{

	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");
	return m_pSoundMgr->Channel_VolumeUp(eID, _vol);
}

_int CGameInstance::Channel_VolumeDown(CHANNELID eID, _float _vol)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");

	return m_pSoundMgr->Channel_VolumeDown(eID,_vol);
}

_int CGameInstance::Channel_Pause(CHANNELID eID)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");

	return m_pSoundMgr->Channel_Pause(eID);
}

HRESULT CGameInstance::PlaySound(TCHAR * pSoundKey, CHANNELID eID ,_float fLouderMultiple)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");

	return m_pSoundMgr->PlaySound(pSoundKey, eID, fLouderMultiple);
}

HRESULT CGameInstance::PlayBGM(TCHAR * pSoundKey, _float fLouderMultiple)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");

	return m_pSoundMgr->PlayBGM(pSoundKey, fLouderMultiple);
}

void CGameInstance::Stop_ChannelSound(CHANNELID eID)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");

	m_pSoundMgr->Stop_ChannelSound(eID);

	return;
}

void CGameInstance::Stop_AllChannel()
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");
	m_pSoundMgr->Stop_AllChannel();
	return;
}

_float CGameInstance::Get_Channel_Volume(CHANNELID eID)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");
	return 	m_pSoundMgr->Get_Channel_Volume(eID);
}

_bool CGameInstance::Get_Channel_IsPaused(CHANNELID eID)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");
	return 	m_pSoundMgr->Get_Channel_IsPaused(eID);
}

HRESULT CGameInstance::Render_UI_Font(wstring szString, _float2 vOnWindowPos, _float2 vFontSize , _float3 Color_RGB , _int UntilDrawIndex)
{
	NULL_CHECK_BREAK(m_pFontMgr);

	
	//GetSingle(CGameInstance)->PlaySound(L"EH_Mission_Failed.wav", CHANNEL_UI);

	return m_pFontMgr->Render_UI_Font(szString, vOnWindowPos, vFontSize, Color_RGB, UntilDrawIndex);
}

HRESULT CGameInstance::Render_World_Font(wstring szString, _float3 vOnWorldPos, _float2 vFontSize, _float3 Color_RGB, _int UntilDrawIndex)
{
	NULL_CHECK_BREAK(m_pFontMgr);

	return m_pFontMgr->Render_World_Font(szString, vOnWorldPos, vFontSize, Color_RGB, UntilDrawIndex);
}

CVIBuffer_Parsed * CGameInstance::Create_ParsedObject(wstring pPosTextFilePath, wstring pIndexTextFilePath)
{
	NULL_CHECK_BREAK(m_pXMLMgr);

	return m_pXMLMgr->Create_ParsedObject(pPosTextFilePath,pIndexTextFilePath);
}





CImguiMgr * CGameInstance::GetIMGui()
{
	return m_pImguiMgr->GetInstance();
}

HRESULT CGameInstance::Update_Transform_ToWorldSpace(POINT p)
{
	NULL_CHECK_BREAK(m_pPickingMgr);
	return m_pPickingMgr->Transform_ToWorldSpace(p);
}

_bool CGameInstance::isPick(_float3 * pLocalPoint, _float3 * pOut,_float2* pUV)
{
	NULL_CHECK_BREAK(m_pPickingMgr);
	return m_pPickingMgr->isPick_UV(pLocalPoint,pOut, pUV);
}


void CGameInstance::Release_Engine()
{


	if (0 != GetSingle(CGameInstance)->DestroyInstance())
		MSGBOX("Failed to Release Com GameInstance");

	if (0 != GetSingle(CThreadMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com ThreadMgr ");

	if (0 != GetSingle(CEasingMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com EasingMgr ");

	if (0 != GetSingle(CPicking)->DestroyInstance())
		MSGBOX("Failed to Release Com CPicking ");

	if (0 != GetSingle(CObjectMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com ObjectMgr ");

	if (0 != GetSingle(CSceneMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com SceneMgr ");

	if (0 != GetSingle(CComponentMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com ComponentMgr");

	if (0 != GetSingle(CFrustumMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com FrustumMgr ");

	if (0 != GetSingle(CInput_Device)->DestroyInstance())
		MSGBOX("Failed to Release Com CInput_Device ");

	if (0 != GetSingle(CGraphic_Device)->DestroyInstance())
		MSGBOX("Failed to Release Com Graphic_Device ");

	if (0 != GetSingle(CTimeMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com TimeMgr ");

	if (0 != GetSingle(CSoundMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com CSoundMgr ");
	
	if (0 != GetSingle(CXMLMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com XMLMgr ");

	if (0 != GetSingle(CFontMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com CFontMgr ");

	if (0 != GetSingle(CImguiMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com CImguiMgr ");

}

void CGameInstance::Free()
{
	Safe_Release(m_pSoundMgr);
	Safe_Release(m_pThreadMgr);
	Safe_Release(m_pObjectMgr);
	Safe_Release(m_pSceneMgr);
	Safe_Release(m_pComponenetMgr);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pInputDevice);
	Safe_Release(m_pImguiMgr);
	Safe_Release(m_pTimerMgr);
	Safe_Release(m_pEasingMgr);
	Safe_Release(m_pPickingMgr);
	Safe_Release(m_pFrustumMgr);
	Safe_Release(m_pFontMgr);
	Safe_Release(m_pXMLMgr);
	
}
