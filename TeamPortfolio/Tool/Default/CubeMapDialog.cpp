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
void CreateCubeMap()
{
	// 큐브 텍스처 초기화
	LPDIRECT3DCUBETEXTURE9 pCubeMap;
	LPDIRECT3DDEVICE9 device = GetSingle(CSuperToolSIngleton)->Get_Graphics_Device();

	FAILED_CHECK_NONERETURN(device->CreateCubeTexture(256, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pCubeMap, NULL));


	// 백버퍼에 랜더 타겟 변경 
	LPDIRECT3DSURFACE9 pBackBuffer, pZBuffer;


	//D3DXVECTOR3 vEyePt2(0, 0, 0.0f);

	//for (DWORD i = 0; i < 6; i++)
	//{
	//	D3DXVECTOR3* vEnvEyePt = &vEyePt2;
	//	D3DXVECTOR3 vLookatPt, vUpVec;
	//	switch (i)
	//	{
	//	case D3DCUBEMAP_FACE_POSITIVE_X:
	//		vLookatPt = D3DXVECTOR3(vEnvEyePt->x + 1.0f, vEnvEyePt->y, vEnvEyePt->z);
	//		vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//		break;
	//	case D3DCUBEMAP_FACE_NEGATIVE_X:
	//		vLookatPt = D3DXVECTOR3(vEnvEyePt->x - 1.0f, vEnvEyePt->y, vEnvEyePt->z);
	//		vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//		break;
	//	case D3DCUBEMAP_FACE_POSITIVE_Y:
	//		vLookatPt = D3DXVECTOR3(vEnvEyePt->x, vEnvEyePt->y + 1.0f, vEnvEyePt->z);
	//		vUpVec = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//		break;
	//	case D3DCUBEMAP_FACE_NEGATIVE_Y:
	//		vLookatPt = D3DXVECTOR3(vEnvEyePt->x, vEnvEyePt->y - 1.0f, vEnvEyePt->z);
	//		vUpVec = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//		break;
	//	case D3DCUBEMAP_FACE_POSITIVE_Z:
	//		vLookatPt = D3DXVECTOR3(vEnvEyePt->x, vEnvEyePt->y, vEnvEyePt->z + 1.0f);
	//		vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//		break;
	//	case D3DCUBEMAP_FACE_NEGATIVE_Z:
	//		vLookatPt = D3DXVECTOR3(vEnvEyePt->x, vEnvEyePt->y, vEnvEyePt->z - 1.0f);
	//		vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//		break;
	//	}

	//	D3DXMATRIX matView;
	//	D3DXMatrixLookAtLH(&matView, vEnvEyePt, &vLookatPt, &vUpVec);
	//	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	//	LPDIRECT3DSURFACE9 pFace;
	//	pCubeMap->GetCubeMapSurface((D3DCUBEMAP_FACES)i, 0, &pFace);
	//	g_pd3dDevice->SetRenderTarget(0, pFace);

	//	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	//	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	//	{
	//		for (DWORD i = 0; i < g_dwNumMaterials; i++)
	//		{
	//			g_pd3dDevice->SetMaterial(&g_pMeshMaterials[i]);
	//			g_pd3dDevice->SetTexture(0, g_pMeshTextures[i]);
	//			g_pMesh->DrawSubset(i);
	//		}

	//		g_pd3dDevice->EndScene();
	//	}

	//	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//	pFace->Release();
	//}
	TCHAR str[64] = L"";
	static int i = 0;
	i++;
	wsprintf(str, L"DDS/cubemap_MakeDDS_%d.dds", i);
	D3DXSaveTextureToFile(str, D3DXIFF_DDS, pCubeMap, NULL);

}



void CCubeMapDialog::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CreateCubeMap();

}


BOOL CCubeMapDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
