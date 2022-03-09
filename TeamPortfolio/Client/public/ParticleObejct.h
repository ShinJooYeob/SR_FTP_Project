#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

//struct Particle
//{
//	D3DXVECTOR3 _position;
//	D3DCOLOR    _color;
//	static const DWORD FVF;
//};



// ��ƼŬ �Ӽ�
typedef struct tag_ParticleAttribute
{
	tag_ParticleAttribute()
	{
		_lifeTime = 0.0f;
		_age = 0.0f;
		_isAlive = true;
	}

	// ������ ��ġ / ���� / �� 
	_float3 _position;
	_float3 _velocity;
	_float	_force;

	// �ð� ũ�� ��
	float       _lifeTime;     // how long the particle lives for before dying  
	float       _age;          // current age of the particle  
	_float3		_color = _float3(255.f, 255.f, 255.f);
//	D3DXCOLOR   _color;        // current color of the particle   
//	D3DXCOLOR   _colorFade;    // how the color fades with respect to time
	bool        _isAlive;

	_float3		_NowparantPos ;
	class CParticleObject* SubParticle = nullptr;
}PARTICLEATT;



struct BoundingBox
{
	BoundingBox()
	{
		// infinite small 
		minPos.x = INFINITY;
		minPos.y = INFINITY;
		minPos.z = INFINITY;

		maxPos.x = -INFINITY;
		maxPos.y = -INFINITY;
		maxPos.z = -INFINITY;
	}

	void ResetBoudingBox(_float3 ParantPos) 
	{
		RelativeMinPos = ParantPos + minPos;
		RelativeMaxPos = ParantPos + maxPos;
	
	};

