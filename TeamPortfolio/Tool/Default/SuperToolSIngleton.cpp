#include "stdafx.h"
#include "SuperToolSIngleton.h"
#include "GameInstance.h"
#include "Layer.h"
#include "ObjectTool_ToolObject.h"
#include "ObjectTool_Terrain.h"
#include "ObjectTool_ToolWire.h"

#include "Camera_Tool.h"
#include "FileInfo.h"

IMPLEMENT_SINGLETON(CSuperToolSIngleton)

CSuperToolSIngleton::E_TOOL_MODE CSuperToolSIngleton::g_MAP_MODE = CSuperToolSIngleton::E_TOOL_MODE::TOOLMODE_OBJECT;

#define LIGHTID_DIRECTION	0
#define LIGHTID_POINT		1
#define LIGHTID_SPOT		2


CSuperToolSIngleton::CSuperToolSIngleton()
	: m_pGraphicDevice(nullptr), m_pGameInstance(GetSingle(CGameInstance))
{
	m_pComRenderer = nullptr;
	m_ViewObjectSelectIndex = -1;
	Safe_AddRef(m_pGameInstance);
	GetSingle(CKeyMgr);

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

	// 조명 초기화 및 설정
	Init_Light();



	// 법선 연산 / 스펙큘러 설정
	m_WhiteMtrl = InitMtrl(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_BLACK, 1.0f);


	// 조명 무효
	// m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, false);



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
	Update_Light();

	return S_OK;
}

HRESULT CSuperToolSIngleton::Render_Begin(D3DCOLOR backbuffer)
{
	if (m_pGameInstance == nullptr)
		return E_FAIL;

	Render_Set_State();
	m_pGameInstance->Render_Begin(backbuffer);
	return S_OK;
}

HRESULT CSuperToolSIngleton::Render_End(HWND hWnd)
{
	if (m_pGameInstance == nullptr)
		return E_FAIL;
	m_pGraphicDevice->EndScene();

	m_pGraphicDevice->Present(NULL, NULL, hWnd, NULL);

	return S_OK;
}

HRESULT CSuperToolSIngleton::Render_Set_State()
{
	if (m_pGraphicDevice == nullptr)
		return E_FAIL;

	_bool bWire = (_bool)m_pMyButtomView->m_CheckWirframeEnable.GetCheck();

	if (bWire)
		m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DRS_FILLMODE);

	m_pGraphicDevice->SetMaterial(&m_WhiteMtrl);


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

HRESULT CSuperToolSIngleton::Init_Light()
{
	D3DXVECTOR3 dir(-1, -1, -1);
	D3DXCOLOR   c = D3DXCOLOR(COLOR_WHITE);
	D3DLIGHT9 dirLight = InitDirectionalLight(&dir, &c);
	// Point 조명
	D3DXVECTOR3 pointPos(0, 5, 0);
	D3DLIGHT9 pointLight = InitPointLight(&pointPos, &c);
	// Spot 조명
	D3DXVECTOR3 SpotPos(0, 0, -5);
	D3DXVECTOR3 SpotDir(0, 0, 1);
	D3DLIGHT9 SpotLight = InitSpotLight(&SpotPos, &SpotDir, &c);

	m_pGraphicDevice->SetLight(0, &dirLight);
	m_pGraphicDevice->SetLight(1, &pointLight);
	m_pGraphicDevice->SetLight(2, &SpotLight);
	

	m_pGraphicDevice->LightEnable(LIGHTID_DIRECTION, true);
//	m_pGraphicDevice->LightEnable(LIGHTID_POINT, true);
//	m_pGraphicDevice->LightEnable(LIGHTID_SPOT, true);

	m_pGraphicDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pGraphicDevice->SetRenderState(D3DRS_SPECULARENABLE, false);

	return S_OK;
}

HRESULT CSuperToolSIngleton::Update_Light()
{
	// 장치에서 조명을 얻어온다.
	D3DLIGHT9 spotlight;
	m_pGraphicDevice->GetLight(LIGHTID_SPOT, &spotlight);

	// 카메라 위치를 얻어온다.
	_Matrix ViewMat;
	m_pGraphicDevice->GetTransform(D3DTS_VIEW, &ViewMat);
	_Matrix vCamMatirx = ViewMat.InverseMatrix();
	_float3 vCamPos = (*(_float3*)(&vCamMatirx.m[3][0]));
	_float3 vCamLook = (*(_float3*)(&vCamMatirx.m[2][0]));
	vCamLook = vCamLook.Get_Nomalize();

	// 카메라 위치에 조명 위치 업데이트
	spotlight.Position = vCamPos;
	spotlight.Direction = vCamLook;
	m_pGraphicDevice->SetLight(LIGHTID_SPOT, &spotlight);

	return S_OK;
}

