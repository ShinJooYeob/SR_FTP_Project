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

	virtual void Set_NowSceneNum(_uint eNowSceneNum) {	m_eNowSceneNum = eNowSceneNum; };
	void Set_Layer_Tag(const _tchar* _pLayer_Tag);

	const _tchar* Get_Layer_Tag();
	
	// ������Ʈ �����Ǻ� / ������Ʈ�� ������ �����ش�.
	void DIED() { m_isDied = true; }
	_bool Get_IsDied() { return m_isDied; }


	//�������� �浹���� ��� �����
	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime);

	class CComponent* Get_Component(const _tchar* tagComponent);

	HRESULT	Compute_CamDistance(class CTransform* pTransform);
	virtual _float Get_CamDistance() const { return m_fCamDistance; }

	class CComponent* Find_Components(const _tchar* tagComponent);
	virtual HRESULT ReInitialize(void* pArg = nullptr) { return S_OK; };

protected:
	map<const _tchar*, class CComponent*>	m_mapComponets;
	typedef map<const _tchar*, class CComponent*>	COMPONENTS;

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDevice = nullptr;
	_uint						m_eNowSceneNum = 0;
	_bool						m_bIsClone = false;
	const _tchar*				m_Layer_Tag = nullptr;;
	_float						m_fCamDistance;
	_bool						m_isDied = false;

	OUTPUT_OBJECTINFO*			m_LoadData = nullptr;
protected:
	HRESULT Add_Component(_uint iScenenNum, const _tchar* tagPrototype,const _tchar* tagComponent, CComponent** ppOut , void* pArg =nullptr);
	HRESULT Change_Component(_uint iScenenNum, const _tchar* tagPrototype, const _tchar* tagComponent, CComponent** ppOut, void* pArg = nullptr);


public:
	virtual CGameObject* Clone(void* pArg = nullptr)PURE;
	virtual void Free() override;
};

END