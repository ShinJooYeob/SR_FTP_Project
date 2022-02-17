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

class CObjectTool_Rect;
class CSuperToolSIngleton : public CBase
{
	DECLARE_SINGLETON(CSuperToolSIngleton)

private:
	explicit CSuperToolSIngleton();
	virtual ~CSuperToolSIngleton() = default;

public: // For. Device
	HRESULT		InitDevice(void);
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

	CObjectTool_Rect* GetObjectRect() { return m_Object_Rect; }
public:
	HRESULT SaveData_Object(CObjectTool_Rect* obj, CWnd* cwnd);
	HRESULT LoadData_Object(CWnd * cwnd);


private:

	// �� â������ ����
	CMainFrame*			m_pMainFrame;	// ���� ������
	CMyForm*			m_pMyButtomView;// ��ư ��
	CMiniView*			m_pMiniView;	// �̴� ��
	CToolView*			m_pToolView;	// �� ��
	CPathFind*			m_pPathDialog;	// ��� â
	CTrans_Dialog*		m_pTransDialog;	// ��ġ ���� â

	CObjectTool_Rect*	m_Object_Rect;


private:
	LPDIRECT3DDEVICE9			m_pGraphicDevice;
	CGameInstance*				m_pGameInstance;
	CRenderer*					m_pComRenderer;

	// ��Ÿ COM ��ü
	// LPD3DXSPRITE			m_pSprite;
	// LPD3DXFONT				m_pFont;

public:
//	void Release_Engine();
	virtual void Free()override;

};

END