#pragma once
#include "Base.h"

BEGIN(Engine)
class CScene;


class CSceneMgr :public CBase
{
	DECLARE_SINGLETON(CSceneMgr)

private:
	explicit CSceneMgr();
	virtual ~CSceneMgr() = default;


public:
	HRESULT Scene_Chage(CScene * _pScene, _int iNextSceneIndex, _bool bIsNotLoadingScene = false);
	_uint	Get_NowSceneNum() {	return (_uint)m_iNowSceneIndx_ExceptLoad;}
	void	Set_iNowSceneIndx_ExceptLoad(_int iSceneNum) {  m_iNowSceneIndx_ExceptLoad= iSceneNum; }
public:
	_int Update(_float fDeltaTime);
	_int LateUpdate(_float fDeltaTime);
	_int Render();
	_int LateRender();

	//¿∫«ı¿Ã∞° ∞«µÎ
public:
	CScene*				Get_Scene();

private:
	_int				m_iNowSceneIndx_ExceptLoad = 0;
	_int				m_iCurrentSceneIdx = 0;

	CScene*				m_pScene = nullptr;
public:
	virtual void Free()override;
};


END