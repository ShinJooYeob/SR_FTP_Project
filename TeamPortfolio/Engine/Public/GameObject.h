#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CGameObject(const CGameObject&  rhs);
	virtual ~CGameObject() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg);
	virtual HRESULT Initialize_Clone(void* pArg);

	virtual _int Update(_float fDeltaTime);
	virtual _int LateUpdate(_float fDeltaTime);
	
	virtual _int Render();
	virtual _int LateRender();


	class CComponent* Find_Components(const _tchar* tagComponent);

protected:
	map<const _tchar*, class CComponent*>	m_mapComponets;
	typedef map<const _tchar*, class CComponent*>	COMPONENTS;

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDevice = nullptr;
	_bool						m_bIsClone = false;

protected:
	HRESULT Add_Component(_uint iScenenNum, const _tchar* tagPrototype,const _tchar* tagComponent, CComponent** ppOut , void* pArg =nullptr);
	

public:
	virtual CGameObject* Clone(void* pArg = nullptr)PURE;
	virtual void Free() override;
};

END