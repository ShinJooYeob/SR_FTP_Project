
float4x4	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector		g_vCamPosition;

textureCUBE	g_TextureCUBE;

//sampler DefaultSampler = sampler_state
//{
//	texture = g_Texture;
//
//	ALPHATESTENABLE = true;
//	ALPHAREF = 0;
//	ALPHAFUNC = GREATER;
//
//
//	MagFilter = linear;
//	MipFilter = linear;
//	MinFilter = linear;
//	
//	AddressU = wrap;
//	AddressV = wrap;
//};

//samplerCUBE EnvironmentSampler = sampler_state
//{
//	Texture = (EnvironmentMap_Tex);
//	MINFILTER = LINEAR;
//	MAGFILTER = LINEAR;
//};

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
};


VS_OUT VS_MAIN(VS_IN In)
{
	/*���������� �޾ƿͼ� ���� ���� ����, ��, ��������� ���Ѵ�. */
	VS_OUT			Out = (VS_OUT)0;

	vector			vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	Out.vViewPosition = mul(vPosition, g_ViewMatrix).xyz;
	vPosition = float4(Out.vViewPosition, 1);

	Out.vPosition = mul(vPosition, g_ProjMatrix);
	Out.vTexUV = In.vTexUV;

//	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix).xyz;
	
	return Out;
}

/* W������ X, Y, Z, W�� ��� ������ .*/
/* ����Ʈ �����̽��� ������ ��ġ�� ��ȭ�Ѵ�. */

/* ���������� ���̸�(TRIANGLELIST) �����Ͷ���� �����Ѵ�. */

struct PS_IN
{
	float4 vPosition : POSITION;
	float3 vTexUV : TEXCOORD0;
	float3 vViewPosition : TEXCOORD1;
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


//PS_OUT PS_MAIN_PLAYER(PS_IN In)
//{
//	PS_OUT		Out = (PS_OUT)0;
//
//	vector		vColor = tex2D(DefaultSampler, In.vTexUV);
//
//
//	if (vColor.a < 0.3f)
//		discard;
//
//	vector		vHitEffect = (vector)1.f;
//
//	//	vColor += vHitEffect * g_fHit;
//
//
//	vector		vDir = vector(In.vWorldPos, 1.f) - g_vCamPosition;
//
//	float		fDistance = length(vDir);
//
//	float		fDepthRatio = (fDistance / 300.f) * 15.f;
//
//	vector		vEffectColor = (vector)1.f;
//
//	Out.vColor = vColor + saturate(fDepthRatio) * vEffectColor;
//
//
//
//
//	return Out;
//}

PS_OUT PS_MAIN_CUBE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float4 vColor = texCUBE(DefaultSampler, In.vTexUV);

	if (vColor.a < 0.1f)
		discard;

//	vector		vDir = vector(In.vWorldPos, 1.f) - g_vCamPosition;

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



technique Default_Technique
{
	/* ��� + �׸��� + ���̶���Ʈ + ������Ʈ */
	/* �� ��ü�� �׸����۾��� ������. */
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Cube
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_CUBE();
	}

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