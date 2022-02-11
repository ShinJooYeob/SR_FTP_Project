#pragma once

#include "Base.h"


BEGIN(Engine)

class CGameObject;
class CComponent;

class CLayer :public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);

	_int Update(_float fDeltaTime);
	_int LateUpdate(_float fDeltaTime);

	CComponent* Get_Commponent_By_LayerIndex(const _tchar* tagComponet, _uint iLayerIndex);
	CGameObject* Get_GameObject_By_LayerIndex(_uint iLayerIndex);

private:
	list< CGameObject*>			m_ObjectList;
	typedef list< CGameObject*>	OBJECTS;

public:
	static CLayer*	Create();
	virtual void Free() override;
};

END