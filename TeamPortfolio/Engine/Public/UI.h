#pragma once

#include "GameObject.h"
BEGIN(Engine)

class ENGINE_DLL CUI : public CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI(const CUI& rhs);
	virtual ~CUI() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int Update(_float fTimeDelta) override;
	virtual _int LateUpdate(_float fTimeDelta) override;
	virtual _int Render() override;
	virtual _int LateRender()override;
	virtual _float Get_CamDistance()  const override { return m_fDepth; }


	//UIDescø° fX,fY,SizeX,SizeY ∫§≈Õ ≥÷¿∏∏Èµ 
	HRESULT Set_UI_Transform(class CTransform* pComTransform,_float4 vUIDesc);


protected:
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float				m_fDepth=0;
	RECT				m_rcRect;
public:
	virtual void Free() override;
};

END