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
public:
	//enum MonsterFSM
	//{
	//	BOSS_FSM_INIT,
	//	BOSS_FSM_IDLE,
	//	BOSS_FSM_PATERN1,
	//	BOSS_FSM_PATERN2,
	//	BOSS_FSM_PATERN3,
	//	BOSS_FSM_HIT,
	//	BOSS_FSM_DEAD,
	//	
	//};


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
	
public:
//	MonsterFSM GetCurrentState() { return mCurrentState; }

protected:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_RenderState()override;
	virtual HRESULT Release_RenderState()override;
	
	virtual HRESULT CreateObject(_int Damage);
	virtual HRESULT Hit(_int Damage);
	virtual HRESULT Die();

private:
	void Update_BossPattern(_float deltatime);

	// 행동 정의
	HRESULT Set_TestPattern();

protected:
	CGameObject*	mPlayerTarget;

	// 총알 담당 컴포넌트
	// 총 컴포넌트를 참조해서 총알 패턴 구현
	class CCom_Gun*			mComGun;

	// 패턴 / 총알 잘 나오는지 보고
	class IAction* mCurrentPattern;
	queue<class IAction*> mQueue_Partern;
	float mPatternTime;

//	void FSMUpdate();


public:
	static CBossMonster* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CBossMonster* Clone(void* pArg) override;


	virtual void Free()override;
};
END
