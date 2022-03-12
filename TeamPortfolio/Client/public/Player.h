#pragma once

#include "GameObject.h"

BEGIN(Engine)
//class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CInventory;
END

BEGIN(Client)

class CCamera_Main;

class CPlayer final :public CGameObject
{
private:
	typedef struct CoolDown
	{
		_bool m_bCoolDownStart=false;
	}COOL;
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fDeltaTime)override;
	virtual HRESULT ViewPortHit(CGameObject* hitobj)override;

	void Set_PlayerPause(_float TotalPauseTime, const _tchar* TagAnim, _float fFrameTime = 6.0f);
	virtual HRESULT ReInitialize(void* pArg)override;

	_int Get_PlayerLife() { return m_iPlayerLife; };
	void Set_PlayerLife(_int iNum) { m_iPlayerLife += iNum; };
	const _tchar* Get_NowTextureTag();
	_bool Get_CoolDownStart(_int eSKILL) { return m_tCoolDown[eSKILL].m_bCoolDownStart; }
	void Set_CoolDownStart_False(_int eSKILL) { m_tCoolDown[eSKILL].m_bCoolDownStart = false; }
	void Set_CoolDownStart_True(_int eSKILL) { m_tCoolDown[eSKILL].m_bCoolDownStart = true; }

	void	Set_BossStatusUI(class CUI_BossStatusUI* pObj) { m_BossStatusUI = pObj; }

	CTransform* Get_TransformCom() { return m_ComTransform; }

	HRESULT Set_StageEnd(_int IsKindsOfEnd);

	HRESULT Get_PlayerLoginDesc(LOGINDESC* pOutLoginData);
	HRESULT Set_PlayerLoginDesc(LOGINDESC pOutLoginData);

private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CInventory*				m_ComInventory = nullptr;


	/* √Êµπ√≥∏Æ */
	CCollision*		m_pCollisionCom = nullptr;
	CCom_CollisionViewPort* m_pCollisionViewCom = nullptr;

private:
	LOGINDESC				m_LoginDesc;

	PARTICLEDESC			m_ArrParticleDesc[10];

	COOL					m_tCoolDown[SKILL_END];
	_float					m_fFrame = 0.f;

	_int					m_iPlayerLife = 0;

	_float					m_fJumpPower = 5.f;
	_float					m_fNowJumpPower = 0.f;
	_uint					m_bIsJumped = 0;

	_uint					m_bIsStageEnd = 0;
	_bool					m_bIsDead = false;
	_float					m_bReHurtTime = 0;
	_float					m_fDeadNPauseTime = 0;
	_float					m_fTotalPauseTime = 0;
	_bool					m_bPause = false;
	const _tchar*			m_szReturnAnimTag = nullptr;

	_bool					m_bTextureReverse = false;
	_bool					m_bIsShdow = false;

	_bool					m_bIsCliming = false;
	_bool					m_bIsRunning = false;

	_float3					m_vDownstairsNear = NOT_EXIST_BLOCK;
	_float3					m_vClimingBlock = NOT_EXIST_BLOCK;
	_float3					m_vReturnStair = NOT_EXIST_BLOCK;

	_float3					m_vCameraPivot = _float3(3, 3, -14.f);

	_float					m_NowAnimFrameTimeCheckerForSound = 0;
	_float					m_OldAnimFrameTimeCheckerForSound = 0;

	CGameObject*			m_pCarryObject = nullptr;
	CTransform*				m_pCarryObjectTransform = nullptr;

	CCamera_Main*			m_pCamera_Main = nullptr;

	class CUI_BossStatusUI* m_BossStatusUI = nullptr; 

	////////¿∫«ı¿Ã∞° ∞«µÎ
public:
	void					SetBestClear(_uint _Stage, _float _timer);
	_float					getBestClear(_uint _Stage) { return StageBestClear[_Stage]; }

	void					Particle_NewDust();
	void					Particle_GameOver();
	void					Particle_Dust(CTransform* _pTransform);

	_float					StageBestClear[SCENE_END];
	_bool					m_bFixSoundSwitch = false;
	////////////////

private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_LoginDesc();
	HRESULT			SetUp_ParticleDesc();

	HRESULT			Input_Keyboard(_float fDeltaTime);
	HRESULT			Animation_Change(_float fDeltaTime);

	HRESULT			Find_FootHold_Object(_float fDeltaTime);
	HRESULT			Set_PosOnFootHoldObject(_float fDeltaTime);
	HRESULT			Set_CamPosXYZ(_float fDeltaTime);


	
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;

};

END	