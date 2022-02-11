#pragma once
#include "Base.h"

BEGIN(Engine)

class CLayer;
class CGameObject;
class CComponent;

class CObjectMgr final :public CBase
{
DECLARE_SINGLETON(CObjectMgr)
private:
	explicit CObjectMgr();
	virtual ~CObjectMgr() = default;


public:
	HRESULT Reserve_Container(_uint _eSceneNum);

	HRESULT Add_GameObject_Prototype(const _tchar* tagPrototype, CGameObject* pPrototype);
	HRESULT Add_GameObject_To_Layer(_uint eSceneNum, const _tchar* tagLayer, const _tchar* tagPrototype, void* pArg);
	HRESULT Clear_Scene_GameObjects(_uint eSceneNum);

	CComponent*		Get_Commponent_By_LayerIndex(_uint eSceneNum, const _tchar* tagLayer, const _tchar* tagComponet, _uint iLayerIndex);
	CGameObject*	Get_GameObject_By_LayerIndex(_uint eSceneNum, const _tchar* tagLayer, _uint iLayerIndex);
	CLayer*			Get_Layer(_uint iSceneNum, const _tchar* tagLayer);

public:
	_int Update(_float fDeltaTime);
	_int LateUpdate(_float fDeltaTime);


private:
	map<const _tchar*, CGameObject*>			m_mapPrototypes;
	typedef map<const _tchar*, CGameObject*>	PROTOTYPES;

private:
	map<const _tchar*, CLayer*>*				m_mapLayer = nullptr;
	typedef map<const _tchar*, CLayer*>		LAYERS;

	_uint		m_iMaxSceneNum = 0;


private:
	CGameObject*	Find_Prototype(const _tchar* tagPrototype);
	CLayer*	Find_Layer(_uint iSceneNum, const _tchar* tagLayer);

public:
	virtual void Free() override;
};

END