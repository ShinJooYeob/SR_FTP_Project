#pragma once

#include "UI.h"


BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CUI_Result final :public CUI
{
private:
	explicit CUI_Result(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CUI_Result(const CUI_Result& rhs);
	virtual ~CUI_Result() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fDeltaTime)override;

	virtual _int LateUpdate(_float fDeltaTime)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

private:
	HRESULT		SetUp_Components();
	HRESULT		SetUp_RenderState();
	HRESULT		Release_RenderState();
	HRESULT		SetUp_Pont(); //폰트와 관련된 함수
	HRESULT		SetUp_Player();	//플레이어에게 전달할 때 쓰이는 함수
public:
	void		Button_Picking();
	void		Set_Clear(_bool _bClear);
	void		Set_Clear_Wait_AnimTime(_bool bIsClear, _float MaxAnimTime);
	HRESULT		Ready_Layer_RankStar(const _tchar * pLayerTag);
	void		Set_RankStar();
	_int		Get_RankStar() {return RankNumber;}

	void		Set_PlusTimer(_float fPlusTime) {m_fMaxTime += fPlusTime; }
	void		Set_MaxTime(_float fMaxtime) { m_fMaxTime= fMaxtime; };
	_float		Get_MaxTime() { return m_fMaxTime; };
	_float		Get_NowTime() { return m_fTimer; };
	void		Set_MaxTimebyTimeUp(_float fTime) { m_fMaxTimebySkill += fTime; }
	_float		Get_MaxTimebyTimeUp() { return m_fMaxTimebySkill; }
	//진우형 프레임워크
private:
	HRESULT		Ready_Layer_Button(const _tchar * pLayerTag);//버튼 클론을 만들고 있음
	HRESULT		Update_UIButtonList(_float fTimeDelta); //이게 버튼의 업데이트를 돌려주고 있다.
	HRESULT		LateUpdate_UIButtonList(_float fTimeDelta); //이게 버튼의 레이트업데이트를 돌려주고 있다.
	CUI *		Find_UI(const _tchar * tagUI); //UI 겹치는게 있는지 체크
	CUI *		Find_Button(const _tchar * tagUIList); //이게 클론을 찾음

	void		Set_Particle_Firecracker2();
	void		Set_Particle_Firecracker();

	void		Set_Particle_Cry();

private:
	CTexture*				m_ComTexture = nullptr;
	CTransform*				m_ComTransform = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;
	CRenderer*				m_ComRenderer = nullptr;

private:
	map<const _tchar*, CUI*>			m_UIButtonList;
	map<const _tchar*, CUI*>			m_UIPrototypes;

	_float			m_fForPlayerAnimationMaxTime = 0;
	_float			m_fPlayerAnimFrame = 0;
	_bool			m_bAfterAnimIsClear = false;		//플레이어 애니메이션 이후 클리어로 만들건지 실패로 만들건지 여부를 판단할 변수


	_float			m_fFrame = 0.f;
	_float			m_fFrameTexture = 0.f;
	_float4			m_vUIDesc; //결과창용

	_float4			m_vButtonDesc; // 버튼용
	RECT			m_fStartButton;
	RECT			m_fCancelButton;

	_bool			m_isClicked = false;

	_float			m_fMaxTimebySkill = 0;
	_uint			RankNumber = 0;
	_bool			m_bStopSwitch = false; //타이며 멈춤 역할
	_float			m_fTimer = 0.f; // 타이머
	_float			m_fMaxTime = 0.f; // 최대 시간
	_bool			m_bClear = false; //클리어 여부
	_uint			TempMinutes = 0; //몇분인지 담는중
	_uint			TempSeconds = 0; //몇초인지 담는중

	_uint			RankStarGold = 0; //RankStar 하나당 골드량
	_uint			AcquisitionGold = 0;//획득골드량
	_float			TempBestClear = 0.f;	//최고 기록을 담고 있음

	_bool			m_bSetupGenerate = false; //1회에 한해서 생성함


	_float			TempY = 0;
	_float			StartPosY = 0;
	_float			TargetPosY = 0;
	_float			EasingTime = 0;

	_bool			EasingSwitch = false;


	_bool			SoundSwitch = false;

	////////////////////////
	_uint			m_iOldFrameTexture = 0;
	_bool			m_bFrameTexture = true;

	_bool			m_bParticle_Switch = false;

public:
	static CUI_Result* Create(LPDIRECT3DDEVICE9 pGraphicDevice, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	virtual void Free()override;
};

END