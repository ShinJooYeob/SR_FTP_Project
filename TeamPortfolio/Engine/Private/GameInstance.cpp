#include "..\Public\GameInstance.h"
#include "Graphic_Device.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "ThreadMgr.h"
#include "ObjectMgr.h"
#include "GameObject.h"
#include "ComponentMgr.h"
#include "EasingMgr.h"

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
	m_pEasingMgr(GetSingle(CEasingMgr))
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
}


HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst,const CGraphic_Device::GRAPHICDESC & GraphicDesc, _uint iMaxSceneNum, LPDIRECT3DDEVICE9 * ppOut, _float fDoubleInterver)
{
	if (m_pGraphicDevice == nullptr || m_pObjectMgr == nullptr || m_pComponenetMgr == nullptr || m_pSceneMgr == nullptr)
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


	_float3 Temp;
	_Matrix Temp2;

	  
	_float3 Temp3 = Temp.Get_Nomalize();

	Temp.Get_Dot(Temp3);

	Temp.Get_Cross(Temp).Get_Nomalize();

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
	return S_OK;
}

_int CGameInstance::Update_Engine(_float fDeltaTime)
{
	if (m_pSceneMgr == nullptr || m_pObjectMgr == nullptr)
		return -1;


	if (FAILED(m_pInputDevice->SetUp_InputDeviceState(fDeltaTime)))
		return -1;

	if (m_pSceneMgr->Update(fDeltaTime) < 0)
		return -1;

	if (m_pObjectMgr->Update(fDeltaTime) < 0)
		return -1;
	



	if (m_pObjectMgr->LateUpdate(fDeltaTime) < 0)
		return -1;
	if (m_pSceneMgr->LateUpdate(fDeltaTime) < 0)
		return -1;

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

CGameObject * CGameInstance::Change_Camera_Ortho_By_LayerIndex(_uint eSceneNum, const _tchar * tagLayer, _uint iLayerIndex)
{
	if (tagLayer == nullptr || m_pObjectMgr == nullptr)
		return nullptr;

	return m_pObjectMgr->Change_Camera_Ortho_By_LayerIndex(eSceneNum, tagLayer, iLayerIndex);
}


CLayer * CGameInstance::Get_Layer(_uint eSceneNum, const _tchar * tagLayer)
{
	if (tagLayer == nullptr || m_pObjectMgr == nullptr)
		return nullptr;

	return m_pObjectMgr->Get_Layer(eSceneNum, tagLayer);
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

void CGameInstance::Render_Begin(void)
{
	if (m_pGraphicDevice == nullptr)
		return;

	m_pGraphicDevice->Render_Begin();
}

void CGameInstance::Render_End()
{
	if (m_pGraphicDevice == nullptr)
		return;

	m_pGraphicDevice->Render_End();
}

HRESULT CGameInstance::Scene_Change(CScene * pScene, _int iNextSceneIdx)
{

	if (m_pSceneMgr == nullptr)
		return E_FAIL;


	m_pSceneMgr->Scene_Chage(pScene, iNextSceneIdx);

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


_float CGameInstance::TargetLinear(_float fStartPoint, _float fTargetPoint,  _float fPassedTime, _float fTotalTime)
{
	if (m_pEasingMgr == nullptr)
		return 0;
	return m_pEasingMgr->TargetLinear(fStartPoint, fTargetPoint,  fPassedTime, fTotalTime);
}

CImguiMgr * CGameInstance::GetIMGui()
{
	return m_pImguiMgr->GetInstance();
}

_float CGameInstance::TargetQuadIn(_float fStartPoint, _float fTargetPoint,  _float fPassedTime, _float fTotalTime)
{
	if (m_pEasingMgr == nullptr)
		return 0;

	return m_pEasingMgr->TargetQuadIn(fStartPoint, fTargetPoint,  fPassedTime, fTotalTime);
}

void CGameInstance::Release_Engine()
{


	if (0 != GetSingle(CGameInstance)->DestroyInstance())
		MSGBOX("Failed to Release Com GameInstance");

	if (0 != GetSingle(CThreadMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com ThreadMgr ");
		
	if (0 != GetSingle(CEasingMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com EasingMgr ");

	if (0 != GetSingle(CObjectMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com ObjectMgr ");

	if (0 != GetSingle(CSceneMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com SceneMgr ");

	if (0 != GetSingle(CComponentMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com ComponentMgr");
	
	if (0 != GetSingle(CInput_Device)->DestroyInstance())
		MSGBOX("Failed to Release Com CInput_Device "); 

	if (0 != GetSingle(CGraphic_Device)->DestroyInstance())
		MSGBOX("Failed to Release Com Graphic_Device "); 
	
	if (0 != GetSingle(CTimeMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com TimeMgr ");

	if (0 != GetSingle(CImguiMgr)->DestroyInstance())
		MSGBOX("Failed to Release Com CImguiMgr ");
}

void CGameInstance::Free()
{
	Safe_Release(m_pThreadMgr);
	Safe_Release(m_pObjectMgr);
	Safe_Release(m_pSceneMgr);
	Safe_Release(m_pComponenetMgr);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pInputDevice);
	Safe_Release(m_pImguiMgr);
	Safe_Release(m_pTimerMgr);
	Safe_Release(m_pEasingMgr);
}
