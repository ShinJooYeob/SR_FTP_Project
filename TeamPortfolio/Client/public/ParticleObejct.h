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

float GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;
	float f = (rand() % 10000) * 0.0001f;
	return (f * (highBound - lowBound)) + lowBound;
}

void GetRandomVector(
	D3DXVECTOR3* out,
	D3DXVECTOR3* min,
	D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

// 파티클 속성
typedef struct tag_ParticleAttribute
{
	tag_ParticleAttribute()
	{
		_lifeTime = 0.0f;
		_age = 0.0f;
		_isAlive = true;
	}

	// 위치와 벡터
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _velocity;
	D3DXVECTOR3 _acceleration;

	// 시간 크기 색
	float       _lifeTime;     // how long the particle lives for before dying  
	float       _age;          // current age of the particle  
	D3DXCOLOR   _color;        // current color of the particle   
	D3DXCOLOR   _colorFade;    // how the color fades with respect to time
	bool        _isAlive;
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

	bool isPointInside(D3DXVECTOR3& p)
	{
		if (p.x >= minPos.x && p.y >= minPos.y && p.z >= minPos.z &&
			p.x <= maxPos.x && p.y <= maxPos.y && p.z <= maxPos.z)
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
};

// 파티클 오브젝트 하나에 여러개의 오브젝트 정보를 들고 있어야한다.
class CParticleObject abstract : public CGameObject
{
protected:
	explicit CParticleObject(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CParticleObject(const CParticleObject&  rhs);
	virtual ~CParticleObject() = default;


public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;


	// 부모 파티클 데이터 재정의
	virtual HRESULT Initialize_Child_Clone()PURE;

	virtual _int Update(_float fTimeDelta)override;
	virtual _int LateUpdate(_float fTimeDelta)override;

	virtual _int Render()override;
	virtual _int LateRender()override;

	bool isEmpty() { return m_list_Particles.empty(); }

	_float3 Get_ParentPos();

protected:
//	virtual void RemoveDeadParticles();

private:
	HRESULT SetUp_Components();
	// 랜더스테이트 재정의 할 수 있게 
	virtual HRESULT SetUp_RenderState();
	virtual HRESULT Release_RenderState();

protected:
	CTransform*				m_ComTransform = nullptr;
	CRenderer*				m_ComRenderer = nullptr;
	CTexture*				m_ComTexture = nullptr;
	CVIBuffer_Rect*			m_ComVIBuffer = nullptr;


	float                   m_RateTime;   
	float                   m_Scale;       
	list<PARTICLEATT>		m_list_Particles;
	int                     m_MaxParticles; 
	BoundingBox				m_boundingBox;


//	DWORD _vbSize;      // size of vb
//	DWORD _vbOffset;    // offset in vb to lock   
//	DWORD _vbBatchSize; // number of vertices to lock starting at _vbOffset


public:
//	static CParticleObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg)PURE;
	virtual void Free() override;
};

class CParticleeObj_Base final: public CParticleObject
{
private:
	explicit CParticleeObj_Base(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParticleeObj_Base(const CParticleeObj_Base& rhs);
	virtual ~CParticleeObj_Base() = default;

private:

	virtual HRESULT Initialize_Child_Clone() override;
public:

	static CParticleeObj_Base* Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg = nullptr);
	virtual CGameObject * Clone(void * pArg) override;

};


class CParticleObj_Snow : public CParticleObject
{
public:

	CParticleObj_Snow(BoundingBox* boundingBox, int numParticles);
	void resetParticle(PARTICLEATT* attribute);
	void update(float timeDelta);
};

END