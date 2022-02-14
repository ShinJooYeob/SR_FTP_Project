#include "stdafx.h"
#include "Terrain.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ToolView.h"

CTerrain::CTerrain()
{
}


CTerrain::~CTerrain()
{
	Release();
}

HRESULT CTerrain::Initialize(void)
{
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(TEX_MULTI, L"../Texture/Stage/Terrain/Tile/Tile%d.png", L"Terrain", L"Tile", 36)))
	{
		AfxMessageBox(L"Tile Texture Insert Failed");
		return E_FAIL;
	}

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE*		pTile = new TILE;

			float		fY = (TILECY / 2.f) * i;
			float		fX = (TILECX * j) + (i % 2) * (TILECX / 2.f);

			pTile->vPos = D3DXVECTOR3(fX, fY, 0.f);
			pTile->vSize = D3DXVECTOR3((float)TILECX, (float)TILECY, 0.f);
			pTile->byOption = 0;
			pTile->byTileIdx = 3;
			pTile->iIndex = i * TILEX + j;
			pTile->iParentIndex = 0;

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

void CTerrain::Update(void)
{

}

void CTerrain::MiniRender(void)
{
	D3DXMATRIX	matWorld, matScale, matTrans;
	if (m_vecTile.empty())
		return;

	for (auto& iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x - m_pMainView->GetScrollPos(0),  // 0�� x�� 1�� y���� ���´ٴ� ��
			iter->vPos.y - m_pMainView->GetScrollPos(1),
			iter->vPos.z);

		matWorld = matScale * matTrans;
		
		SetRatio(&matWorld, 0.3f, 0.3f);

		const	TEXINFO*		pTextureInfo = CTextureMgr::GetInstance()->Get_Texture(L"Terrain", L"Tile", iter->byTileIdx);
		if (nullptr == pTextureInfo)
			return;

		// �̹����� �߽� ��ǥ
		float	fCenterX = pTextureInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTextureInfo->tImgInfo.Height / 2.f;

		CDevice::GetInstance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->Get_Sprite()->Draw(pTextureInfo->pTexture,
			nullptr,	// ����� �̹��� ������ ���� ��Ʈ ����ü �ּҰ�, null�� ��� �̹����� 0,0�� �������� ����ϰԵ�
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),	// ����� �̹����� �߽� �� ��ǥ�� vec3 Ÿ���� �ּҰ�, null�� ��� 0,0�� �߽� ��ǥ�� ��
			nullptr,	// ��ġ ��ǥ�� ���� vec3Ÿ���� �ּҰ�, null�� ��� ��ũ�� ���� 0, 0 ��ǥ ���
			D3DCOLOR_ARGB(255, 255, 255, 255));	
	}
}

void CTerrain::Render(void)
{
	RECT		rc;

	::GetClientRect(m_pMainView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);


	D3DXMATRIX	matWorld, matScale, matTrans;
	TCHAR		szBuf[64] = L"";
	int			iIndex = 0;

	for (auto& iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, 
			iter->vPos.x - m_pMainView->GetScrollPos(0),  // 0�� x�� 1�� y���� ���´ٴ� ��
			iter->vPos.y - m_pMainView->GetScrollPos(1), 
			iter->vPos.z);

		matWorld = matScale * matTrans;

		SetRatio(&matWorld, fX, fY);

		const	TEXINFO*		pTextureInfo = CTextureMgr::GetInstance()->Get_Texture(L"Terrain", L"Tile", iter->byTileIdx);
		if (nullptr == pTextureInfo)
			return;

		// �̹����� �߽� ��ǥ
		float	fCenterX = pTextureInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTextureInfo->tImgInfo.Height / 2.f;

		CDevice::GetInstance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->Get_Sprite()->Draw(pTextureInfo->pTexture,
			nullptr,	// ����� �̹��� ������ ���� ��Ʈ ����ü �ּҰ�, null�� ��� �̹����� 0,0�� �������� ����ϰԵ�
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),	// ����� �̹����� �߽� �� ��ǥ�� vec3 Ÿ���� �ּҰ�, null�� ��� 0,0�� �߽� ��ǥ�� ��
			nullptr,	// ��ġ ��ǥ�� ���� vec3Ÿ���� �ּҰ�, null�� ��� ��ũ�� ���� 0, 0 ��ǥ ���
			D3DCOLOR_ARGB(255, 255, 255, 255));

		swprintf_s(szBuf, L"%d", iIndex);

		CDevice::GetInstance()->Get_Font()->DrawTextW(CDevice::GetInstance()->Get_Sprite(),
														szBuf, 
														lstrlen(szBuf), 
														nullptr,
														0, 
														D3DCOLOR_ARGB(255, 255, 0, 0));


		++iIndex;
	}
}

void CTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<TILE*>);
	m_vecTile.clear();
}

int CTerrain::GetTileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t index = 0; index < m_vecTile.size(); ++index)
	{
		if (Picking(vPos, index))
		{
			return index;
		}
	}

	return -1;		// 0�� Ÿ���� �����ϱ� ������ ���� ��ŷ ó���� �ùٸ��� �ʴٸ� ������ ��ȯ�Ѱ� ��
}

void CTerrain::TileChange(const D3DXVECTOR3 & vPos, const int & iTileIdx)
{
	int		iIndex = GetTileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byTileIdx = iTileIdx;
	m_vecTile[iIndex]->byOption  = 1;
}

/*
bool CTerrain::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{

	float		fGradient[4] = { 

		((TILECY / 2.f) / (TILECX / 2.f)) * -1.f,
		((TILECY / 2.f) / (TILECX / 2.f)),
		((TILECY / 2.f) / (TILECX / 2.f)) * -1.f,
		((TILECY / 2.f) / (TILECX / 2.f)),
	};

	// 12�� ������ �������� ������ ��ġ �� ����
	D3DXVECTOR3		vPoint[4] = 
	{
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f),
	};

	// ������ �������� ���ؼ� y������ b�� ������
	// y = ax + b  
	// -b = ax - y
	// b = y - ax

	float	fB[4] = {

		vPoint[0].y - fGradient[0] * vPoint[0].x, 
		vPoint[1].y - fGradient[1] * vPoint[1].x,
		vPoint[2].y - fGradient[2] * vPoint[2].x,
		vPoint[3].y - fGradient[3] * vPoint[3].x
	};

	// 0 = ax + b - y : ���� �� �����ִ�.
	// 0 > ax + b - y : ���� �Ʒ��� �����ִ�.(��ī��Ʈ ����)
	// 0 < ax + b - y : ���� ���� �����ִ�.(��ī��Ʈ ����)
	
	// vPos ������ ���콺�� ������ ��ǥ �����̴�. �׷��� ��ȣ ������ �ʿ��ϴ�.

	bool	bCheck[4] = { false };

	if (0 < fGradient[0] * vPos.x + fB[0] - vPos.y)
		bCheck[0] = true;

	if (0 > fGradient[1] * vPos.x + fB[1] - vPos.y)
		bCheck[1] = true;

	if (0 > fGradient[2] * vPos.x + fB[2] - vPos.y)
		bCheck[2] = true;

	if (0 < fGradient[3] * vPos.x + fB[3] - vPos.y)
		bCheck[3] = true;

	return bCheck[0] && bCheck[1] && bCheck[2] && bCheck[3];
}*/ // ������ ������

// ������ �̿��� ��ŷ
bool CTerrain::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{

	// 12�� ������ �������� ������ ��ġ �� ����
	D3DXVECTOR3		vPoint[4] =
	{
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f),
	};

	D3DXVECTOR3		vDir[4] = {

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	// �� ���� ������ ���� ���͸� �������� �Ѵ�.
	// �������ʹ� �� ���Ϳ� ������ �̷�� ���� ���ͷ� ũ��� �ݵ�� 1�̾�� �Ѵ�.

	D3DXVECTOR3	vNormal[4] = {

		D3DXVECTOR3(-vDir[0].y, vDir[0].x, 0.f),
		D3DXVECTOR3(-vDir[1].y, vDir[1].x, 0.f),
		D3DXVECTOR3(-vDir[2].y, vDir[2].x, 0.f),
		D3DXVECTOR3(-vDir[3].y, vDir[3].x, 0.f)	
	};
	
	// Ÿ���� �� ������ ���콺 ��ġ�� ���ϴ� ���� ���͸� ���Ѵ�.

	D3DXVECTOR3	vMouseDir[4] = {

		vPos - vPoint[0], 
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};
	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		// ���Ⱑ ����� ���(����)
		if (0.f < D3DXVec3Dot(&vMouseDir[i], &vNormal[i]))
			return false;
	}

	return true;
}

void CTerrain::SetRatio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
{
	pOut->_11 *= fRatioX;
	pOut->_21 *= fRatioX;
	pOut->_31 *= fRatioX;
	pOut->_41 *= fRatioX;

	pOut->_12 *= fRatioY;
	pOut->_22 *= fRatioY;
	pOut->_32 *= fRatioY;
	pOut->_42 *= fRatioY;
}
