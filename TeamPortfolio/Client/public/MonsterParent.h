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
	
protected:
	// 컴포넌트 초기화
	// 몬스터들은 동일한 컴포넌트 사용.
	virtual	HRESULT SetUp_Components();

protected:
	// Components
	CTransform*	 m_ComTransform;
	CRenderer*	 m_ComRenderer;
	CVIBuffer*	 m_ComVIBuffer;
	
	CTexture*	 m_ComTexture;
	CCollision*  m_ComCollision;
	// 셰이더 추가
	CShader*	 m_ComShader;


//	float mHp;
//	float mMp;



public:
	virtual void Free()override;
};
END
