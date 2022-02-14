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

	void Set_NowSceneNum(_uint eNowSceneNum) {	m_eNowSceneNum = eNowSceneNum; };
	void Set_Layer_Tag(const _tchar* _pLayer_Tag);

	const _tchar* Get_Layer_Tag();


	//옵저버가 충돌했을 경우 실행됨
	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime);

	class CComponent* Get_Component(const _tchar* tagComponent);


	class CComponent* Find_Components(const _tchar* tagComponent);

protected:
	map<const _tchar*, class CComponent*>	m_mapComponets;
	typedef map<const _tchar*, class CComponent*>	COMPONENTS;

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDevice = nullptr;
	_uint						m_eNowSceneNum = 0;
	_bool						m_bIsClone = false;
	const _tchar*				m_Layer_Tag = nullptr;;

protected:
	HRESULT Add_Component(_uint iScenenNum, const _tchar* tagPrototype,const _tchar* tagComponent, CComponent** ppOut , void* pArg =nullptr);
	

public:
	virtual CGameObject* Clone(void* pArg = nullptr)PURE;
	virtual void Free() override;
};

END