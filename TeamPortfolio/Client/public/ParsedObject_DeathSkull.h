#pragma once

#include "GameObject.h"
#include "ParsedObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollision;
class CVIBuffer_Parsed;
END

BEGIN(Client)

class CParsedObject_DeathSkull final : public CParsedObject
{
public:
	enum E_SKUAL_PASS
	{
		SKUAL_PASS_DEFAULT,
		SKUAL_PASS_VERTEX,
		SKUAL_PASS_RED,
		SKUAL_PASS_END,
	};
private:
	explicit CParsedObject_DeathSkull(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParsedObject_DeathSkull(const CParsedObject_DeathSkull& rhs);
	virtual ~CParsedObject_DeathSkull() = default;


public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

	void	Set_RenderPass(E_SKUAL_PASS num) { m_pathNum = num; }
//	virtual _int Obsever_On_Trigger(CGameObject* pDestObjects, _float3 fCollision_Distance, _float fTimeDelta)override;
	void	Set_ShaderVal(float height, float speed, float waveSpeed)
	{
		mWaveHeight = height;
		mTime = 0;
		mSpeed = speed;
		mWaveFrequency = waveSpeed;
	}

	CTransform* Get_Transform() const { return m_ComTransform; }

private:
	HRESULT SetUp_Components();
//	HRESULT SetUp_RenderState();
//	HRESULT Release_RenderState();

private:
	CTransform*				m_ComTransform = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;
	CVIBuffer_Parsed*		m_ComVIBuffer = nullptr;

	CCollision*				m_pCollisionCom = nullptr;

	CShader*				m_ComShader = nullptr;

	E_SKUAL_PASS			m_pathNum;

	static	float			mStaticTime;
	float					mWaveHeight;
	float					mTime;
	float					mSpeed;
	float					mWaveFrequency;


public:
	static CParsedObject_DeathSkull* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END