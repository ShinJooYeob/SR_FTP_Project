#include "stdafx.h"
#include "SuperToolSIngleton.h"
#include "GameInstance.h"
#include "Layer.h"
#include "ObjectTool_ToolObject.h"
#include "ObjectTool_Terrain.h"
#include "Camera_Tool.h"

IMPLEMENT_SINGLETON(CSuperToolSIngleton)

CSuperToolSIngleton::E_TOOL_MODE CSuperToolSIngleton::g_MAP_MODE = CSuperToolSIngleton::E_TOOL_MODE::TOOLMODE_OBJECT;

CSuperToolSIngleton::CSuperToolSIngleton()
	: m_pGraphicDevice(nullptr), m_pGameInstance(GetSingle(CGameInstance))
{
	m_pComRenderer = nullptr;
	m_ViewObjectSelectIndex = -1;
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


	// 오브젝트 수정에 사용하는 View 레이어
	// FAILED_CHECK(Ready_Object_Clone_View(TAG_LAY(Layer_View)));

	// 맵 수정에 사용하는 map 레이어
	// FAILED_CHECK(Ready_Object_Clone_Map(TAG_LAY(Layer_Map)));

	FAILED_CHECK(Ready_Object_Camera(TAG_LAY(Layer_Camera_Main)));
	return S_OK;
}

HRESULT CSuperToolSIngleton::Ready_Object_Component()
{

	// #Tag 툴 모든 컴포넌트의 원형 생성

	//렌더러 컴객체 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), m_pComRenderer = CRenderer::Create(m_pGraphicDevice)));

	Safe_AddRef(m_pComRenderer);

	//버퍼인덱스 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), CVIBuffer_Rect::Create(m_pGraphicDevice)));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube), CVIBuffer_Cube::Create(m_pGraphicDevice)));

	CVIBuffer_Terrain::TERRAINDESC desc;
	desc.iCol = 129;
	desc.iRow = 129;
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Terrain128x128), CVIBuffer_Terrain::Create(m_pGraphicDevice,&desc)));


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

	

	// #Tag 툴 모든 오브젝트의 원형 생성

	// Prototype_GameObject_BackGround
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_Prototype(TAG_OP(Prototype_BackGround), CObjectTool_ToolObject::Create(m_pGraphicDevice)));

	// 카메라 생성
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_Prototype(TAG_OP(Prototype_Camera_Main), CCamera_Tool::Create(m_pGraphicDevice)));
	
	// 지형 생성
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_Prototype(TAG_OP(Prototype_TerrainGround), CObjectTool_Terrain::Create(m_pGraphicDevice)));


	// 테스트
	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, TAG_LAY(Layer_Terrain), TAG_OP(Prototype_TerrainGround)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CSuperToolSIngleton::Ready_Object_Clone_View(const _tchar* layertag)
{
	// 

	//// BackObj
	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, layertag, TAG_OP(Prototype_BackGround)))
	//	return E_FAIL;

	//// 최초 객체는 0번쨰에 저장한다.
	//m_Object_Rect = (CObjectTool_ToolObject*)GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENEID::SCENE_STATIC, layertag);
	//m_Object_Rect->Set_Defult(L"DefaultObject");
	//Safe_AddRef(m_Object_Rect);

	return S_OK;
}

HRESULT CSuperToolSIngleton::Ready_Object_Clone_Map(const _tchar* layertag)
{
	// 
	//if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, layertag, TAG_OP(Prototype_BackGround)))
	//	return E_FAIL;

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

	lstrcat(szPath, g_FilePath_ObjectPathData_Save.c_str());
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		// 이름

		CString				str = Dlg.GetPathName().GetString();
		CString				Filename = PathFindFileName(str);
		
		TCHAR				newName[64] = L"";
	
		lstrcpy(newName, Filename);
		PathRemoveExtension(newName);		

		
		
		const TCHAR*		pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;


		DWORD	dwByte = 0;

		// 파일 ID / 정보
		
		OUTPUT_ID id;
		id.FILEID = OUTPUT_OBJECT;
		lstrcpy(id.strObjectName, newName);

		obj->Set_OUTPUTData_Save();
		WriteFile(hFile, &id, sizeof(OUTPUT_ID), &dwByte, nullptr);
		WriteFile(hFile, &obj->Get_OutputData(), sizeof(OUTPUT_OBJECTINFO), &dwByte, nullptr);

		CloseHandle(hFile);
	}
	return S_OK;
}

