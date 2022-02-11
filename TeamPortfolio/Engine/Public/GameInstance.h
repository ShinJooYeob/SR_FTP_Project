#pragma once

#include "Base.h"
#include "Graphic_Device.h"
#include "ComponentMgr.h"
#include "Input_Device.h"

BEGIN(Engine)

class CTimeMgr;
class CSceneMgr;
class CScene;
class CThreadMgr; 
class CObjectMgr;
class CGameObject;
class CLayer;

class ENGINE_DLL CGameInstance final :public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:
	explicit CGameInstance();
	virtual ~CGameInstance() = default;

	 

public:/*엔진이 돌아갈 때 전반적으로 처리해줘야할 것들*/
	HRESULT		Initialize_Engine(HINSTANCE hInst, const CGraphic_Device::GRAPHICDESC& GraphicDesc,_uint iMaxSceneNum , LPDIRECT3DDEVICE9* ppOut, _float fDoubleInterver = 0.3f);
	_int		Update_Engine(_float fDeltaTime);
	HRESULT		Clear_Scene_Resource(_uint eSceneNum);


public:/* For.ObjectMgr */
	HRESULT			Add_GameObject_Prototype(const _tchar* tagPrototype, CGameObject* pPrototype);
	HRESULT			Add_GameObject_To_Layer(_uint eSceneNum, const _tchar* tagLayer, const _tchar* tagPrototype, void* pArg = nullptr);
	CComponent*		Get_Commponent_By_LayerIndex(_uint eSceneNum, const _tchar* tagLayer, const _tchar* tagComponet, _uint iLayerIndex = 0);
	CGameObject*	Get_GameObject_By_LayerIndex(_uint eSceneNum, const _tchar* tagLayer, _uint iLayerIndex = 0);
	CLayer*			Get_Layer(_uint eSceneNum, const _tchar* tagLayer);


public: /*For.ComMgr*/
	HRESULT			Add_Component_Prototype(_uint eSceneIdx, const _tchar* tagPrototypeComponent, CComponent* pComponenet);
	CComponent*		Clone_Component(_uint eSceneIdx, const _tchar* tagPrototypeComponent, void* pArg = nullptr);


public: /* For.TimerMgr */
	_float	Get_DeltaTime(const _tchar* tagTimer);
	HRESULT Add_Timer(const _tchar* tagTimer);

public:  /*For Graphic Device*/
	void Render_Begin(void);
	void Render_End();

public: /*For SceneMgr*/
	HRESULT Scene_Change(CScene* pScene, _int iNextSceneIdx);
	_int	Render_Scene(); // 


public: /*For TreadMgr*/
	HRESULT		PlayThread(void* _ThreadFunc, void* _pArg);

public: /* For.Input_Device */
	_byte Get_DIKeyState(_ubyte eKeyID);
	_long Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState);
	_byte Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState);


private:
	CGraphic_Device*	m_pGraphicDevice = nullptr;
	CTimeMgr*			m_pTimerMgr = nullptr;
	CSceneMgr*			m_pSceneMgr = nullptr;
	CThreadMgr*			m_pThreadMgr = nullptr;
	CObjectMgr*			m_pObjectMgr = nullptr;
	CComponentMgr*		m_pComponenetMgr = nullptr;
	CInput_Device*		m_pInputDevice = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END