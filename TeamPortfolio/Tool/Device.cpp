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

	// �������� DX9 �ʱ�ȭ
	CGraphic_Device::GRAPHICDESC desc;
	desc.hWnd = g_hWnd;
	desc.eScreenMode = CGraphic_Device::TYPE_SCREEN;
	desc.iWinCX = WINCX;
	desc.iWinCY = WINCY;

	// �� Init���� ����
	m_GameInstance->Initialize_Engine_Tool(desc, SCENE_ID::SCENE_ID_END,&m_pDevice);

	// IMGUI ���̺귯�������� IMGUI �ʱ�ȭ ������Ѵ�. / ������ �ڵ����� �Ŵ������� ����
	m_GameInstance->GetIMGui()->Initialize_IMGUI(desc.hWnd, m_pDevice);

	return S_OK;
}

void CDevice::Render_Begin(void)
{
	// ������ ���� : �ĸ� ���۸� ���� ����(�����)-> �ĸ� ���ۿ� �׸���-> �ĸ���ۿ� ����� ���ǽ��� ������ۿ� ����� ���ǽ��� ��ü�Ͽ� ����Ѵ�.

	// 1���� : ������� �ϴ� ��Ʈ�� ����
	// 2���� : ������� �ϴ� ��Ʈ �迭�� �̸�(nullptr�� ��� ��ü ȭ�� ������ �ش��ϴ� ���Ͷ�� �ǹ�)



	m_GameInstance->Render_Begin();

// 2D �̹����� �׸� �� �ֵ��� ��ġ���� �˸�, ���� ������ �ɼ��� �Ű������� �־���
// �����׽�Ʈ�� ��ȿ�� ���¿��� ���ĺ����� ����ϰڴٴ� �ɼ�
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
