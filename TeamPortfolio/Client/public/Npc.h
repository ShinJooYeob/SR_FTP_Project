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
public:
	typedef struct tagNpcdesc
	{
		_float3 vPos;
		_float3 vDir;
		_tchar*	pNpcTextureName;
		_tchar* pText;
		E_CUBEID eCubeInfo;
	}NPCDESC;
protected:
explicit CNpc(LPDIRECT3DDEVICE9	pGraphicDevice);
explicit CNpc(const CNpc& rhs);
virtual	~CNpc() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;
	virtual HRESULT ReInitialize(void* pArg)override;

public:
	virtual void Free()override;
};
END
