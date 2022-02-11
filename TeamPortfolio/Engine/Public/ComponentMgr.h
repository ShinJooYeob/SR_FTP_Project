#pragma once

#include "Renderer.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"
#include "Transform.h"
#include "Texture.h"

BEGIN(Engine)
class CComponent;

class CComponentMgr final : public CBase
{
	DECLARE_SINGLETON(CComponentMgr);
private:
	explicit CComponentMgr();

public:
	HRESULT			Reserve_Container(_uint	eMaxSceneNum);
	HRESULT			Add_Component_Prototype(_uint eSceneIdx, const _tchar* tagComPrototype, CComponent* pComponenet);
	CComponent*		Clone_Component(_uint eSceneIdx, const _tchar* tagComPrototype, void* pArg);
	HRESULT			Clear_Scene_Componenets(_uint eSceneIdx);


private:
	map<const _tchar*, CComponent* >*		m_mapComPrototype = nullptr;
	typedef map<const _tchar*, CComponent*>			COMPROTOTYPES;
		
	_uint								m_iMaxScenenNum;

private:
	CComponent* Find_Component(_uint eScenenIdx, const _tchar* tagComPrototype);

public:
	virtual void Free()override;

};

END	