HRESULT CSuperToolSIngleton::SaveData_Map(list<CObjectTool_ToolObject*> objlist, CWnd* cwnd)
{
	// 선택한 오브젝트를 저장한다.

	// 1. 클릭시 다이얼 로그 생성.
	//CFileDialog		Dlg(FALSE,
	//	L"dat", // .dat파일로 저장
	//	L"*.dat",
	//	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	//	L"Data Files(*.dat)|*.dat||",
	//	cwnd);

	//TCHAR	szPath[MAX_PATH] = L"";
	//GetCurrentDirectory(MAX_PATH, szPath);
	//PathRemoveFileSpec(szPath);

	//lstrcat(szPath, g_FilePath_ObjectPathData_Save.c_str());
	//Dlg.m_ofn.lpstrInitialDir = szPath;

	//if (IDOK == Dlg.DoModal())
	//{
	//	// 이름

	//	CString				str = Dlg.GetPathName().GetString();
	//	CString				Filename = PathFindFileName(str);

	//	TCHAR				newName[64] = L"";

	//	lstrcpy(newName, Filename);
	//	PathRemoveExtension(newName);
	//	

	//	const TCHAR*		pGetPath = str.GetString();

	//	HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	//	if (INVALID_HANDLE_VALUE == hFile)
	//		return E_FAIL;


	//	DWORD	dwByte = 0;

	//	// 저장
	//	obj->Set_OUTPUTData_Save();
	//	WriteFile(hFile, &obj->Get_OutputData(), sizeof(OUTPUT_OBJECTINFO), &dwByte, nullptr);

	//	CloseHandle(hFile);
	//}
	return S_OK;
}


HRESULT CSuperToolSIngleton::LoadData_Object(CWnd * cwnd)
{
	// 새 오브젝트를 가져온다.

	//	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");

	static TCHAR BASED_CODE szFilter[] = _T("데이터 파일(*.dat) | *.dat; | 모든파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.dat"), _T(""), OFN_HIDEREADONLY, szFilter);

	OUTPUT_ID			infoid;

	OUTPUT_OBJECTINFO infoObj;


	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();

		// 이름
		CString				str = dlg.GetPathName().GetString();
		const TCHAR*		pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		// 해당 공간에 오브젝트 정보에 해당하는 값을 넣는다.
		DWORD	dwByte = 0;

		// 로드
		ReadFile(hFile, &infoid, sizeof(OUTPUT_ID), &dwByte, nullptr);
		switch (infoid.FILEID)
		{
		case OUTPUT_OBJECT:
			ReadFile(hFile, &infoObj, sizeof(OUTPUT_OBJECTINFO), &dwByte, nullptr);
			// 새 오브젝트 생성
			Create_ToolObject_Data(infoid.strObjectName,infoObj);
			break;
		case OUTPUT_MAP:
			break;
		}

		

		CloseHandle(hFile);
	}

	return S_OK;
}

HRESULT CSuperToolSIngleton::LoadData_Map(CWnd * cwnd)
{
	return S_OK;
}

HRESULT CSuperToolSIngleton::Create_ToolObject_Button(wstring name)
{
	// 버튼을 누르면 새 오브젝트 생성.
	//FAILED_CHECK(Change_ToolObject(Create_New_ToolObject(name)));

	Create_New_ToolObject(name,TAG_LAY(Layer_View));

	// 리스트 박스 업데이트
	m_pMyButtomView->Update_ViewListBox();
	return S_OK;
}

HRESULT CSuperToolSIngleton::Create_ToolObject_Data(const _tchar* str ,const OUTPUT_OBJECTINFO& data)
{
	// 새 오브젝트 생성뒤 넣기
	// 1. 디폴트 오브젝트 생성
	CObjectTool_ToolObject* newobj = Create_New_ToolObject(str,TAG_LAY(Layer_View));
	// 2. 데이터 세팅
	newobj->LoadData(data);

	// 리스타 박스 업데이트
	m_pMyButtomView->Update_ViewListBox();
	return S_OK;
}

