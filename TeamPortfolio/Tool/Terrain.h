#pragma once

#include "Include.h"

class CToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	HRESULT		Initialize(void);
	void		Update(void);
	void		MiniRender(void);
	void		Render(void);
	void		Release(void);

	vector<TILE*>&		Get_Tile(void) { return m_vecTile; }

	void	TileChange(const D3DXVECTOR3& vPos, const int& iTileIdx);

	void	SetMainView(CToolView* pMainView) { m_pMainView = pMainView;  }

 private:
	int		GetTileIndex(const D3DXVECTOR3& vPos);
	// bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex); // 직선의 방정식
	bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex);

	void	SetRatio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

private:
	vector<TILE*>		m_vecTile;
	CToolView*			m_pMainView = nullptr;

};

