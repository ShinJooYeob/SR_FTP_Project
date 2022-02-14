#include "stdafx.h"
#include "MultiTexture.h"
#include "Device.h"

CMultiTexture::CMultiTexture()
{
}

CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO* CMultiTexture::Get_Texture(const TCHAR* pStateKey /*= L""*/, const int& iCnt /*= 0*/)
{
	auto	iter = find_if(m_mapMultiTex.begin(),
		m_mapMultiTex.end(),
		[&](auto& MyPair)->bool
	{
		if (pStateKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_mapMultiTex.end())
		return nullptr;

	return iter->second[iCnt];
}

HRESULT CMultiTexture::InsertTexture(const TCHAR* pFilePath,
	const TCHAR* pStateKey /*= L""*/,
	const int& iCnt /*= 0*/)
{
	// �츮�� �����ϴ� ����� �ִ� ������ MAX_PATHũ�� ������ �����ϴ�.
	TCHAR		szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iCnt; ++i)
	{
		wsprintf(szFullPath, pFilePath, i);

		TEXINFO*	pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			MSG_BOX(szFullPath);
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->Get_Device(),
			szFullPath,
			pTexInfo->tImgInfo.Width,
			pTexInfo->tImgInfo.Height,
			pTexInfo->tImgInfo.MipLevels,
			0, // D3DUSAGE_RENDERTARGET ȭ�� ��ü��¿� �ؽ�ó ���� �ɼ�
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,  // �̹��� ���͸� ��� : �ȼ��� Ȯ����� �� ��� ó���� ������ ���� ����
			D3DX_DEFAULT,  // �Ӹ��� �̿��� Ȯ�� ��� �� ���͸��� ��� ���� ����
			0,				// colorkey : ������ ����, 0�� �ָ� �÷�Ű ��ȿȭ
			nullptr,		// m_pTexInfo->tImgInfo �̹��� ���� ���
			nullptr,		// �ȷ�Ʈ�� ó���� ������ ����(8��Ʈ ���� ARGB ���� ǥ���ϱ� ���� ���� ����ü)
			&(pTexInfo->pTexture))))
		{
			MSG_BOX(L"MultiTexture Create Failed");
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		m_mapMultiTex[pStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTexture::Release(void)
{
	for_each(m_mapMultiTex.begin(), m_mapMultiTex.end(),
		[](auto& MyPair)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(), Safe_Delete<TEXINFO*>);
		MyPair.second.clear();
	});

	m_mapMultiTex.clear();
}