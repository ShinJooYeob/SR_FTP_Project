#include "stdafx.h"
#include "SuperToolSIngleton.h"
#include "GameInstance.h"
#include "ObjectTool_ToolObject.h"
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
	Render_Set_Statee();

	m_pGameInstance->Render_Begin();
	return S_OK;
}

HRESULT CSuperToolSIngleton::Render_End(HWND hWnd)
{
	if (m_pGameInstance == nullptr)
		return E_FAIL;
	m_pGraphicDevice->EndScene();
	m_pGraphicDevice->Present(nullptr, nullptr, hWnd, nullptr);
	return S_OK;
}

HRESULT CSuperToolSIngleton::Render_Set_Statee()
{
	if (m_pGraphicDevice == nullptr)
		return E_FAIL;
	
	_bool bWire = (_bool)m_pMyButtomView->m_CheckWirframeEnable.GetCheck();
	if (bWire)
		m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DRS_FILLMODE);

	return S_OK;
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
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_Prototype(TAG_OP(Prototype_BackGround), CObjectTool_ToolObject::Create(m_pGraphicDevice)));

	// 카메라 생성
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_Prototype(TAG_OP(Prototype_Camera_Main), CCamera_Tool::Create(m_pGraphicDevice)));

	// #Tag 툴 모든 컴포넌트의 원형 생성

	//렌더러 컴객체 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), m_pComRenderer = CRenderer::Create(m_pGraphicDevice)));

	Safe_AddRef(m_pComRenderer);

	//버퍼인덱스 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect) , CVIBuffer_Rect::Create(m_pGraphicDevice)));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube), CVIBuffer_Cube::Create(m_pGraphicDevice)));


	//Transform 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Transform), CTransform::Create(m_pGraphicDevice)));

	// 블록 디폴트 
	CTexture::TEXTUREDESC TextureDesc{};
	TextureDesc.szTextFilePath = g_FileName_Blocktxt;
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Default), CTexture::Create(m_pGraphicDevice, &TextureDesc)));


	// 큐브 디폴트
	TextureDesc.eTextureType = CTexture::TYPE_CUBEMAP;
	TextureDesc.szTextFilePath = g_Filename_Cubetxt;
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Texture_Cube), CTexture::Create(m_pGraphicDevice, &TextureDesc)));


	return S_OK;
}

HRESULT CSuperToolSIngleton::Ready_Object_Clone(const _tchar* layertag)
{
	// #Tag 씬에서 사용할 클론 객체 생성

	// BackObj
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, layertag, TAG_OP(Prototype_BackGround)))
		return E_FAIL;

	// 최초 객체는 0번쨰에 저장한다.
	m_Object_Rect = (CObjectTool_ToolObject*)GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENEID::SCENE_STATIC, layertag);
	m_Object_Rect->Set_Defult(L"DefaultObject");
	Safe_AddRef(m_Object_Rect);
	Add_Vec_ToolObject(m_Object_Rect);
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

	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, layertag, TAG_OP(Prototype_Camera_Main), &CameraDesc))
		return E_FAIL;
	return S_OK;
}

HRESULT CSuperToolSIngleton::Initialize_ToolView()
{
	m_Vec_ToolViewObjects.reserve(iObjectSize);

	m_pMainFrame = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pToolView = static_cast<CToolView*>(m_pMainFrame->m_MainSplitter.GetPane(0, 1));
	m_pMiniView = static_cast<CMiniView*>(m_pMainFrame->m_MainSplitter.GetPane(0, 0));
	m_pMyButtomView = static_cast<CMyForm*>(m_pMainFrame->m_SecondSplitter.GetPane(1, 0));

	m_pTransDialog = &m_pMyButtomView->m_TransformDialog;
	m_pPathDialog = &m_pMyButtomView->m_PathFind;

	return S_OK;
}


HRESULT CSuperToolSIngleton::SaveData_Object(CObjectTool_ToolObject* obj, CWnd* cwnd)
{
	// 선택한 오브젝트를 저장한다.


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
		OUTPUT_OBJECTINFO infoObj = obj->Get_ObjectInfo();

		DWORD	dwByte = 0;

		// 저장
		WriteFile(hFile, &infoObj, sizeof(OUTPUT_OBJECTINFO), &dwByte, nullptr);

		CloseHandle(hFile);

	}
	return S_OK;
}

HRESULT CSuperToolSIngleton::LoadData_Object(CWnd * cwnd)
{
	// 새 오브젝트를 가져온다.

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

HRESULT CSuperToolSIngleton::Create_ToolObject_Button(wstring name)
{
	// 버튼을 누르면 새 오브젝트 생성.
	FAILED_CHECK(Change_ToolObject(Create_New_ToolObject(), name));
	// 리스타 박스 업데이트
	m_pMyButtomView->Update_ViewListBox();
	return S_OK;
}

CObjectTool_ToolObject* CSuperToolSIngleton::Create_New_ToolObject()
{
	// 현재 오브젝트를 저장하고 새 큐브 오브젝트를 만든다.
	// 새로 만든 객체는 여기 클래스의 Vector에 넣는다.
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, L"Object", TAG_OP(Prototype_BackGround)))
		return nullptr;

	int index = Get_ToolVec_Size();
	CObjectTool_ToolObject* newobj = (CObjectTool_ToolObject*)GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENEID::SCENE_STATIC,L"Object", index);
	Add_Vec_ToolObject(newobj);
	Safe_AddRef(newobj);
	return newobj;
}

HRESULT CSuperToolSIngleton::Change_ToolObject(CObjectTool_ToolObject * obj, wstring name)
{
	// 현재 오브젝트 릴리스
	Safe_Release(m_Object_Rect);
	// 새 오브젝트 대입
	m_Object_Rect = obj;
	// Safe_AddRef
	Safe_AddRef(m_Object_Rect);
	m_Object_Rect->Set_Defult(name);
	return S_OK;
}

HRESULT CSuperToolSIngleton::Add_Vec_ToolObject(CObjectTool_ToolObject * obj)
{
	m_Vec_ToolViewObjects.push_back(obj);
	return S_OK;
}

CObjectTool_ToolObject * CSuperToolSIngleton::Find_Vec_ToolObject(_uint index)
{
	if (index >= m_Vec_ToolViewObjects.size())
		return nullptr;

	return m_Vec_ToolViewObjects[index];

}



void CSuperToolSIngleton::Free()
{
	for (auto vec : m_Vec_ToolViewObjects)
		Safe_Release(vec);

	Safe_Release(m_Object_Rect);

	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pComRenderer);
	Safe_Release(m_pGameInstance);
	m_pGameInstance->Release_Engine();
}
