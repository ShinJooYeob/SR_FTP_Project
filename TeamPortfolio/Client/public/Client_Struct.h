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
	Particle_Fixed,
	Particle_Suck,

	Particle_End
};

typedef struct tagParticleDesc
{
	ParticleID eParticleID;

	_float3 FixedTarget = _float3(9999.f, 9999.f, 9999.f);
	CTransform* FollowingTarget = nullptr;
	const _tchar* szTextureProtoTypeTag = nullptr;
	const _tchar* szTextureLayerTag = nullptr;
	_bool  m_bIsTextureAutoFrame = false;
	_float	fAutoFrameMul = 1.f;

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
	_float3 TargetColor2 = _float3(255.f, 255.f, 255.f);


	_bool  m_bIsUI = false;
	_float  m_bUIDepth = 0;
	//_bool	ParticleAlphaChage = false;
	//_float TargetAlpha = 255.f;

	_float3 ParticleStartRandomPosMin = _float3(-5.0f, -5.0f, -5.0f);
	_float3 ParticleStartRandomPosMax= _float3(5.f, 5.f, 5.f);

	_bool	MustDraw = false;
	_bool	IsParticleFameEndtoDie = false;


	_bool	AlphaBlendON = false;

	_bool	bSubPraticle = false;

	_float		m_fAlphaTestValue = 100.f;

}PARTICLEDESC;


typedef struct tagLoginDesc
{
	wstring szID = L"";
	wstring szPassword =L"";

	_int iArrStageBestClear[SCENE_END] = {};
	_int iArrQuestProgress[QUEST_END] = {};
	_int iArrSkillLevel[SKILL_END] = {};
	_int iGold = 0;

}LOGINDESC;


// 전역화함
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
static bool GetRandomBool()
{
	int random = (rand() % 10000) + 1;
	return 10000 * 0.5 < random;
}
END