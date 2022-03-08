#pragma once
#include "MonsterParent.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CCollision;
class CVIBuffer;
class CRenderer;
END


BEGIN(Client)

// ÃÑ¾Ë 
class CBullet final: public CMonsterParent
{
protected:
	explicit CBullet(LPDIRECT3DDEVICE9	pGraphicDevice);
	explicit CBullet(const CBullet& rhs);
	virtual	~CBullet() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

protected:
	virtual HRESULT SetUp_RenderState() override;
	virtual HRESULT Release_RenderState() override;

	virtual HRESULT CreateObject(_int Damage);
	virtual HRESULT Hit(_int Damage);
	virtual HRESULT Die();


public:

	static CBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg);
	virtual CBullet * Clone(void * pArg = nullptr) override;

	virtual void Free()override;


};
END