	bool isPointInside(D3DXVECTOR3& p)
	{
		if (p.x >= RelativeMinPos.x && p.y >= RelativeMinPos.y && p.z >= RelativeMinPos.z &&
			p.x <= RelativeMaxPos.x && p.y <= RelativeMaxPos.y && p.z <= RelativeMaxPos.z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	_float3 minPos;
	_float3 maxPos;

	_float3 RelativeMinPos;
	_float3 RelativeMaxPos;
};

// ��ƼŬ ������Ʈ �ϳ��� �������� ������Ʈ ������ ��� �־���Ѵ�.
class CParticleObject abstract : public CGameObject
{

public:
	// ���� �Լ�
	static float GetRandomFloat(float lowBound, float highBound)
	{
		if (lowBound >= highBound) // bad input
			return lowBound;
		float f = (rand() % 10000) * 0.0001f;
		return (f * (highBound - lowBound)) + lowBound;
	}

	static void GetRandomVector(
		_float3* out,
		_float3* min,
		_float3* max)
	{
		out->x = GetRandomFloat(min->x, max->x);
		out->y = GetRandomFloat(min->y, max->y);
		out->z = GetRandomFloat(min->z, max->z);
	}

protected:
	explicit CParticleObject(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CParticleObject(const CParticleObject&  rhs);
	virtual ~CParticleObject() = default;


public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;


	// �θ� ��ƼŬ ������ ������
	virtual HRESULT Initialize_Child_Clone()PURE;

	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	// ������ ����� ���ڸ�
	virtual void ResetParticle(PARTICLEATT* attribute);

	HRESULT Set_SubParticleDesc(PARTICLEDESC* pSubParticleDesc);

	virtual _float Get_CamDistance() const { return m_ParticleDesc.m_bUIDepth; }

	virtual void Reset_Velocity(_float3& fAttVlocity)PURE;
	virtual void Update_Position_by_Velocity(PARTICLEATT* tParticleAtt, _float fTimeDelta)PURE;
	virtual void Update_ColorChange(PARTICLEATT* tParticleAtt, _float fTimeDelta);

	HRESULT Reset_FixedPostion(_float3 vPos);

	virtual _int Render()override;
	virtual _int LateRender()override;

	bool isEmpty() { return m_list_Particles.empty(); }

	_float3 Get_ParentPos();

protected:
//	virtual void RemoveDeadParticles();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ParticleDesc(void* pArg);
	// ����������Ʈ ������ �� �� �ְ� 
	virtual HRESULT SetUp_RenderState();
	virtual HRESULT Release_RenderState();

protected:
	CTransform*				m_ComParticleTransform = nullptr;
	CTransform*				m_ComParentTransform = nullptr;

	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;


	PARTICLEDESC			m_ParticleDesc;

	PARTICLEDESC			m_SubParticleDesc;

	_float3					m_vUp = _float3(0,1,0);
	_float3					m_vRight = _float3(1, 0, 0);
	_float3					m_vLook = _float3(0, 0, 1);


	//_float3					m_vSubUp = _float3(0, 1, 0);
	//_float3					m_vSubRight = _float3(1, 0, 0);
	//_float3					m_vSubLook = _float3(0, 0, 1);

	//float                   m_RateTime;   
	//float                   m_Scale;       


	list<PARTICLEATT>		m_list_Particles;
	//int                     m_MaxParticles; 
	BoundingBox				m_boundingBox;

//	DWORD _vbSize;      // size of vb
//	DWORD _vbOffset;    // offset in vb to lock   
//	DWORD _vbBatchSize; // number of vertices to lock starting at _vbOffset

	_float					m_fEffectLifeTime = 0;


public:
//	static CParticleObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg)PURE;
	virtual void Free() override;
};




///////////�� ���� ��ƼŬ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CParticleeObj_Ball final: public CParticleObject
{
private:
	explicit CParticleeObj_Ball(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParticleeObj_Ball(const CParticleeObj_Ball& rhs);
	virtual ~CParticleeObj_Ball() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(PARTICLEATT* tParticleAtt, _float fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	// ������ �θ� �� ���


public:

	static CParticleeObj_Ball* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;

};


///////////���� ���� ��ƼŬ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CParticleeObj_Straight final : public CParticleObject
{
private:
	explicit CParticleeObj_Straight(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParticleeObj_Straight(const CParticleeObj_Straight& rhs);
	virtual ~CParticleeObj_Straight() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(PARTICLEATT* tParticleAtt, _float fTimeDelta)override;


	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	// ������ �θ� �� ���


public:

	static CParticleeObj_Straight* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;

};


///////////�м� ���� ��ƼŬ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CParticleeObj_Fountain final : public CParticleObject
{
private:
	explicit CParticleeObj_Fountain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParticleeObj_Fountain(const CParticleeObj_Fountain& rhs);
	virtual ~CParticleeObj_Fountain() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(PARTICLEATT* tParticleAtt, _float fTimeDelta)override;
	virtual void ResetParticle(PARTICLEATT * attribute)override;

	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	// ������ �θ� �� ���


public:

	static CParticleeObj_Fountain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;

};

///////////��ä ���� ��ƼŬ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CParticleeObj_Cone final : public CParticleObject
{
private:
	explicit CParticleeObj_Cone(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParticleeObj_Cone(const CParticleeObj_Cone& rhs);
	virtual ~CParticleeObj_Cone() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(PARTICLEATT* tParticleAtt, _float fTimeDelta)override;

	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	// ������ �θ� �� ���


public:

	static CParticleeObj_Cone* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;

};



class CParticleeObj_Spread final : public CParticleObject
{
private:
	explicit CParticleeObj_Spread(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParticleeObj_Spread(const CParticleeObj_Spread& rhs);
	virtual ~CParticleeObj_Spread() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(PARTICLEATT* tParticleAtt, _float fTimeDelta)override;
	virtual void ResetParticle(PARTICLEATT * attribute)override;

	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	// ������ �θ� �� ���


public:

	static CParticleeObj_Spread* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;

};


///////////���� ���� ��ƼŬ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class CParticleeObj_Fixed final : public CParticleObject
{
private:
	explicit CParticleeObj_Fixed(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParticleeObj_Fixed(const CParticleeObj_Fixed& rhs);
	virtual ~CParticleeObj_Fixed() = default;

private:

	virtual void Reset_Velocity(_float3& fAttVlocity)override;
	virtual void Update_Position_by_Velocity(PARTICLEATT* tParticleAtt, _float fTimeDelta)override;
	virtual void ResetParticle(PARTICLEATT * attribute)override;

	virtual HRESULT Initialize_Child_Clone() override;
	//	virtual void ResetParticle(PARTICLEATT* attribute);

	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;
	// ������ �θ� �� ���


public:

	static CParticleeObj_Fixed* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;

};
END