HRESULT CSuperToolSIngleton::Ready_Object_Component()
{

	// #ToolTag 툴 모든 컴포넌트의 원형 생성

	//렌더러 컴객체 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TAG_CP(Prototype_Renderer), m_pComRenderer = CRenderer::Create(m_pGraphicDevice)));

	Safe_AddRef(m_pComRenderer);

	//버퍼인덱스 프로토타입 생성
	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), CVIBuffer_Rect::Create(m_pGraphicDevice)));

	FAILED_CHECK(m_pGameInstance->Add_Component_Prototype
	(SCENEID::SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube), CVIBuffer_Cube_Normal::Create(m_pGraphicDevice)));

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

	

	// #ToolTag 툴 모든 오브젝트의 원형 생성

	// Prototype_GameObject_BackGround
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_Prototype(TAG_OP(Prototype_BackGround), CObjectTool_ToolObject::Create(m_pGraphicDevice)));
	// 와이어 큐브
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_Prototype(TAG_OP(Prototype_WIRECUBE), CObjectTool_ToolWire::Create(m_pGraphicDevice)));


	// 카메라 생성
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_Prototype(TAG_OP(Prototype_Camera_Main), CCamera_Tool::Create(m_pGraphicDevice)));
	
	// 지형 생성
	FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_Prototype(TAG_OP(Prototype_TerrainGround), CObjectTool_Terrain::Create(m_pGraphicDevice)));


	// #ToolTag 지형 테스트 까는 것
//	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, TAG_LAY(Layer_Terrain), TAG_OP(Prototype_TerrainGround)))
//		return E_FAIL;

	// 와이어 큐브는 한개만 생성
	if (GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, TAG_LAY(Layer_WireCube), TAG_OP(Prototype_WIRECUBE)))
	return E_FAIL;

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

	m_pMyButtomView = static_cast<CMyForm*>(m_pMainFrame->m_SecondSplitter.GetPane(0, 0));
	m_pMiniView = static_cast<CMiniView*>(m_pMainFrame->m_SecondSplitter.GetPane(1, 0));

	m_pTransDialog = &m_pMyButtomView->m_TransformDialog;
	m_pPathDialog = &m_pMyButtomView->m_PathFind;
	m_pMapToolDialog = &m_pMyButtomView->m_MapToolDialog;

	return S_OK;
}

HRESULT CSuperToolSIngleton::Initialize_ToolView_Data()
{
	// 최초 데이터 세팅 / 
	
	// 최초 텍스처 로딩

	
	return S_OK;

}

D3DLIGHT9 CSuperToolSIngleton::InitDirectionalLight(D3DXVECTOR3 * direction, D3DXCOLOR * color)
{

	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color * 0.7f;
	light.Diffuse = *color;
	light.Specular = *color * 0.8f;
	light.Direction = *direction;

	return light;
}

D3DLIGHT9 CSuperToolSIngleton::InitPointLight(D3DXVECTOR3 * position, D3DXCOLOR * color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_POINT;
	light.Ambient = *color * 0.2f;
	light.Diffuse = *color;
	light.Specular = *color * 0.3f;
	light.Position = *position;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 0.5f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;

	return light;
}

D3DLIGHT9 CSuperToolSIngleton::InitSpotLight(D3DXVECTOR3 * position, D3DXVECTOR3 * direction, D3DXCOLOR * color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_SPOT;
	light.Ambient = *color * 0.4f;
	light.Diffuse = *color;
	light.Specular = *color * 0.8f;
	light.Position = *position;
	light.Direction = *direction;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta = 1.5f;
	light.Phi = 1.0f;

	return light;
}

D3DMATERIAL9 CSuperToolSIngleton::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
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

HRESULT CSuperToolSIngleton::SaveData_Map(list<CGameObject*> objlist, CWnd* cwnd)
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

	lstrcat(szPath, g_FilePath_MapPathData_Save.c_str());
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

		OUTPUT_ID id;
		id.FILEID = OUTPUT_MAP;
		lstrcpy(id.strObjectName, newName);

		// 저장
		DWORD	dwByte = 0;	

		WriteFile(hFile, &id, sizeof(OUTPUT_ID), &dwByte, nullptr);

		for (auto& iter : objlist)
		{
			((CObjectTool_ToolObject*)iter)->Set_OUTPUTData_Save();
			WriteFile(hFile, &((CObjectTool_ToolObject*)iter)->Get_OutputData(), sizeof(OUTPUT_OBJECTINFO), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
	return S_OK;
}


HRESULT CSuperToolSIngleton::LoadData_Data(CWnd * cwnd)
{
	// 새 오브젝트를 가져온다.

	//	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");

	static TCHAR BASED_CODE szFilter[] = _T("데이터 파일(*.dat) | *.dat; | 모든파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.dat"), _T(""), OFN_HIDEREADONLY, szFilter);

	OUTPUT_ID			infoid;
	OUTPUT_OBJECTINFO	infoObj;


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
			while (true)
			{
				ReadFile(hFile, &infoObj, sizeof(OUTPUT_OBJECTINFO), &dwByte, nullptr);

				if (0 == dwByte)
				{
					// Safe_Delete(pObjInfo);
					break;
				}
				GetSingle(CSuperToolSIngleton)->Create_Load_MapObject(infoObj, TAG_LAY(Layer_Map));
			}
			break;
		}

		CloseHandle(hFile);
	}

	return S_OK;
}


