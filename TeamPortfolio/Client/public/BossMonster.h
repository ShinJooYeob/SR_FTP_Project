#pragma once
#include "MonsterParent.h"
#include "Camera_Main.h"

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
	bool Get_IsTurn() const;
	CCamera_Main::CameraLookStateID	Get_CameraLookState() const;
	CCamera_Main* Get_MainCamera() { return mMainCamera; }

public:
	HRESULT CreateObjectBullet_Target();
	HRESULT CreateObjectBullet_Target_Dir(_float3 dir, _float speed =1.0f);

	_float3 Get_OffsetPos(_float3 offset);

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
	HRESULT Set_TestPattern1();
	HRESULT Set_TestPattern2();
//	HRESULT Set_TestPattern3();
	HRESULT Choose_NextPattern();

private:
	// 패턴 설정 함수 캡슐화
	void Set_MovePattern(_float2 screenPos, _float time, EasingTypeID id);
	void Set_AttackPattern(_float attackcount, E_BulletType bulletType, _float time, _float speed=1, _float3 moveidr = _float3(1,0,0));


private:
	float	mPatternTime;
	int		mNextPattern;


protected:
	CPlayer*		mPlayerTarget;
	CCamera_Main*	mMainCamera;

	// 총알 담당 컴포넌트
	// 총 컴포넌트를 참조해서 총알 패턴 구현
	class CCom_Gun*			mComGun;

	class IAction*			mCurrentPattern;
	queue<class IAction*>	mQueue_Partern;

	D3DVIEWPORT9			mViewPort;


	_int					mHp;
	_int					mMaxHp;

public:
	static CBossMonster* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CBossMonster* Clone(void* pArg) override;


	virtual void Free()override;
};
END
