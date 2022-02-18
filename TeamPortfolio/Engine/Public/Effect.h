#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CEffect: public CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CEffect(const CEffect&  rhs);
	virtual ~CEffect() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int Update(_float fTimeDelta) override;
	virtual _int LateUpdate(_float fTimeDelta) override;
	virtual _int Render() override;
	virtual _int LateRender()override;
private:
protected:
	virtual void Free() override;
};

END