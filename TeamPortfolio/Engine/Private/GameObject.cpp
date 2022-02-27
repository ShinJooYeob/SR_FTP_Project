#include "..\Public\GameObject.h"
#include "GameInstance.h"



CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDevice)
	:m_pGraphicDevice(pGraphicDevice), m_bIsClone(false), m_isDied(false)
{
	m_pGraphicDevice->AddRef();
}

CGameObject::CGameObject(const CGameObject & rhs)
	:m_pGraphicDevice(rhs.m_pGraphicDevice), m_bIsClone(true), m_isDied(false)
{
	m_pGraphicDevice->AddRef();
}

HRESULT CGameObject::Initialize_Prototype(void * pArg)
{
	return S_OK;
}

HRESULT CGameObject::Initialize_Clone(void * pArg)
{
	m_eNowSceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	return S_OK;
}

_int CGameObject::Update(_float fDeltaTime)
{
	return _int();
}

_int CGameObject::LateUpdate(_float fDeltaTime)
{
	return _int();
}

_int CGameObject::Render()
{
	return _int();
}

_int CGameObject::LateRender()
{
	return _int();
}

void CGameObject::Set_Layer_Tag(const _tchar * _pLayer_Tag)
{
	m_Layer_Tag = _pLayer_Tag;
}

const _tchar * CGameObject::Get_Layer_Tag()
{
	return m_Layer_Tag;
}

_int CGameObject::Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)
{
	return _int();
}

CComponent* CGameObject::Get_Component(const _tchar * tagComponent)
{
	return Find_Components(tagComponent);
}

HRESULT CGameObject::Compute_CamDistance(CTransform * pTransform)
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;
	_Matrix ViewMatrix;
	m_pGraphicDevice->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	_float3 vCamPosition = *(_float3*)&ViewMatrix.m[3][0];
	
	_float3 vPosition = pTransform->Get_MatrixState(CTransform::STATE_POS);

	_float3 vDir = vPosition - vCamPosition;

	m_fCamDistance = D3DXVec3Length(&vDir);

	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iScenenNum, const _tchar* tagPrototype, const _tchar* tagComponent, CComponent** ppOut, void* pArg)
{
	if (Find_Components(tagComponent) != nullptr)
		return E_FAIL;

	 CComponent* pCloneComponent = GetSingle(CGameInstance)->Clone_Component(iScenenNum, tagPrototype, pArg);

	 if (pCloneComponent == nullptr)
		 return E_FAIL;

	 m_mapComponets.emplace(tagComponent, pCloneComponent);

	 (*ppOut) = pCloneComponent;
	 Safe_AddRef(pCloneComponent);

	return S_OK;
}

HRESULT CGameObject::Change_Component(_uint iScenenNum, const _tchar * tagPrototype, const _tchar * tagComponent, CComponent ** ppOut, void * pArg)
{
	// 같은 이름의 컴포넌트를 다른 자식의 컴포넌트로 교체
	// EX) Rect -> Cube
	if (Find_Components(tagComponent) != nullptr)
	{
		// 기존 컴포넌트 삭제
		Safe_Release(*ppOut);
		Safe_Release(m_mapComponets[tagComponent]);

		CComponent* pCloneComponent = GetSingle(CGameInstance)->Clone_Component(iScenenNum, tagPrototype, pArg);
		if (pCloneComponent == nullptr)
			return E_FAIL;

		// 키로 접근해 해당 컴포넌트 삭제 후에 다시 넣는다.
		m_mapComponets[tagComponent] = pCloneComponent;
		(*ppOut) = pCloneComponent;
		Safe_AddRef(pCloneComponent);
	}
	return S_OK;
}

CComponent * CGameObject::Find_Components(const _tchar * tagComponent)
{
	auto iter = find_if(m_mapComponets.begin(), m_mapComponets.end(), CTagFinder(tagComponent));

	if (iter == m_mapComponets.end())
		return nullptr;

	return iter->second;
}

//void CGameObject::Set_LoadData(OUTPUT_OBJECTINFO * pInfo)
//{
//	// OUTPUT_OBJECTINFO를 포인터 대입받음
//	m_pLoadInfoData = pInfo;
//}


void CGameObject::Free()
{
	for (auto& ComponentPair : m_mapComponets)
		Safe_Release(ComponentPair.second);
	m_mapComponets.clear();
	
	Safe_Release(m_pGraphicDevice);

}
