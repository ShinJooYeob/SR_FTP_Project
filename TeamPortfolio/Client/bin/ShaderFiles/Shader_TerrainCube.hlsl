
float4x4	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector		g_vPlayerPosition;

textureCUBE	g_TextureCUBE;

samplerCUBE DefaultSampler = sampler_state
{
	Texture = g_TextureCUBE;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : POSITION;
	float3 vTexUV : TEXCOORD0;
	float3 vViewPosition : TEXCOORD1;
	float3 vWorldPosition : TEXCOORD2;
	float3 vPlayerViewPosition: TEXCOORD3;

};


VS_OUT VS_MAIN(VS_IN In)
{
	/*로컬정점을 받아와서 내가 직접 월드, 뷰, 투영행렬을 곱한다. */
	VS_OUT			Out = (VS_OUT)0;

	vector			vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	Out.vWorldPosition = vPosition;
	Out.vViewPosition = mul(vPosition, g_ViewMatrix).xyz;

	Out.vPlayerViewPosition = mul(g_vPlayerPosition, g_ViewMatrix).xyz;

	vPosition = float4(Out.vViewPosition, 1);

	Out.vPosition = mul(vPosition, g_ProjMatrix);
	Out.vTexUV = In.vTexUV;

	return Out;
}

/* W값으로 X, Y, Z, W를 모두 나눈다 .*/
/* 뷰포트 스페이스로 정점의 위치를 변화한다. */

/* 정점세개가 모이면(TRIANGLELIST) 래스터라이즈를 수행한다. */

struct PS_IN
{
	float4 vPosition : POSITION;
	float3 vTexUV : TEXCOORD0;
	float3 vViewPosition : TEXCOORD1;
	float3 vWorldPosition : TEXCOORD2;
	float3 vPlayerViewPosition: TEXCOORD3;

};

struct PS_OUT
{
	vector	vColor : COLOR;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;



	Out.vColor = texCUBE(DefaultSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_MAIN_CUBE_VIEW(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float4 vColor = texCUBE(DefaultSampler, In.vTexUV);

	if (vColor.a < 0.1f)
		discard;

	float		fDistance = (In.vViewPosition.z);

	float		fDistanceRatio = fDistance/100 * 2;


	fDistanceRatio = ceil(fDistanceRatio*20)/20;

//	vector		vEffectColor = (vector)1.f;

	 if (fDistanceRatio >= 0.8f)
	 	fDistanceRatio = 0.8f;

	 if (fDistanceRatio <= 0.3f)
		 fDistanceRatio = 0.0f;


//	float4		vAmbiant = 0.1f;
	Out.vColor = vColor * (1 - fDistanceRatio);
	Out.vColor.a = 1;

	// Out.vColor = vColor + ambiant;
	// Out.vColor.a = 0;
	return Out;
}

PS_OUT PS_MAIN_CUBE_PLAYER(PS_IN In)
{
	// 플레이어 위치에 따라 수정
	PS_OUT		Out = (PS_OUT)0;

	float4	vColor = texCUBE(DefaultSampler, In.vTexUV);

	//if (vColor.a < 0.1f)
	//	discard;

	float	fDistance = distance((In.vWorldPosition),(g_vPlayerPosition));
	
	float	fDistanceRatio = fDistance * 0.1f;

	if (fDistanceRatio > 0.5f)
		fDistanceRatio = 0.7;
	
	Out.vColor = vColor * (1- fDistanceRatio);
	Out.vColor.a = 1;

	// Out.vColor = vColor + ambiant;
	// Out.vColor.a = 0;
	return Out;
}

PS_OUT PS_MAIN_CUBE_PLAYERVIEW(PS_IN In)
{
	// 플레이어 뷰와 큐브의 view와 비교
	PS_OUT		Out = (PS_OUT)0;

	float4	vColor = texCUBE(DefaultSampler, In.vTexUV);

//	float	fDistance = length(In.vViewPosition) - length(In.vPlayerViewPosition);
	float	fDistance = (In.vViewPosition).z - (In.vPlayerViewPosition).z;

	float	fDistanceRatio = abs(fDistance) * 0.1f;;

	//if (fDistanceRatio > 0.6f)
	//	fDistanceRatio = 0.8;

	Out.vColor = vColor * saturate(fDistanceRatio);
	Out.vColor.a = 0;

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

	pass Cube1
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_CUBE_VIEW();
	}

	pass Cube2
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_CUBE_PLAYER();
	}

	//pass Cube3
	//{
	//	VertexShader = compile vs_3_0 VS_MAIN();
	//	PixelShader = compile ps_3_0 PS_MAIN_CUBE_PLAYERVIEW();
	//}
	// pass PlayerPass
	// {
	// 	CullMode = None;
	// 
	// 	//AlphaBlendEnable = true;
	// 	//SrcBlend = SrcAlpha;
	// 	//DestBlend = InvSrcAlpha;
	// 
	// 	VertexShader = compile vs_3_0 VS_MAIN();
	// 	PixelShader = compile ps_3_0 PS_MAIN_PLAYER();
	// }
	// 
	// pass Terrain
	// {
	// 
	// 	VertexShader = compile vs_3_0 VS_MAIN();
	// 	PixelShader = compile ps_3_0 PS_MAIN_TERRAIN();
	// }
}