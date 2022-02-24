#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)
class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking)
private:
	explicit CPicking();
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd, void * pArg=nullptr);
	 _bool	is_On_Rect(const RECT* TargetRect);
	 HRESULT Transform_ToWorldSpace();
	 HRESULT Transform_ToWorldSpace(POINT mousePos);
	 HRESULT Transform_ToLocalSpace(_Matrix WorldMatrixinverse);
	 _bool isPick(_float3 * pLocalPoint, _float3 * pOut);
	 _bool isPick_UV(_float3 * pLocalPoint, _float3 * pOut, _float2 * pOutUV);

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	HWND					m_hWnd;
public:
	_float3					m_vRayDir;
	_float3					m_vRayPos;

	_float3					m_vLocalRayDir;
	_float3					m_vLocalRayPos;
	virtual void Free() override;


};

END