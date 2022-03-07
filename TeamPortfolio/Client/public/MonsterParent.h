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

// ���� �θ�ü
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
	// ������Ʈ �ʱ�ȭ
	// ���͵��� ������ ������Ʈ ���.
	virtual	HRESULT SetUp_Components();
	virtual HRESULT SetUp_RenderState()PURE;
	virtual HRESULT Release_RenderState()PURE;

protected:
	// Components
	CTransform*	 m_ComTransform;
	CRenderer*	 m_ComRenderer;
	CVIBuffer*	 m_ComVIBuffer;
	
	

	CTexture*	 m_ComTexture;
	CCollision*  m_ComCollision;
	// ���̴� �߰�
	CShader*	 m_ComShader;

	_float		m_DeltaTime;

	_int		mHp;
	_int		mMaxHp;


public:
	virtual void Free()override;
};
END
