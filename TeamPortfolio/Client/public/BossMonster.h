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
	//enum PatternID
	//{
	//	PATTERN_MOVE, PATTERN_ATTACK, PATTERN_END

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
	HRESULT CreateObjectBullet_Target();

	virtual HRESULT ViewPortHit(CGameObject* hitobj)override;


	virtual HRESULT CreateObject(_int Damage);
	virtual HRESULT Hit(_int Damage);
	virtual HRESULT Die();

protected:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_RenderState()override;
	virtual HRESULT Release_RenderState()override;


private:
	void Update_BossPattern(_float deltatime);

	// #TODO 심화 몬스터 패턴 정의
	HRESULT Set_TestMovePattern1();
	HRESULT Set_TestAttackPattern1();
	HRESULT Choose_NextPattern();

private:
	float	mPatternTime;
	int		mNextPattern;


protected:
	CPlayer*	mPlayerTarget;

	// 총알 담당 컴포넌트
	// 총 컴포넌트를 참조해서 총알 패턴 구현
	class CCom_Gun*			mComGun;

	class IAction*			mCurrentPattern;
	queue<class IAction*>	mQueue_Partern;

	D3DVIEWPORT9			mViewPort;

	


public:
	static CBossMonster* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CBossMonster* Clone(void* pArg) override;


	virtual void Free()override;
};
END
