#include "..\Public\Picking.h"

IMPLEMENT_SINGLETON(CPicking)
CPicking::CPicking()
{
}

HRESULT CPicking::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd, void * pArg)
{
	m_hWnd = hWnd;
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(m_pGraphic_Device);
	return S_OK;
}

_bool CPicking::is_On_Rect(const RECT* TargetRect)
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);
	
	return PtInRect(TargetRect, ptMouse);
}

HRESULT	CPicking::Transform_ToWorldSpace()
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ClientToScreen(m_hWnd, &ptMouse);

	D3DVIEWPORT9	ViewPortDesc;
	m_pGraphic_Device->GetViewport(&ViewPortDesc);

	_float4 vMousePos;
	vMousePos.x = ptMouse.x / (ViewPortDesc.Width*0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPortDesc.Height*0.5f) + 1.f;
	vMousePos.z = 0.f;
	vMousePos.w = 1.f;

	_Matrix ProjMatrixInverse;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrixInverse);
	D3DXMatrixInverse(&ProjMatrixInverse, nullptr, &ProjMatrixInverse);
	D3DXVec4Transform(&vMousePos, &vMousePos, &ProjMatrixInverse);
	memcpy(&m_vRayDir, &(vMousePos - _float4(0.f, 0.f, 0.f, 1.f)), sizeof(_float3));
	m_vRayPos = _float3(0.f, 0.f, 0.f);

	_Matrix ViewMatrixInverse;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrixInverse);
	D3DXMatrixInverse(&ViewMatrixInverse, nullptr, &ViewMatrixInverse);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &ViewMatrixInverse);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &ViewMatrixInverse);

	return S_OK;
}

HRESULT CPicking::Transform_ToWorldSpace(POINT mousePos)
{

	D3DVIEWPORT9	ViewPortDesc;
	m_pGraphic_Device->GetViewport(&ViewPortDesc);

	_float4 vMousePos;
	vMousePos.x = mousePos.x / (ViewPortDesc.Width*0.5f) - 1.f;
	vMousePos.y = mousePos.y / -(ViewPortDesc.Height*0.5f) + 1.f;
	vMousePos.z = 0.f;
	vMousePos.w = 1.f;

	_Matrix ProjMatrixInverse;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrixInverse);
	D3DXMatrixInverse(&ProjMatrixInverse, nullptr, &ProjMatrixInverse);
	D3DXVec4Transform(&vMousePos, &vMousePos, &ProjMatrixInverse);
	memcpy(&m_vRayDir, &(vMousePos - _float4(0.f, 0.f, 0.f, 1.f)), sizeof(_float3));
	m_vRayPos = _float3(0.f, 0.f, 0.f);

	_Matrix ViewMatrixInverse;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrixInverse);
	D3DXMatrixInverse(&ViewMatrixInverse, nullptr, &ViewMatrixInverse);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &ViewMatrixInverse);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &ViewMatrixInverse);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	return S_OK;
}

HRESULT CPicking::Transform_ToLocalSpace(_Matrix WorldMatrixInverse)
{
	D3DXVec3TransformNormal(&m_vLocalRayDir, &m_vRayDir, &WorldMatrixInverse);
	D3DXVec3Normalize(&m_vLocalRayDir, &m_vLocalRayDir);
	D3DXVec3TransformCoord(&m_vLocalRayPos, &m_vRayPos, &WorldMatrixInverse);

	return S_OK;
}

_bool CPicking::isPick(_float3* pLocalPoint, _float3 *pOut)
{
	_float fU, fV, fDist;
	if (D3DXIntersectTri(&pLocalPoint[0], &pLocalPoint[1], &pLocalPoint[2], &m_vLocalRayPos, &m_vLocalRayDir, &fU, &fV, &fDist))
	{
		*pOut = m_vLocalRayPos + m_vLocalRayDir*fDist;
		return true;
	}
	else
	{
		return false;
	}
}

_bool CPicking::isPick_UV(_float3 * pLocalPoint, _float3 * pOut, _float2 * pOutUV)
{
	_float fDist;
	if (D3DXIntersectTri(&pLocalPoint[0], &pLocalPoint[1], &pLocalPoint[2], &m_vLocalRayPos, &m_vLocalRayDir, &pOutUV->x, &pOutUV->y, &fDist))
	{
		*pOut = m_vLocalRayPos + m_vLocalRayDir * fDist;
		return true;
	}
	else
	{
		return false;
	}
}


void CPicking::Free()
{
	Safe_Release(m_pGraphic_Device);
}
