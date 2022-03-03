#pragma once

#include "Base.h"

// 툴의 모든 데이터를 한공간에서 관리하기 위한 싱글턴
// 기능이 좀 완성되고 추가 구현

BEGIN(Engine)
class CGameInstance;
class CRenderer;
END

#include "MainFrm.h"
#include "MyForm.h"
#include "MiniView.h"
#include "PathFind.h"
#include "Trans_Dialog.h"
#include "ToolView.h"
#include "MapTool.h"

BEGIN(Tool)

class CObjectTool_ToolObject;
class CObjectTool_ToolWire;
class CSuperToolSIngleton : public CBase
{
	DECLARE_SINGLETON(CSuperToolSIngleton)
public:
	// 툴 모드들
	enum E_TOOL_MODE
	{
		TOOLMODE_OBJECT,	// 오브젝트 모드
		TOOLMODE_MAP,		// 맵모드
		TOOLMODE_END
	};

	const int iObjectSize = 20;

private:
	explicit CSuperToolSIngleton();
	virtual ~CSuperToolSIngleton() = default;

public: // For. Device
	HRESULT		InitDevice();
	HRESULT		Update_Tool(_float ftimer);
	HRESULT		Render_Begin(D3DCOLOR backbuffer = COLOR_GRAY);
	HRESULT		Render_End(HWND hWnd = NULL);

	// 오브젝트 초기화
public:
	HRESULT Ready_Initalize_Object();
private:
	HRESULT		Init_Light();
	HRESULT		Update_Light();

	HRESULT Ready_Object_Component();
	HRESULT Ready_Object_Clone_View(const _tchar* layertag);
	HRESULT Ready_Object_Clone_Map(const _tchar* layertag);
	HRESULT Ready_Object_Camera(const _tchar* layertag);

	HRESULT		Render_Set_State();

	// 창 초기화
private:
	HRESULT Initialize_ToolView();
	HRESULT Initialize_ToolView_Data();

private:
	// 빛 모델 초기화 
	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);


	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	D3DMATERIAL9 m_WhiteMtrl;

public: // Get Set
	CGameInstance* GetGameInstance()
	{
		return m_pGameInstance;
	}
	CRenderer* Get_Component_Renderer()
	{
		return m_pComRenderer;
	}
	LPDIRECT3DDEVICE9 Get_Graphics_Device()
	{
		return m_pGraphicDevice;
	}

	CMainFrame* GetMainFrm() { return m_pMainFrame; }
	CMyForm* GetMyButtonView() { return m_pMyButtomView; }
	CToolView*  GetToolView() { return m_pToolView; }
	CMiniView* GetMiniView() { return m_pMiniView; }

	CPathFind*  GetPathTool() { return m_pPathDialog; }
	CTrans_Dialog* GetTransTool() { return m_pTransDialog; }
	CMapTool* GetMapTool() { return m_pMapToolDialog; }

public: //For. Data
	HRESULT SaveData_Object(CObjectTool_ToolObject* obj, CWnd* cwnd);
	HRESULT SaveData_Map(list<CGameObject*> objlist, CWnd* cwnd);

	HRESULT LoadData_Data(CWnd * cwnd);

	HRESULT LoadData_ObjectFile();

	HRESULT Create_ToolObject_Button(wstring name);
	HRESULT Create_ToolObject_Data(const _tchar* str, const OUTPUT_OBJECTINFO& data);

public:// For Create
	CObjectTool_ToolObject* Create_New_ToolObject(wstring name, const _tchar* laytag);
	CObjectTool_ToolObject* Create_Load_MapObject(const OUTPUT_OBJECTINFO& info, const _tchar* laytag);
	CObjectTool_ToolObject* Create_Clone_MapObject(_float3 Pos, const _tchar* laytag , _bool bRandom = false, _uint texIndex=0);

public:// For Object
	_float3					Get_Center_MapPosition();
	CObjectTool_ToolObject* Get_ViewObject_Object(int index = 0);
	CObjectTool_ToolObject* Get_ViewObject_SelectObject();
	CObjectTool_ToolWire*	Get_WireCube();
	void					Set_ViewObject_Index(int index);

	list<CGameObject*>*	 Get_GameObjectList(const _tchar* laytag);

	// 선택된 것만 랜더링
	HRESULT Update_Select_Render_None(const _tchar* laytag);
	HRESULT Update_Select_Render_Visble(const _tchar* laytag, CObjectTool_ToolObject* visbleobj);


private:

	// 각 창에대한 정보
	CMainFrame*			m_pMainFrame;	// 메인 프레임
	CMyForm*			m_pMyButtomView;// 버튼 뷰
	CMiniView*			m_pMiniView;	// 미니 뷰
	CToolView*			m_pToolView;	// 툴 뷰
	CPathFind*			m_pPathDialog;	// 경로 창
	CTrans_Dialog*		m_pTransDialog;	// 위치 조정 창
	CMapTool*			m_pMapToolDialog;// 맵툴

private:
	LPDIRECT3DDEVICE9			m_pGraphicDevice;
	CGameInstance*				m_pGameInstance;
	CRenderer*					m_pComRenderer;	

private:
	_int						m_ViewObjectSelectIndex;

	// 기타 COM 객체
	// LPD3DXSPRITE				m_pSprite;
	// LPD3DXFONT				m_pFont;

public:
	//	void Release_Engine();
	static E_TOOL_MODE g_MAP_MODE;

	virtual void Free()override;
};

END