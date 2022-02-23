#pragma once

#include "Base.h"

// ���� ��� �����͸� �Ѱ������� �����ϱ� ���� �̱���
// ����� �� �ϼ��ǰ� �߰� ����

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

	// ������Ʈ �ʱ�ȭ
public:
	HRESULT Ready_Initalize_Object();
private:

	HRESULT Ready_Object_Component();
	HRESULT Ready_Object_Clone_View(const _tchar* layertag);
	HRESULT Ready_Object_Clone_Map(const _tchar* layertag);
	HRESULT Ready_Object_Camera(const _tchar* layertag);

	HRESULT		Render_Set_Statee();

	// â �ʱ�ȭ
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

	// ���õ� �͸� ������
	HRESULT Update_Select_Render_None(const _tchar* laytag);
	HRESULT Update_Select_Render_Visble(const _tchar* laytag, CObjectTool_ToolObject* visbleobj);


private:

	// �� â������ ����
	CMainFrame*			m_pMainFrame;	// ���� ������
	CMyForm*			m_pMyButtomView;// ��ư ��
	CMiniView*			m_pMiniView;	// �̴� ��
	CToolView*			m_pToolView;	// �� ��
	CPathFind*			m_pPathDialog;	// ��� â
	CTrans_Dialog*		m_pTransDialog;	// ��ġ ���� â
	CMapTool*			m_pMapToolDialog;

private:
	LPDIRECT3DDEVICE9			m_pGraphicDevice;
	CGameInstance*				m_pGameInstance;
	CRenderer*					m_pComRenderer;

private:
	_int						m_ViewObjectSelectIndex;
	// ��Ÿ COM ��ü
	// LPD3DXSPRITE				m_pSprite;
	// LPD3DXFONT				m_pFont;

public:
	//	void Release_Engine();
	virtual void Free()override;
};

END