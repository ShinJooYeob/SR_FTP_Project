#pragma once
#include "Base.h"

BEGIN(Engine)
class CFrustumMgr :public CBase
{
	DECLARE_SINGLETON(CFrustumMgr);

private:
	explicit CFrustumMgr();
	virtual ~CFrustumMgr() = default;

public:
	HRESULT Initialize_FrustumMgr(LPDIRECT3DDEVICE9 pGraphicDevice);

public:
	//�� �Լ��� �� ������ �� �ѹ��� ���� ������ �� ������ ���� ����, �� ������״ϱ� 
	HRESULT SetUp_WorldFrustumPlane();
	//�� �Լ��� ���� ��ü���� �����ؼ� �׷������� ���θ� �Ǵ�
	_bool IsNeedToRender(_float3 vWorldPosition, _float fLenth = 1.4142135623f);

private:
	_float3 vDefaultProjectFrustumMgrPos[8];
	D3DXPLANE tWorldFrustumPlane[6];

private:
	LPDIRECT3DDEVICE9 m_pGraphicDevice = nullptr;

public:
	virtual void Free() override;

};
END

