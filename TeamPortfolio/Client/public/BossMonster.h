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
	const float FIX_DEADANIMATIONTIMER = 2.f;
	const float	RIGHT_POS = 10.f;
	const float	LEFT_POS = -10.f;
	const float	TOP_POS = 5.f;
	const float	BOTTOM_PLAYER = -2.f;
	const float	BOTTOM_POS= -5.f;


	const _float2 NomalPos = _float2(RIGHT_POS, TOP_POS);
	const _float2 CenterPos = _float2(RIGHT_POS, 1.f);

	const _float2 UpFront = _float2(RIGHT_POS - 3, TOP_POS);
	const _float2 BottomFront = _float2(RIGHT_POS - 3, BOTTOM_PLAYER);

	const _float3 DefaultSpawnOffset = _float3(-0.3f, -0.2f, 0);


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
	const _tchar*	Get_NowTextureTag()const;				//주엽 => 보스 스테이터스 창에 나오는 보스 이미지 변경할때 쓰려고 만든거
	void	Set_BossStatusUI(class CUI_BossStatusUI* pObj) { m_BossStatusUI = pObj; }
	
	virtual void	Update_Die(float deltatime)override;


public:

	_float3 Get_OffsetPos(_float3 offset);
	_float3 GetLocalPos() const { return mCameralocalPosition; }

	void SetLocalPos(_float3 pos)
	{
		mCameralocalPosition = pos;
	}
	void	Start_AttackAniMaion(float frameSpeed=6.0f);

	virtual _float3 Update_CameraPosition(_float3 localPos);
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
	HRESULT Set_TestPattern_Create();
	HRESULT Set_BossPattern1();
	HRESULT Set_BossPattern2();
	HRESULT Set_BossPattern3();
	HRESULT Set_BossMoveDefault();

	HRESULT Choose_NextPattern();

private:
	// 패턴 설정 함수 캡슐화
	void Set_MovePattern(_float2 endPos, _float time, EasingTypeID id);
	void Set_Attack_WorldPattern(_float3 startPosOffset, _float3 endPos,_float count ,_float speed=1,_float dealytime=1,
		E_BulletType_MOVE type2 = BULLETTYPE_MOVE_NOMAL);

	void CBossMonster::Set_Attack_LocalDirPattern(_float3 startPosOffset, _float angle, _float count, _float speed, _float dealytime=1, 
		E_BulletType_MOVE type2 = BULLETTYPE_MOVE_NOMAL);

	void CBossMonster::Set_Attack_PlayerTargetPattern(_float3 startPosOffset, _float count, _float speed, _float dealytime = 1,
		E_BulletType_MOVE type2 = BULLETTYPE_MOVE_NOMAL);

	void CBossMonster::Set_Delay(_float Timer);

private: // For Effect
	PARTICLEDESC CreateMonster_Hit_ParticleDESC();
	PARTICLEDESC CreateMonster_Default_ParticleDESC();

	
private:
	float	mPatternTime;
	int		mNextPattern;
	float	mTimer;
	class   CUI_BossStatusUI* m_BossStatusUI = nullptr;


protected:
	CPlayer*		mPlayerTarget;
	CCamera_Main*	mMainCamera;
	_float3			mCameralocalPosition;

	// 총알 담당 컴포넌트
	// 총 컴포넌트를 참조해서 총알 패턴 구현
	class CCom_Gun*			mComGun;

	class IAction*			mCurrentPattern;
	queue<class IAction*>	mQueue_Partern;

	D3DVIEWPORT9			mViewPort;


	_int					mMaxHp;

	_float					mDefaultEffect;



public:
	static CBossMonster* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CBossMonster* Clone(void* pArg) override;
	virtual void Free()override;
};
END