HRESULT CSuperToolSIngleton::LoadData_ObjectFile()
{
	// 기본 오브젝트 파일 로드
	
	// 1. 디렉토리의 모든 파일 Path 얻어옴
	CFileFind find;

	list<wstring> objPathList;
	CFileInfo::DirInfoExtraction_Custom(g_FilePath_ObjectsPathData_Load, objPathList, FILETYPE_DAT);
	


	// 2. 해당 오브젝트 만큼 로드

	for (auto path : objPathList)
	{
		HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		// 해당 공간에 오브젝트 정보에 해당하는 값을 넣는다.
		DWORD	dwByte = 0;
		OUTPUT_ID id;
		OUTPUT_OBJECTINFO data;

		// 로드
		ReadFile(hFile, &id, sizeof(OUTPUT_ID), &dwByte, nullptr);
		switch (id.FILEID)
		{
		case OUTPUT_OBJECT:
			ReadFile(hFile, &data, sizeof(OUTPUT_OBJECTINFO), &dwByte, nullptr);
			// 새 오브젝트 생성
			Create_ToolObject_Data(id.strObjectName, data);
			break;
		case OUTPUT_MAP:
			break;
		}

		CloseHandle(hFile);
	}
	

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

CObjectTool_ToolObject * CSuperToolSIngleton::Create_Load_MapObject(const OUTPUT_OBJECTINFO& info, const _tchar* laytag)
{
	// 데이터로 오브젝트 생성 생성
	FAILED_CHECK_NONERETURN(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, laytag, TAG_OP(Prototype_BackGround)));

	// 3. 생성된 오브젝트 가져오기
	CObjectTool_ToolObject* newobj = (CObjectTool_ToolObject*)GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, laytag)->back();
	newobj->LoadData(info);
	return newobj;
}


CObjectTool_ToolObject * CSuperToolSIngleton::Create_Clone_MapObject(_float3 Pos, const _tchar* laytag, _bool bRandom,_uint texIndex)
{
	// 현재 선택된 오브젝트를 클론해 맵 레이어에 오브젝트를 제작한다.
	// 1. 오브젝트 선택
	CObjectTool_ToolObject* cloneobj = Get_ViewObject_SelectObject();
	if (cloneobj == nullptr)
		return nullptr;

	// 데이터 받아옴
	const OUTPUT_OBJECTINFO& info = cloneobj->Get_OutputData();
	OUTPUT_OBJECTINFO newinfo(info);
	//if (bRandom)
	//{
	//	newinfo.Set_StateIndex(texIndex);
	//}


	// 2. 오브젝트 생성
	FAILED_CHECK_NONERETURN(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, laytag, TAG_OP(Prototype_BackGround)));
	
	// 3. 생성된 오브젝트 가져오기
	CObjectTool_ToolObject* newobj = (CObjectTool_ToolObject*)GetSingle(CGameInstance)->Get_ObjectList_from_Layer(SCENEID::SCENE_STATIC, laytag)->back();
	newobj->LoadData(newinfo);	
	newobj->Set_WorldMat(cloneobj->Get_Matrix());
	newobj->Set_Position(Pos);
	return newobj;
}

_float3 CSuperToolSIngleton::Get_Center_MapPosition()
{
	// 생성된 맵의 중심위치 반환
	_float3 newCenterPos = _float3(0, 0, 0);
	auto maplist =  Get_GameObjectList(TAG_LAY(Layer_Map));
	if (maplist == nullptr)
		return newCenterPos;

	
	for (auto object : *maplist)
	{
		CTransform* trans = (CTransform*) object->Get_Component(TAG_COM(Com_Transform));
		_float3 pos = trans->Get_MatrixState(CTransform::STATE_POS);
		newCenterPos.x = (newCenterPos.x < pos.x) ? pos.x : newCenterPos.x;
		newCenterPos.y = (newCenterPos.y < pos.y) ? pos.y : newCenterPos.y;
		newCenterPos.z = (newCenterPos.z < pos.z) ? pos.z : newCenterPos.z;
	}

	newCenterPos *= 0.5f;
	return newCenterPos;
}

CObjectTool_ToolObject * CSuperToolSIngleton::Get_ViewObject_Object(int index)
{
	auto list  = Get_GameObjectList(TAG_LAY(Layer_View));

	if (list== nullptr)
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

CObjectTool_ToolWire * CSuperToolSIngleton::Get_WireCube()
{
	CGameObject* cubeobj = m_pGameInstance->Get_GameObject_By_LayerIndex(0, TAG_LAY(Layer_WireCube), 0);

	return static_cast<CObjectTool_ToolWire*>(cubeobj);
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
	if (objlist == nullptr)
		return E_FAIL;

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

	GetSingle(CKeyMgr)->DestroyInstance();
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pComRenderer);
	Safe_Release(m_pGameInstance);
	m_pGameInstance->Release_Engine();
}