#pragma once

// 특수큐브의 위치와 정보를 저장해둔다.
typedef struct tag_SpecialCube_DATA
{
	tag_SpecialCube_DATA(wstring w, _Matrix worldmat)
	{
		Tagname = w;
		WorldMat = worldmat;
	}
	wstring Tagname;
	_Matrix WorldMat;
}SPECIALCUBE;



BEGIN(Client)

enum ParticleID
{
	Particle_Straight,
	Particle_Ball,
	Particle_Cone,
	Particle_Fountain,
	Particle_Spread,

	Particle_End
};

typedef struct tagParticleDesc
{
	ParticleID eParticleID;

	_float3 FixedTarget = _float3(9999.f, 9999.f, 9999.f);
	CTransform* FollowingTarget = nullptr;
	const _tchar* szTextureProtoTypeTag = nullptr;
	const _tchar* szTextureLayerTag = nullptr;

	_float3 ParticleSize;

	_float	TotalParticleTime;
	_float	EachParticleLifeTime;
	_uint	MaxParticleCount;


	_float Particle_Power;
	_float2 PowerRandomRange = _float2(0.5f, 1.5f);

	_float3 vUp = _float3(0, 1, 0);

	_float3 MaxBoundary =_float3(10,10,10);

	_bool	ParticleColorChage = false;
	_float3 TargetColor = _float3(255.f, 255.f, 255.f);


	_bool  m_bIsUI = false;
	//_bool	ParticleAlphaChage = false;
	//_float TargetAlpha = 255.f;

}PARTICLEDESC;

END