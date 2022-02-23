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
class CSuperToolSIngleton : public CBase
{
	DECLARE_SINGLETON(CSuperToolSIngleton)

	const int iObjectSize = 20;
private:
	explicit CSuperToolSIngleton();
	virtual ~CSuperToolSIngleton() = default;

public: // For. Device
	HRESULT		InitDevice();
	HRESULT		Update_Tool(_float ftimer);
	HRESULT		Render_Begin(void);
	HRESULT		Render_End(HWND hWnd = NULL);

	// 오브젝트 초기화
public:
	HRESULT Ready_Initalize_Object();
private:

	HRESULT Ready_Object_Component();
	HRESULT Ready_Object_Clone_View(const _tchar* layertag);
	HRESULT Ready_Object_Clone_Map(const _tchar* layertag);
	HRESULT Ready_Object_Camera(const _tchar* layertag);

	HRESULT		Render_Set_Statee();

	// 창 초기화
private:
	HRESULT Initialize_ToolView();

public:
	CGameInstance* GetGameInstance()
	{
		return m_pGameInstance;
	}
	CRenderer* Get_Component_Renderer()
	{
		return m_pComRenderer;
	}

	CMainFrame* GetMainFrm() { return m_pMainFrame; }
	CMyForm* GetMyButtonView() { return m_pMyButtomView; }
	CToolView*  GetToolView() { return m_pToolView; }
	CMiniView* GetMiniView() { return m_pMiniView; }

	CPathFind*  GetPathTool() { return m_pPathDialog; }
	CTrans_Dialog* GetTransTool() { return m_pTransDialog; }
	CMapTool* GetMapTool() { return m_pMapToolDialog; }

public:
	HRESULT SaveData_Object(CObjectTool_ToolObject* obj, CWnd* cwnd);
	HRESULT LoadData_Object(CWnd * cwnd);
	HRESULT Create_ToolObject_Button(wstring name);
	HRESULT Create_ToolObject_Data(const OUTPUT_OBJECTINFO& data);

	HRESULT Select_ToolObject_Button(int index);

private:
	CObjectTool_ToolObject* Create_New_ToolObject(wstring name, const _tchar* laytag);
	CObjectTool_ToolObject* Create_Clone_MapObject(const OUTPUT_OBJECTINFO& protoInfo, _float3 Pos, wstring laytag);

	HRESULT Change_ToolObject(CObjectTool_ToolObject* obj);

public:

	CObjectTool_ToolObject* Get_ViewObject_Object(int index = 0);
	CObjectTool_ToolObject* Get_ViewObject_SelectObject();
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
	CMapTool*			m_pMapToolDialog;

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
	virtual void Free()override;
};

END