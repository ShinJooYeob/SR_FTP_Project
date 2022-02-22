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
	HRESULT Ready_Object_Clone(const _tchar* layertag);
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

	CPathFind*  GetPathTool() {	return m_pPathDialog; }
	CTrans_Dialog* GetTransTool() { return m_pTransDialog; }

public:
	HRESULT SaveData_Object(CObjectTool_ToolObject* obj, CWnd* cwnd);
	HRESULT LoadData_Object(CWnd * cwnd);
	HRESULT Create_ToolObject_Button(wstring name);
	HRESULT Select_ToolObject_Button(int index);

private:
	CObjectTool_ToolObject* Create_New_ToolObject(wstring name);
	HRESULT Change_ToolObject(CObjectTool_ToolObject* obj);

public: // For.ToolView tp ToolObject 
	HRESULT Add_Vec_ToolObject(CObjectTool_ToolObject* obj);
	CObjectTool_ToolObject* Find_Vec_ToolObject(_uint index);
	CObjectTool_ToolObject* Get_CurrentToolObject() { return m_Object_Rect; }

	const vector<CObjectTool_ToolObject*>&  Get_ToolVec() const { return m_Vec_ToolViewObjects; }

	_uint Get_ToolVec_Size() { return m_Vec_ToolViewObjects.size(); }
	_bool Get_ToolVec_isEmpty() { return m_Vec_ToolViewObjects.empty(); }

	// ���õ� �͸� ������
	HRESULT Update_Select_Render_None();
	HRESULT Update_Select_Render_Visble(CObjectTool_ToolObject* visbleobj);

private:
	// ToolView���� ������Ʈ ����Ʈ���� ������ ����
	vector<CObjectTool_ToolObject*> m_Vec_ToolViewObjects;

	// Current Tool View Object
	// �̰� ���� �������� ����ϴ� ���� �ƴ϶� ����� �� ���� ������ �޾� ����ϰ� ����
	CObjectTool_ToolObject*	m_Object_Rect;

	// CObjectTool_ToolObject*	m_Object_ProtoObject;


private:

	// �� â������ ����
	CMainFrame*			m_pMainFrame;	// ���� ������
	CMyForm*			m_pMyButtomView;// ��ư ��
	CMiniView*			m_pMiniView;	// �̴� ��
	CToolView*			m_pToolView;	// �� ��
	CPathFind*			m_pPathDialog;	// ��� â
	CTrans_Dialog*		m_pTransDialog;	// ��ġ ���� â


private:
	LPDIRECT3DDEVICE9			m_pGraphicDevice;
	CGameInstance*				m_pGameInstance;
	CRenderer*					m_pComRenderer;

	// ��Ÿ COM ��ü
	// LPD3DXSPRITE				m_pSprite;
	// LPD3DXFONT				m_pFont;

public:
//	void Release_Engine();
	virtual void Free()override;

};

END