
float4x4	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture		g_Texture;

sampler DefaultSampler = sampler_state
{
	texture = g_Texture;

/*	MagFilter = linear;
	MipFilter = linear;
	MinFilter = linear;

	AddressU = wrap;
	AddressV = wrap;*/	
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
};


// MVP ���� ��ȯ ����
VS_OUT VS_MAIN(VS_IN In)
{
	// ���� ���� ī�޶� �������� ȸ�� ����

	VS_OUT			Out = (VS_OUT)0;
	vector			vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	// vector			Camdir = normalize(vPosition - g_CameraWorldMatrix[3]);
	// Camdir[3] = vPosition[3];

	
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

technique Default_Technique
{	
	// �⺻ �ؽ�ó ������
	pass Default
	{
		cullmode = none;
		ALPHATESTENABLE = true;
		ALPHAREF = 130;
		ALPHAFUNC = GREATER;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	//// �ǰ� ������
	//pass Red
	//{
	//	VertexShader = compile vs_3_0 VS_MAIN();
	//	PixelShader = compile ps_3_0 PS_MAIN();
	//}

	//// ���� ������
	//pass Sun
	//{
	//	VertexShader = compile vs_3_0 VS_MAIN();
	//	PixelShader = compile ps_3_0 PS_MAIN();
	//}

}