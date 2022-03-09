
float4x4	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector		g_vCamPosition;
texture		g_Texture;

sampler DefaultSampler = sampler_state
{
	texture = g_Texture;

	MagFilter = linear;
	MipFilter = linear;
	MinFilter = linear;

	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float3 vWorldPos : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	/*로컬정점을 받아와서 내가 직접 월드, 뷰, 투영행렬을 곱한다. */
	VS_OUT			Out = (VS_OUT)0;

	vector			vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);

	Out.vPosition = mul(vPosition, g_ProjMatrix);
	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix).xyz;

	/* 리턴. */
	return Out;
}

/* W값으로 X, Y, Z, W를 모두 나눈다 .*/
/* 뷰포트 스페이스로 정점의 위치를 변화한다. */

/* 정점세개가 모이면(TRIANGLELIST) 래스터라이즈를 수행한다. */

struct PS_IN
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float3 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	vector	vColor : COLOR;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(DefaultSampler, In.vTexUV);

	Out.vColor.gb = Out.vColor.r;

	/*Out.vColor = vector(In.vTexUV.y, 0.f, 0.f, 1.f);*/

	return Out;
}


PS_OUT PS_MAIN_PLAYER(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vColor = tex2D(DefaultSampler, In.vTexUV);


	if (vColor.a < 0.3f)
		discard;

	vector		vHitEffect = (vector)1.f;

	//	vColor += vHitEffect * g_fHit;


	vector		vDir = vector(In.vWorldPos, 1.f) - g_vCamPosition;

	float		fDistance = length(vDir);

	float		fDepthRatio = (fDistance / 300.f) * 15.f;

	vector		vEffectColor = (vector)1.f;

	Out.vColor = vColor + saturate(fDepthRatio) * vEffectColor;




	return Out;
}

PS_OUT PS_MAIN_TERRAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vColor = tex2D(DefaultSampler, In.vTexUV);

	vector		vDir = vector(In.vWorldPos, 1.f) - g_vCamPosition;

	float		fDistance = length(vDir);

	float		fDepthRatio = (fDistance / 300.f) * 15.f;

	vector		vEffectColor = (vector)1.f;

	Out.vColor = vColor + saturate(fDepthRatio) * vEffectColor;

	return Out;
}



technique Default_Technique
{
	/* 명암 + 그림자 + 하이라이트 + 림라이트 */
	/* 그 객체의 그리기작업이 끝난다. */
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass PlayerPass
	{
		CullMode = None;

		//AlphaBlendEnable = true;
		//SrcBlend = SrcAlpha;
		//DestBlend = InvSrcAlpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_PLAYER();
	}

	pass Terrain
	{

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_TERRAIN();
	}
}