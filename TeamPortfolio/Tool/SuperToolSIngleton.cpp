#include "stdafx.h"
#include "SuperToolSIngleton.h"
#include "GameInstance.h"
#include "ObjectTool_Rect.h"
#include "Camera_Tool.h"


IMPLEMENT_SINGLETON(CSuperToolSIngleton)


CSuperToolSIngleton::CSuperToolSIngleton()
	: m_pGraphicDevice(nullptr), m_pGameInstance(GetSingle(CGameInstance))
{
	m_Object_Rect = nullptr;
	m_pComRenderer = nullptr;
	Safe_AddRef(m_pGameInstance);
}

HRESULT CSuperToolSIngleton::InitDevice(void)
{
	// �������� DX9 �ʱ�ȭ
	CGraphic_Device::GRAPHICDESC desc;
	desc.hWnd = g_hWnd;
	desc.eScreenMode = CGraphic_Device::TYPE_SCREEN;
	desc.iWinCX = TOOL_WINCX;
	desc.iWinCY = TOOL_WINCY;

	// �� �ʱ�ȭ
	m_pGameInstance->Initialize_Engine_Tool(desc, SCENEID::SCENE_END, &m_pGraphicDevice);

	// IMGUI ���̺귯�������� IMGUI �ʱ�ȭ ������Ѵ�. / ������ �ڵ����� �Ŵ������� ����
	m_pGameInstance->GetIMGui()->Initialize_IMGUI(desc.hWnd, m_pGraphicDevice);


	// ������ ����

	//���̾� ������
//	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//��� �������� ������
	m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, false);

	//// �ؽ�ó
	//m_pGraphicDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	////�ؽ����� ����� Ȯ��ǰų� ��ҵɶ� ���� �� ����� �߻��� ������ ���� �����ؼ� ä��
	//m_pGraphicDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	////�ؽ��ĸ� �ΰ��� ������ �����ϰ� �Ǹ� ���ϰ� �����ؼ� ������ ���� ���� ��찡 �߻�
	//m_pGraphicDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//m_pGraphicDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	////UV��ǥ�� 1�� �Ѿ�� �ۼ��� ��� �ٽ� 1�� ���� 0���� ġȯ�Ͽ� ��ǥ�� ����


	// ������Ʈ ����
	Ready_Initalize_Object();

	// �� â ����
	Initialize_ToolView();

	return S_OK;
}

HRESULT CSuperToolSIngleton::Update_Tool(_float ftimer)
{
	if (m_pGameInstance == nullptr)
		return E_FAIL;

	int i = m_pGameInstance->Update_Engine_Tool(ftimer);

	return S_OK;
}

HRESULT CSuperToolSIngleton::Render_Begin(void)
{
	if (m_pGameInstance == nullptr)
		return E_FAIL;
	m_pGameInstance->Render_Begin();
	return S_OK;
}

HRESULT CSuperToolSIngleton::Render_End(HWND hWnd)
{
	if (m_pGameInstance == nullptr)
		return E_FAIL;
	m_pGraphicDevice->EndScene();
	m_pGraphicDevice->Present(nullptr, nullptr, hWnd, nullptr);
}

HRESULT CSuperToolSIngleton::Ready_Initalize_Object()
{

	FAILED_CHECK(Ready_Object_Component());
	FAILED_CHECK(Ready_Object_Clone(TEXT("Object")));
	FAILED_CHECK(Ready_Object_Camera(TEXT("Camera")));
	return S_OK;
}

HRESULT CSuperToolSIngleton::Ready_Object_Component()
{
	// #Tag �� ��� ������Ʈ�� ���� ����

	// Prototype_GameObject_BackGround
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_Prototype(TEXT("Prototype_GameObject_BackGround"), CObjectTool_Rect::Create(m_pGraphicDevice)));

	// ī�޶� ����
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Camera"), CCamera_Tool::Create(m_pGraphicDevice)));

	// #Tag �� ��� ������Ʈ�� ���� ����


	//������ �İ�ü ������Ÿ�� ����
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), m_pComRenderer = CRenderer::Create(m_pGraphicDevice)));

	Safe_AddRef(m_pComRenderer);

	//�����ε��� ������Ÿ�� ����
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphicDevice)));

	//Transform ������Ÿ�� ����
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphicDevice)));

	/* ����Ʈ �ؽ�ó ������Ÿ�� ���� */
	CTexture::TEXTUREDESC TextureDesc{};
	TextureDesc.szFilePath = TEXT("../Resource/Default.png");

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Texture_Default"), CTexture::Create(m_pGraphicDevice, &TextureDesc)));

	return S_OK;
}

