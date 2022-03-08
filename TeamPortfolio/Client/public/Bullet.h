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


// 총알 
class CBullet final: public CMonsterParent
{
public:
	typedef struct tag_BulletDesc
	{
		E_BulletType BulletType;
		_float3 StartPos;
		_float3 MoveDir;

	}BULLETDESC;

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


public:
	void SetMoveDir(_float3 dir) { mDesc.MoveDir = dir; }
	void SetBulletType(E_BulletType type) { mDesc.BulletType = type; }

protected:
	virtual HRESULT SetUp_RenderState() override;
	virtual HRESULT Release_RenderState() override;

	virtual HRESULT CreateObject(_int Damage);
	virtual HRESULT Hit(_int Damage);
	virtual HRESULT Die();


private:
	// 총알이 업데이트 되는 타입
	BULLETDESC mDesc;

public:

	static CBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg);
	virtual CBullet * Clone(void * pArg = nullptr) override;

	virtual void Free()override;


};
END
