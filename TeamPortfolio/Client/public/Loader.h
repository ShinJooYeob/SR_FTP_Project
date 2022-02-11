#pragma once

#include "Base.h"

BEGIN(Client)

class CLoader final : public CBase
{


private:
	explicit CLoader();
	virtual ~CLoader() = default;

public:
	HRESULT Initialize(LPDIRECT3DDEVICE9 pGraphicDevice, SCENEID eSceneID);

public:
	_bool IsLoadFinished() { return m_bIsLoadingFinished; };
	_int Get_ProgressCount() { return m_iLoadingProgressCount; };
	_int Get_MaxCount() { return m_iLoadingMaxCount; };
	SCENEID Get_SceneID() {	return m_eSceneID;}


public:
	HRESULT Load_Scene_Loby(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_StageSelect(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_Stage1(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_Stage2(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	HRESULT Load_Scene_Stage3(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);




private:
	//로딩이 끝낫음을 알려주는 변수
	_bool m_bIsLoadingFinished = false;
	
	//로딩바 게이지를 위한 변수
	_int m_iLoadingMaxCount = 1;
	_int m_iLoadingProgressCount = 1;

	SCENEID  m_eSceneID = SCENEID::SCENE_END;
	LPDIRECT3DDEVICE9 m_pGraphicDevice = nullptr;

public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphicDevice, SCENEID eSceneID);
	virtual void Free() override;
};

END