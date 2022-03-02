// MapTool.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CubeMapDialog.h"
#include "afxdialogex.h"
#include "ObjectTool_ToolObject.h"



// MapTool 대화 상자

IMPLEMENT_DYNAMIC(CCubeMapDialog, CDialog)

CCubeMapDialog::CCubeMapDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MYCUBEMAP, pParent)
{
	srand(time(NULL));
	m_pComSprite = nullptr;
	m_pCom_Tex = nullptr;
}

CCubeMapDialog::~CCubeMapDialog()
{

}

void CCubeMapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCubeMapDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CCubeMapDialog::OnBnClickedButton1)
END_MESSAGE_MAP()

// 큐브맵을 제작
void CCubeMapDialog::CubeMapMake()
{
	LPDIRECT3DDEVICE9 device = GetSingle(CSuperToolSIngleton)->Get_Graphics_Device();

	if (m_pComSprite == nullptr)
	{
		D3DXCreateSprite(device, &m_pComSprite);

	}


	wstring path = L"DDS/Test.png";
	if (m_pCom_Tex == nullptr)
		CreateTexute(path);

	// 큐브 텍스처 초기화
	LPDIRECT3DCUBETEXTURE9 pCubeMap;

	device->CreateCubeTexture
	(256, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pCubeMap, NULL);


	// 백버퍼에 랜더 타겟 변경 
	// LPDIRECT3DSURFACE9 pBackBuffer, pZBuffer;

	for (DWORD i = 0; i < 6; i++)
	{


		/*switch (i)
		{
		case D3DCUBEMAP_FACE_POSITIVE_X:
			vLookatPt = D3DXVECTOR3(vEnvEyePt->x + 1.0f, vEnvEyePt->y, vEnvEyePt->z);
			vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;
		case D3DCUBEMAP_FACE_NEGATIVE_X:
			vLookatPt = D3DXVECTOR3(vEnvEyePt->x - 1.0f, vEnvEyePt->y, vEnvEyePt->z);
			vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;
		case D3DCUBEMAP_FACE_POSITIVE_Y:
			vLookatPt = D3DXVECTOR3(vEnvEyePt->x, vEnvEyePt->y + 1.0f, vEnvEyePt->z);
			vUpVec = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			break;
		case D3DCUBEMAP_FACE_NEGATIVE_Y:
			vLookatPt = D3DXVECTOR3(vEnvEyePt->x, vEnvEyePt->y - 1.0f, vEnvEyePt->z);
			vUpVec = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			break;
		case D3DCUBEMAP_FACE_POSITIVE_Z:
			vLookatPt = D3DXVECTOR3(vEnvEyePt->x, vEnvEyePt->y, vEnvEyePt->z + 1.0f);
			vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;
		case D3DCUBEMAP_FACE_NEGATIVE_Z:
			vLookatPt = D3DXVECTOR3(vEnvEyePt->x, vEnvEyePt->y, vEnvEyePt->z - 1.0f);
			vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;
		}*/



		LPDIRECT3DSURFACE9 pFace;
		pCubeMap->GetCubeMapSurface((D3DCUBEMAP_FACES)i, 0, &pFace);
		device->SetRenderTarget(0, pFace);

		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,0, 0, 255), 1.0f, 0);

		device->BeginScene();
		m_pComSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_pComSprite->Draw((LPDIRECT3DTEXTURE9)m_pCom_Tex,
			// Texture 객체 pointer.
			NULL, // Sprite 영역. 
			NULL, // Sprite 중심. 
			NULL, // Sprite 위치.
			0xFFFFFFFF // 색상. 
		);

		m_pComSprite->End();
		device->EndScene();
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		device->Present(NULL, NULL, g_hWnd2, NULL); // 디버그용
		pFace->Release();
	}
	TCHAR str[64] = L"";
	static int i = 0;
	i++;
	wsprintf(str, L"DDS/cubemap_MakeDDS_%d.dds", i);
	D3DXSaveTextureToFile(str, D3DXIFF_DDS, pCubeMap, NULL);


	// 1. png 이미지를 복사한다.
	// 2. 각 면을 32x32로 복사한다.
	// 3. png 이미지를 복사한다.
	// 4. 각 면을 32x32로 복사한다.

}

HRESULT CCubeMapDialog::CreateTexute(wstring path)
{
	// _tchar	szFullPath[MAX_PATH] = L"";
	// wsprintf(szFullPath, tTextureLayerDesc.szFilePath, i);

	LPDIRECT3DBASETEXTURE9		pTexture = nullptr;
	HRESULT		hr = 0;

	LPDIRECT3DDEVICE9 device = GetSingle(CSuperToolSIngleton)->Get_Graphics_Device();
	hr = D3DXCreateTextureFromFile(device, path.c_str(), (LPDIRECT3DTEXTURE9*)&pTexture);

	if (FAILED(hr))
		return E_FAIL;
	return S_OK;

	//if (tTextureLayerDesc.eTextureType == 0)
	//	hr = D3DXCreateTextureFromFile(m_pGraphicDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
	//else
	//	hr = D3DXCreateCubeTextureFromFile(m_pGraphicDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);

	//if (FAILED(hr))
	//	return E_FAIL;



}

void CCubeMapDialog::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CubeMapMake();
}


BOOL CCubeMapDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
