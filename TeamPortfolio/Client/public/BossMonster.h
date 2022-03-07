#pragma once
#include "MonsterParent.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CCollision;
class CVIBuffer;
class CRenderer;
class CShader;
END


BEGIN(Client)

// 보스 몬스터 
class CBossMonster final : public CMonsterParent
{

protected:
	explicit CBossMonster(LPDIRECT3DDEVICE9	pGraphicDevice);
	explicit CBossMonster(const CBossMonster& rhs);
	virtual	~CBossMonster() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;
	
protected:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_RenderState()override;
	virtual HRESULT Release_RenderState()override;
	
protected:
	CGameObject* mPlayerTarget;

	// 패턴 컴포넌트
//	CStateMachine* m_StateMachine;

public:
	static CBossMonster* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CBossMonster* Clone(void* pArg) override;


	virtual void Free()override;
};
END