CObjectTool_ToolObject* CSuperToolSIngleton::Create_New_ToolObject(wstring name,const _tchar* laytag)
{
	// 현재 오브젝트를 저장하고 새 큐브 오브젝트를 만든다.
	// 새로 만든 객체는 여기 클래스의 Vector에 넣는다.
	FAILED_CHECK_NONERETURN(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, laytag, TAG_OP(Prototype_BackGround)));

	CObjectTool_ToolObject* newobj = (CObjectTool_ToolObject*)GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, laytag)->back();
	newobj->Set_Default(name);

	return newobj;
}

CObjectTool_ToolObject * CSuperToolSIngleton::Create_New_MapObject(_float3 Pos, const _tchar* laytag)
{
	// 현재 선택된 오브젝트를 클론해 맵 레이어에 오브젝트를 제작한다.
	// 1. 오브젝트 선택
	CObjectTool_ToolObject* cloneobj = Get_ViewObject_SelectObject();
	if (cloneobj == nullptr)
		return nullptr;

	// 데이터 받아옴
	const OUTPUT_OBJECTINFO& info = cloneobj->Get_OutputData();

	// 2. 오브젝트 생성
	FAILED_CHECK_NONERETURN(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, laytag, TAG_OP(Prototype_BackGround)));
	
	// 3. 생성된 오브젝트 가져오기
	CObjectTool_ToolObject* newobj = (CObjectTool_ToolObject*)GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, laytag)->back();
	newobj->LoadData(info);
	newobj->Set_Position(Pos);
	return newobj;
}

CObjectTool_ToolObject * CSuperToolSIngleton::Get_ViewObject_Object(int index)
{
	auto list  = Get_GameObjectList(TAG_LAY(Layer_View));

	if (list->empty())
		return nullptr;

	if(index == 0)
		return static_cast<CObjectTool_ToolObject*>(list->front());
	else
	{
		int count = 0;
		for (auto iter :*list)
		{
			if (index == count)
				return static_cast<CObjectTool_ToolObject*>(iter);
			count++;
		}
	}
	return nullptr;
}

CObjectTool_ToolObject * CSuperToolSIngleton::Get_ViewObject_SelectObject()
{
	if (m_ViewObjectSelectIndex == -1)
		return nullptr;

	auto gameobj = GetSingle(CGameInstance)->Get_GameObject_By_LayerIndex(SCENEID::SCENE_STATIC, TAG_LAY(Layer_View), m_ViewObjectSelectIndex);

	if (gameobj == nullptr)
		return nullptr;

	return static_cast<CObjectTool_ToolObject*>(gameobj);
}

void CSuperToolSIngleton::Set_ViewObject_Index(int index)
{
	int viewObjSize = Get_GameObjectList(TAG_LAY(Layer_View))->size();

	if (m_ViewObjectSelectIndex >= viewObjSize)
		index = -1;
	else
		m_ViewObjectSelectIndex = index;

}

list<CGameObject*>* CSuperToolSIngleton::Get_GameObjectList(const _tchar* layertag)
{
	 return  GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, layertag);
}

HRESULT CSuperToolSIngleton::Update_Select_Render_None(const _tchar* laytag)
{
	list<CGameObject*>* objlist = Get_GameObjectList(laytag);

	for (auto obj : *objlist)
	{
		((CObjectTool_ToolObject*)obj)->Set_Visble(false);
	}
	return S_OK;
}

HRESULT CSuperToolSIngleton::Update_Select_Render_Visble(const _tchar* laytag, CObjectTool_ToolObject* visbleobj)
{
	list<CGameObject*>* objlist = Get_GameObjectList(laytag);

	for (auto obj : *objlist)
	{
		if (visbleobj == obj)
		{
			((CObjectTool_ToolObject*)obj)->Set_Visble(true);
			return S_OK;
		}
	}

	return E_FAIL;
}

void CSuperToolSIngleton::Free()
{

	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pComRenderer);
	Safe_Release(m_pGameInstance);
	m_pGameInstance->Release_Engine();
}