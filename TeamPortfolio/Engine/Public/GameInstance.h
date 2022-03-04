#pragma once

#include "Base.h"
#include "Graphic_Device.h"
#include "EasingMgr.h"
#include "ComponentMgr.h"
#include "Input_Device.h"
#include "ImguiMgr.h"

BEGIN(Engine)

class CTimeMgr;
class CSceneMgr;
class CScene;
class CThreadMgr; 
class CObjectMgr;
class CGameObject;
class CLayer;
class CImguiMgr;
class CPicking;
class CFrustumMgr;
class CSoundMgr;
class CFontMgr;
class CXMLMgr;


class ENGINE_DLL CGameInstance final :public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:
	explicit CGameInstance();
	virtual ~CGameInstance() = default;

	 

public:/*엔진이 돌아갈 때 전반적으로 처리해줘야할 것들*/
	HRESULT		Initialize_Engine(HINSTANCE hInst, const CGraphic_Device::GRAPHICDESC& GraphicDesc,_uint iMaxSceneNum , LPDIRECT3DDEVICE9* ppOut, _float fDoubleInterver = 0.3f);
	_int		Update_Engine(_float fDeltaTime);
	_int		LateUpdate_Engine(_float fDeltaTime);

	HRESULT		Initialize_Engine_Tool(const CGraphic_Device::GRAPHICDESC& GraphicDesc, _uint iMaxSceneNum, LPDIRECT3DDEVICE9* ppOut);
	_int		Update_Engine_Tool(_float fDeltaTime);

	HRESULT		Clear_Scene_Resource(_uint eSceneNum);


public:/* For.ObjectMgr */
	HRESULT			Add_GameObject_Prototype(const _tchar* tagPrototype, CGameObject* pPrototype);
	HRESULT			Add_GameObject_To_Layer(_uint eSceneNum, const _tchar* tagLayer, const _tchar* tagPrototype, void* pArg = nullptr);
	CComponent*		Get_Commponent_By_LayerIndex(_uint eSceneNum, const _tchar* tagLayer, const _tchar* tagComponet, _uint iLayerIndex = 0);
	CGameObject*	Get_GameObject_By_LayerIndex(_uint eSceneNum, const _tchar* tagLayer, _uint iLayerIndex = 0);
	list<CGameObject*>*	Get_ObjectList_from_Layer(_uint eSceneNum, const _tchar* tagLayer);
	CGameObject*	Change_Camera_Ortho_By_LayerIndex(_uint eSceneNum, const _tchar* tagLayer, _uint iLayerIndex = 0);
	HRESULT			Delete_GameObject_To_Layer_Index(_uint eSceneNum, const _tchar* tagLayer, _uint index);
	HRESULT			Delete_GameObject_To_Layer_Object(_uint eSceneNum, const _tchar* tagLayer, CGameObject* obj);



public: /*For.ComMgr*/
	HRESULT			Add_Component_Prototype(_uint eSceneIdx, const _tchar* tagPrototypeComponent, CComponent* pComponenet);
	CComponent*		Clone_Component(_uint eSceneIdx, const _tchar* tagPrototypeComponent, void* pArg = nullptr);


public: /* For.TimerMgr */
	_float	Get_DeltaTime(const _tchar* tagTimer);
	HRESULT Add_Timer(const _tchar* tagTimer);

public:  /*For Graphic Device*/
	void Render_Begin(D3DXCOLOR backbuffer = COLOR_GRAY);
	void Render_End();

public: /*For SceneMgr*/
	HRESULT Scene_Change(CScene* pScene, _int iNextSceneIdx, _bool bIsNotLoadingScene = false);
	_int	Render_Scene();
	_uint	Get_NowSceneNum();
	void	Set_iNowSceneIndx_ExceptLoad(_int iSceneNum);
	//은혁이가 건듬
	CScene*	Get_Scene();

public: /*For TreadMgr*/
	HRESULT		PlayThread(void* _ThreadFunc, void* _pArg);

public: /* For.Input_Device */
	_byte Get_DIKeyState(_ubyte eKeyID);
	_long Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState);
	_byte Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState);

public:
	_float Easing(_uint eEasingType,_float fStartPoint, _float fTargetPoint,  _float fPassedTime, _float fTotalTime = 1.0f);
	
	//FrustumMgr
public:
	HRESULT SetUp_WorldFrustumPlane();													//이 함수는 매 프레임 당 한번만 실행 어차피 매 프레임 같은 투영, 뷰 행렬일테니까 
	_bool IsNeedToRender(_float3 vWorldPosition, _float fLenth = 1.4142135623f);		//이 함수를 이제 객체별로 전달해서 그려야할지 여부를 판단


	//SoundMgr
public:
	_int  Channel_VolumeUp(CHANNELID eID, _float _vol);
	_int  Channel_VolumeDown(CHANNELID eID, _float _vol);
	_int  Channel_Pause(CHANNELID eID);

	HRESULT PlaySound(TCHAR* pSoundKey, CHANNELID eID, _float fLouderMultiple = 1.f);
	HRESULT PlayBGM(TCHAR* pSoundKey, _float fLouderMultiple = 1.f);
	void Stop_ChannelSound(CHANNELID eID);
	void Stop_AllChannel();

	_float  Get_Channel_Volume(CHANNELID eID);
	_bool  Get_Channel_IsPaused(CHANNELID eID);

public:
	HRESULT Render_UI_Font(wstring szString, _float2 vOnWindowPos, _float2 vFontSize = _float2(20.f, 30.f), _float3 Color_RGB = { 0,0,0 }, _int UntilDrawIndex = -1);
	HRESULT Render_World_Font(wstring szString, _float3 vOnWorldPos, _float2 vFontSize = _float2(0.3f, 0.45f), _float3 Color_RGB = { 0,0,0 }, _int UntilDrawIndex = -1);

public://XMLParser
	CVIBuffer_Parsed* Create_ParsedObject(wstring pPosTextFilePath, wstring pIndexTextFilePath);


public: /* For. IMGUI Interface */
	CImguiMgr* GetIMGui();

public:
	HRESULT Update_Transform_ToWorldSpace(POINT p);
	_bool isPick(_float3 * pLocalPoint, _float3 * pOut, _float2* pUV);

private:
	CGraphic_Device*	m_pGraphicDevice = nullptr;
	CTimeMgr*			m_pTimerMgr = nullptr;
	CSceneMgr*			m_pSceneMgr = nullptr;
	CThreadMgr*			m_pThreadMgr = nullptr;
	CObjectMgr*			m_pObjectMgr = nullptr;
	CComponentMgr*		m_pComponenetMgr = nullptr;
	CInput_Device*		m_pInputDevice = nullptr;
	CEasingMgr*			m_pEasingMgr = nullptr;
	CImguiMgr*			m_pImguiMgr = nullptr;
	CPicking*			m_pPickingMgr = nullptr;
	CFrustumMgr*		m_pFrustumMgr = nullptr;
	CSoundMgr*			m_pSoundMgr = nullptr;
	CFontMgr*			m_pFontMgr = nullptr;
	CXMLMgr*			m_pXMLMgr = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END