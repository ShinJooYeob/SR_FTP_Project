#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CCollision;
class CVIBuffer;
class CRenderer;
class CShader;
END


BEGIN(Client)

// 몬스터 부모객체
class CMonsterParent abstract : public CGameObject
{

protected:
	explicit CMonsterParent(LPDIRECT3DDEVICE9	pGraphicDevice);
	explicit CMonsterParent(const CMonsterParent& rhs);
	virtual	~CMonsterParent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg) override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual _int Update(_float fDeltaTime)override;
	virtual _int LateUpdate(_float fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;
	
public: // For. ObjectFunc
	HRESULT SetPos(_float3 pos);

	_float3 GetPos() { return m_ComTransform->Get_MatrixState(CTransform::STATE_POS); }
	_float3 GetScale() { return m_ComTransform->Get_MatrixScale(); }
	HRESULT MoveDir(_float3 Dir, _float Timer){	m_ComTransform->MovetoDir(Dir, Timer);}
	_float3 GetScreenToWorld(_float2 screenPos);

	// 생성 피격 죽음 연출 개별 설정
	virtual HRESULT CreateObject(_int Damage)PURE;
	virtual HRESULT Hit(_int Damage)PURE;
	virtual HRESULT Die()PURE;

protected:
	// 컴포넌트 초기화
	// 몬스터들은 동일한 컴포넌트 사용.
	virtual	HRESULT SetUp_Components();
	virtual HRESULT SetUp_RenderState()PURE;
	virtual HRESULT Release_RenderState()PURE;
	
	// 카메라 위치 기준으로 위치 옮기기
	_float3 Update_CameraPosition(_float z=20);
	_float3 Update_CameraPosition(_float3 ObjectPosition, _float z = 20);


protected:
	// Components
	CTransform*	 m_ComTransform;
	CRenderer*	 m_ComRenderer;
	CVIBuffer*	 m_ComVIBuffer;
	CTexture*	 m_ComTexture;
	
	CCom_CollisionViewPort* m_Com_Viewport;

	// 셰이더 추가
	// CShader*	 m_ComShader;


	float mFrameCount;

public:
	virtual void Free()override;
};
END
