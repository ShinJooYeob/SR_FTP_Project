#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CCollision;
class CVIBuffer;
class CRenderer;
END


BEGIN(Client)

class CNpc : public CGameObject
{
private:
explicit CNpc(LPDIRECT3DDEVICE9	pGraphicDevice);
explicit CNpc(const CNpc& rhs);
virtual	~CNpc() = default;

private:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
public:

private:

public:
};
END
