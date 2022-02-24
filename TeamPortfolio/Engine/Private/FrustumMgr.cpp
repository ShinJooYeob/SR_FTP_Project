#include "..\Public\FrustumMgr.h"

IMPLEMENT_SINGLETON(CFrustumMgr);


CFrustumMgr::CFrustumMgr()
{

	vDefaultProjectFrustumMgrPos[0] = _float3(-1, 1, 0);
	vDefaultProjectFrustumMgrPos[1] = _float3(1, 1, 0);
	vDefaultProjectFrustumMgrPos[2] = _float3(1, -1, 0);
	vDefaultProjectFrustumMgrPos[3] = _float3(-1, -1, 0);

	vDefaultProjectFrustumMgrPos[4] = _float3(-1, 1, 1);
	vDefaultProjectFrustumMgrPos[5] = _float3(1, 1, 1);
	vDefaultProjectFrustumMgrPos[6] = _float3(1, -1, 1);
	vDefaultProjectFrustumMgrPos[7] = _float3(-1, -1, 1);

}

HRESULT CFrustumMgr::Initialize_FrustumMgr(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	if (pGraphicDevice == nullptr)
		return E_FAIL;

	m_pGraphicDevice = pGraphicDevice;
	Safe_AddRef(m_pGraphicDevice);

	return S_OK;
}

HRESULT CFrustumMgr::SetUp_WorldFrustumPlane()
{
	if (m_pGraphicDevice == nullptr)
		return E_FAIL;

	_Matrix matVeiw, matProject;
	m_pGraphicDevice->GetTransform(D3DTS_VIEW, &matVeiw);
	m_pGraphicDevice->GetTransform(D3DTS_PROJECTION, &matProject);


	//���� -> �� -> ����
	_float3 vWorldFrustumMgrPos[8] = {};

	memcpy(vWorldFrustumMgrPos, vDefaultProjectFrustumMgrPos, sizeof(_float3) * 8);

	for (_uint i = 0; i < 8; i++)
	{
		vWorldFrustumMgrPos[i] = vWorldFrustumMgrPos[i].PosVector_Matrix(matProject.InverseMatrix());
		vWorldFrustumMgrPos[i] = vWorldFrustumMgrPos[i].PosVector_Matrix(matVeiw.InverseMatrix());
	}
	//6��� ���ϱ�
	ZeroMemory(tWorldFrustumPlane, sizeof(D3DXPLANE) * 6);

	//��� ���� �� �� �� �� �� ��   => �տ� �ִ� �ֵ��� ���� �ɷ����°� ������ ���Ƽ� �����غ��� �̼�����ΰ� ���� ������ �ɷ����� ������

	//��
	D3DXPlaneFromPoints(&(tWorldFrustumPlane[0]), &(vWorldFrustumMgrPos[0]), &(vWorldFrustumMgrPos[3]), &(vWorldFrustumMgrPos[4]));
	//��
	D3DXPlaneFromPoints(&(tWorldFrustumPlane[1]), &(vWorldFrustumMgrPos[2]), &(vWorldFrustumMgrPos[1]), &(vWorldFrustumMgrPos[6]));

	//��
	D3DXPlaneFromPoints(&(tWorldFrustumPlane[2]), &(vWorldFrustumMgrPos[1]), &(vWorldFrustumMgrPos[0]), &(vWorldFrustumMgrPos[5]));
	//��
	D3DXPlaneFromPoints(&(tWorldFrustumPlane[3]), &(vWorldFrustumMgrPos[3]), &(vWorldFrustumMgrPos[2]), &(vWorldFrustumMgrPos[7]));

	//��
	D3DXPlaneFromPoints(&(tWorldFrustumPlane[4]), &(vWorldFrustumMgrPos[0]), &(vWorldFrustumMgrPos[1]), &(vWorldFrustumMgrPos[3]));
	//��
	D3DXPlaneFromPoints(&(tWorldFrustumPlane[5]), &(vWorldFrustumMgrPos[5]), &(vWorldFrustumMgrPos[4]), &(vWorldFrustumMgrPos[6]));



	return S_OK;
}

_bool CFrustumMgr::IsNeedToRender(_float3 vWorldPosition, _float fLenth)
{

	for (_uint i = 0; i < 6; i++)
	{
		if (tWorldFrustumPlane[i].a * vWorldPosition.x + tWorldFrustumPlane[i].b * vWorldPosition.y +
			tWorldFrustumPlane[i].c * vWorldPosition.z + tWorldFrustumPlane[i].d > fLenth)
			return false;
	}

	return true;
}


void CFrustumMgr::Free()
{
	Safe_Release(m_pGraphicDevice);
}
