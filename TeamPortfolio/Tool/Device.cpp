#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	: m_p3D(nullptr) , m_pDevice(nullptr), m_pSprite(nullptr), m_pFont(nullptr)
	, m_GameInstance(GetSingle(CGameInstance))
{
	Safe_AddRef(m_GameInstance);
}

HRESULT CDevice::InitDevice(void)
{

	// 엔진으로 DX9 초기화
	CGraphic_Device::GRAPHICDESC desc;
	desc.hWnd = g_hWnd;
	desc.eScreenMode = CGraphic_Device::TYPE_SCREEN;
	desc.iWinCX = WINCX;
	desc.iWinCY = WINCY;

	// 툴 Init으로 변경
	m_GameInstance->Initialize_Engine_Tool(desc, SCENE_ID::SCENE_ID_END,&m_pDevice);

	// IMGUI 라이브러리때문에 IMGUI 초기화 해줘야한다. / 삭제는 자동으로 매니저에서 삭제
	m_GameInstance->GetIMGui()->Initialize_IMGUI(desc.hWnd, m_pDevice);

	return S_OK;
}

void CDevice::Render_Begin(void)
{
	// 렌더링 과정 : 후면 버퍼를 먼저 비운다(지운다)-> 후면 버퍼에 그린다-> 후면버퍼에 연결된 서피스를 전면버퍼에 연결된 서피스와 교체하여 출력한다.

	// 1인자 : 지우고자 하는 렉트의 개수
	// 2인자 : 지우고자 하는 렉트 배열의 이름(nullptr인 경우 전체 화면 영역에 해당하는 렉터라는 의미)



	m_GameInstance->Render_Begin();

// 2D 이미지를 그릴 수 있도록 장치에게 알림, 현재 렌더링 옵션은 매개변수로 넣어줌
// 알파테스트가 유효한 상태에서 알파블렌딩을 사용하겠다는 옵션
//	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);





}

void CDevice::Render_End(HWND hWnd)
{
//	m_pSprite->End();

	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);

//	GetGameInstance->Render_End(hWnd);

}

void CDevice::Free()
{
	Safe_Release(m_p3D);
	Safe_Release(m_pDevice);
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_GameInstance);
}
