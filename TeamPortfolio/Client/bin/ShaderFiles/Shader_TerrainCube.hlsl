
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
	/*���������� �޾ƿͼ� ���� ���� ����, ��, ��������� ���Ѵ�. */
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

/* W������ X, Y, Z, W�� ��� ������ .*/
/* ����Ʈ �����̽��� ������ ��ġ�� ��ȭ�Ѵ�. */

/* ���������� ���̸�(TRIANGLELIST) �����Ͷ���� �����Ѵ�. */

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
	// �÷��̾� ��ġ�� ���� ����
	PS_OUT		Out = (PS_OUT)0;

	float4	vColor = texCUBE(DefaultSampler, In.vTexUV);

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
	// �÷��̾� ��� ť���� view�� ��
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
	/* ��� + �׸��� + ���̶���Ʈ + ������Ʈ */
	/* �� ��ü�� �׸����۾��� ������. */
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