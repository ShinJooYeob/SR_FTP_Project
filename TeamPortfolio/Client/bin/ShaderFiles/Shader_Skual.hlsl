
float4x4	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector		g_vPlayerPosition;

// 버텍스 애니메이션용
float g_WaveHeight, g_Time, g_Speed, g_WaveFrequency;

texture		g_Texture;

sampler DefaultSampler = sampler_state
{
	texture = g_Texture;
};

// VS_SHADER
struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float3 vWorldPosition : TEXCOORD1;

};


// MVP 정점 변환 수행
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;
	vector			vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	
	Out.vWorldPosition = vPosition.xyz;

	vPosition = mul(vPosition, g_ViewMatrix);
	
	Out.vPosition = mul(vPosition, g_ProjMatrix);
	Out.vTexUV = In.vTexUV;

	return Out;
}

VS_OUT VS_UVANI(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	float cosTime = g_WaveHeight * cos(g_Time * g_Speed + In.vTexUV.x * g_WaveFrequency);
	float sinTime = g_WaveHeight * sin(g_Time * g_Speed + In.vTexUV.y * g_WaveFrequency*0.5f);

//	In.vPosition.x += sinTime;
	In.vPosition.y += cosTime;


	vector			vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

	vPosition = mul(vPosition, g_ViewMatrix);

	Out.vPosition = mul(vPosition, g_ProjMatrix);
	Out.vTexUV = In.vTexUV;

	return Out;
}


// PS_SHADER
struct PS_IN
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
	float3 vWorldPosition : TEXCOORD1;

};

struct PS_OUT
{
	vector	vColor : COLOR;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = tex2D(DefaultSampler, In.vTexUV);
	return Out;
}

PS_OUT PS_MAIN_COLOR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = tex2D(DefaultSampler, In.vTexUV);
	
	float colorRatio = sin(g_Time);
//	colorRatio = ceil(colorRatio * 5) / 5;

	if (colorRatio <= 0.4f)
		colorRatio = 0.4f;

	float redColorRatio = saturate(cos(g_Time));

	if (redColorRatio < 0.2f)
		redColorRatio = 0.2f;
	if (redColorRatio > 0.8f)
		redColorRatio = 0.8f;

	Out.vColor = Out.vColor * colorRatio * 1.1f;
//	Out.vColor.r = redColorRatio;

	Out.vColor.a = 1;

	return Out;
}



technique Default_Technique
{
	// 기본 텍스처 랜더링
	pass Default
	{
		/*cullmode = none;
		ALPHATESTENABLE = true;
		ALPHAREF = 130;
		ALPHAFUNC = GREATER;*/

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass UVAni
	{
		cullmode = none;
		VertexShader = compile vs_3_0 VS_UVANI();
		PixelShader = compile ps_3_0 PS_MAIN_COLOR();
	}

	//pass Pass2
	//{
	//	VertexShader = compile vs_3_0 VS_MAIN();
	//	PixelShader = compile ps_3_0 PS_MAIN();
	//}

}