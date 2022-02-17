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
	// 엔진으로 DX9 초기화
	CGraphic_Device::GRAPHICDESC desc;
	desc.hWnd = g_hWnd;
	desc.eScreenMode = CGraphic_Device::TYPE_SCREEN;
	desc.iWinCX = TOOL_WINCX;
	desc.iWinCY = TOOL_WINCY;

	// 툴 초기화
	m_pGameInstance->Initialize_Engine_Tool(desc, SCENEID::SCENE_END, &m_pGraphicDevice);

	// IMGUI 라이브러리때문에 IMGUI 초기화 해줘야한다. / 삭제는 자동으로 매니저에서 삭제
	m_pGameInstance->GetIMGui()->Initialize_IMGUI(desc.hWnd, m_pGraphicDevice);


	// 랜더링 설정

	//와이어 프레임
//	m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//잠시 조명연산을 꺼놓자
	m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, false);

	//// 텍스처
	//m_pGraphicDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	////텍스쳐의 사이즈가 확대되거나 축소될때 변경 된 사이즈에 발생한 공백을 선형 보간해서 채움
	//m_pGraphicDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	////텍스쳐를 인게임 내에서 수정하게 되면 과하게 선명해서 오히려 눈이 아픈 경우가 발생
	//m_pGraphicDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//m_pGraphicDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	////UV좌표를 1을 넘어가게 작성할 경우 다시 1인 점을 0으로 치환하여 좌표를 찍음


	// 오브젝트 정보
	Ready_Initalize_Object();

	// 툴 창 정보
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
	// #Tag 툴 모든 오브젝트의 원형 생성

	// Prototype_GameObject_BackGround
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_Prototype(TEXT("Prototype_GameObject_BackGround"), CObjectTool_Rect::Create(m_pGraphicDevice)));

	// 카메라 생성
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Camera"), CCamera_Tool::Create(m_pGraphicDevice)));

	// #Tag 툴 모든 컴포넌트의 원형 생성


	//렌더러 컴객체 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Renderer"), m_pComRenderer = CRenderer::Create(m_pGraphicDevice)));

	Safe_AddRef(m_pComRenderer);

	//버퍼인덱스 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pGraphicDevice)));

	//Transform 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_pGraphicDevice)));

	/* 디폴트 텍스처 프로토타입 생성 */
	CTexture::TEXTUREDESC TextureDesc{};
	TextureDesc.szFilePath = TEXT("../Resource/Default.png");

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TEXT("Prototype_Component_Texture_Default"), CTexture::Create(m_pGraphicDevice, &TextureDesc)));

	return S_OK;
}

HRESULT CSuperToolSIngleton::Ready_Object_Clone(const _tchar* layertag)
{
	// #Tag 씬에서 사용할 클론 객체 생성

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
	// 저장 클릭시 하는 것

	// 1. 클릭시 다이얼 로그 생성.
	CFileDialog		Dlg(FALSE,
		L"dat", // .dat파일로 저장
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
		// 이름
		CString				str = Dlg.GetPathName().GetString();
		CString				Filename = PathFindFileName(str);

		const TCHAR*		pGetPath = str.GetString();



		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		// 해당 공간에 오브젝트 정보에 해당하는 값을 넣는다.
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

		// 저장
		WriteFile(hFile, &infoObj, sizeof(OUTPUT_OBJECTINFO), &dwByte, nullptr);

		CloseHandle(hFile);

	}
	return S_OK;
}

HRESULT CSuperToolSIngleton::LoadData_Object(CWnd * cwnd)
{

	//	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");


	static TCHAR BASED_CODE szFilter[] = _T("데이터 파일(*.dat) | *.dat; | 모든파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.dat"), _T(""), OFN_HIDEREADONLY, szFilter);

	OUTPUT_OBJECTINFO infoobj = {};

	if (IDOK == dlg.DoModal())
	{

		CString pathName = dlg.GetPathName();

		// 해석 코드


		// 이름
		CString				str = dlg.GetPathName().GetString();
		const TCHAR*		pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		// 해당 공간에 오브젝트 정보에 해당하는 값을 넣는다.		
		DWORD	dwByte = 0;

		// 로드
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
