#pragma once
#include "Component.h"
#include "..\public\Bullet.h"

BEGIN(Engine)

END


BEGIN(Client)

// 총알패턴을 만드는 총 컴포넌트
class CCom_Gun final: public CComponent
{
public:
	enum STATE_GUN
	{
		STATE_GUN_NONE,
		STATE_GUN_START,
		STATE_GUN_SHOOT,
		STATE_GUN_RETIRE,
		STATE_GUN_END
	};
public:
	const float FIX_BulletTime = 0.5f; // 애니메이션에서 쏘는 시간 맞춤
	const float FIX_BulletEndTime = 0.5f; // 애니메이션 종료시간 맞추기

public:
	typedef struct tagGunDesc	
	{
		_uint mSceneID;

	}GUNDESC;
		

private:
	explicit CCom_Gun(LPDIRECT3DDEVICE9	pGraphicDevice);
	explicit CCom_Gun(const CCom_Gun& rhs);
	virtual	~CCom_Gun() = default;

public:
	void Set_MonsterObject(class CBossMonster* monster);
public:
   // #TODO 총알 패턴설정

	// 타겟탄
	HRESULT CreateBullet_Target(_float3 startPos, _float3 targetPos, _float speed=1.f, E_BulletType_MOVE type = BULLETTYPE_MOVE_NOMAL);
	// 방향탄
	HRESULT CreateBullet_Dir(_float3 startPos, _float3 moveidr, _float speed = 1.f,E_BulletType_MOVE type = BULLETTYPE_MOVE_NOMAL);

	// 원형탄	
	HRESULT DestoryBullet_All();


public:
	HRESULT Initialize_Prototype(void * pArg);
	HRESULT Initialize_Clone(void * pArg);
	HRESULT Update_CreateBullet(float deltatime);

private:
	HRESULT Push_BackBulletList(CBullet::BULLETDESC b);
private:
	tagGunDesc mDesc;	
	class CBossMonster* mBossMonster = nullptr;

//	CBullet::BULLETDESC* mCurrentBulletData = nullptr;
	list<CBullet::BULLETDESC> mListBullets;
	float mCurrentTimer = FIX_BulletTime;

	STATE_GUN mStateGun = STATE_GUN_NONE;


public:
	static CCom_Gun* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;

};
END