HRESULT CSuperToolSIngleton::Ready_Object_Clone(const _tchar* layertag)
{
	// #Tag ������ ����� Ŭ�� ��ü ����

	// BackObj
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, layertag, TEXT("Prototype_GameObject_BackGround")))
		return E_FAIL;

	m_Object_Rect = (CObjectTool_Rect*)GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENEID::SCENE_STATIC, layertag);
	Safe_AddRef(m_Object_Rect);
	return S_OK;
}


HRESULT CSuperToolSIngleton::Ready_Object_Camera(const _tchar* layertag)
{
	CCamera::CAMERADESC cc;

	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vWorldRotAxis = _float3(0, 0, 0);
	CameraDesc.vEye = _float3(0, 0, -5.f);
	CameraDesc.vAt = _float3(0, 0, 0);
	CameraDesc.vAxisY = _float3(0, 1, 0);

	CameraDesc.fFovy = D3DXToRadian(90);
	CameraDesc.fAspect = _float(TOOL_WINCX) / TOOL_WINCY;
	CameraDesc.fNear = 1.0f;
	CameraDesc.fFar = 100.0f;

	CameraDesc.TransformDesc.fMovePerSec = 10.f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);


	//if (FAILED(m_pGameInstance->Add_GameObject_Prototype(TAG_OP(Prototype_Camera_Main), CCamera_Main::Create(m_pGraphicDevice, &CameraDesc))))

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, layertag, TEXT("Prototype_GameObject_Camera"), &CameraDesc))
		return E_FAIL;

}

HRESULT CSuperToolSIngleton::Initialize_ToolView()
{
	m_pMainFrame = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pToolView = static_cast<CToolView*>(m_pMainFrame->m_MainSplitter.GetPane(0, 1));
	m_pMiniView = static_cast<CMiniView*>(m_pMainFrame->m_MainSplitter.GetPane(0, 0));
	m_pMyButtomView = static_cast<CMyForm*>(m_pMainFrame->m_SecondSplitter.GetPane(1, 0));

	m_pTransDialog = &m_pMyButtomView->m_TransformDialog;
	m_pPathDialog = &m_pMyButtomView->m_PathFind;

	return S_OK;
}


HRESULT CSuperToolSIngleton::SaveData_Object(CObjectTool_Rect* obj, CWnd* cwnd)
{
	// ���� Ŭ���� �ϴ� ��

	// 1. Ŭ���� ���̾� �α� ����.
	CFileDialog		Dlg(FALSE,
		L"dat", // .dat���Ϸ� ����
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||",
		cwnd);

	TCHAR	szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);

	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		// �̸�
		CString				str = Dlg.GetPathName().GetString();
		CString				Filename = PathFindFileName(str);

		const TCHAR*		pGetPath = str.GetString();



		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		// �ش� ������ ������Ʈ ������ �ش��ϴ� ���� �ִ´�.
		MYFILEPATH path = obj->Get_PathData();
		OUTPUT_OBJECTINFO infoObj;
		infoObj.fScale = obj->Get_Scale();
		infoObj.fPos = obj->Get_Pos();
		lstrcpy(infoObj.strObjectName, Filename);
		lstrcpy(infoObj.strTextureName, path.wFileName.c_str());
		lstrcpy(infoObj.strTexturePath, path.wstrFullPath.c_str());


		//infoObj.strFileName = path.wFileName.c_str();
		//infoObj.strFullPath = path.wstrFullPath.c_str();

		DWORD	dwByte = 0;

		// ����
		WriteFile(hFile, &infoObj, sizeof(OUTPUT_OBJECTINFO), &dwByte, nullptr);

		CloseHandle(hFile);

	}
	return S_OK;
}

HRESULT CSuperToolSIngleton::LoadData_Object(CWnd * cwnd)
{

	//	static TCHAR BASED_CODE szFilter[] = _T("�̹��� ����(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |�������(*.*)|*.*||");


	static TCHAR BASED_CODE szFilter[] = _T("������ ����(*.dat) | *.dat; | �������(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.dat"), _T(""), OFN_HIDEREADONLY, szFilter);

	OUTPUT_OBJECTINFO infoobj = {};

	if (IDOK == dlg.DoModal())
	{

		CString pathName = dlg.GetPathName();

		// �ؼ� �ڵ�


		// �̸�
		CString				str = dlg.GetPathName().GetString();
		const TCHAR*		pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		// �ش� ������ ������Ʈ ������ �ش��ϴ� ���� �ִ´�.		
		DWORD	dwByte = 0;

		// �ε�
		ReadFile(hFile, &infoobj, sizeof(OUTPUT_OBJECTINFO), &dwByte, nullptr);
		m_Object_Rect->Set_Data(infoobj);
		CloseHandle(hFile);
	}

	return S_OK;
}



void CSuperToolSIngleton::Free()
{
	Safe_Release(m_Object_Rect);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pComRenderer);
	Safe_Release(m_pGameInstance);
	m_pGameInstance->Release_Engine();
}
