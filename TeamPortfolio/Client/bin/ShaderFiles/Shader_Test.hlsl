
float4x4	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

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
	/*���������� �޾ƿͼ� ���� ���� ����, ��, ��������� ���Ѵ�. */
	VS_OUT			Out = (VS_OUT)0;

	vector			vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);

	Out.vPosition = mul(vPosition, g_ProjMatrix);
	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix).xyz;

	/* ����. */		 
	return Out;
}

/* W������ X, Y, Z, W�� ��� ������ .*/
/* ����Ʈ �����̽��� ������ ��ġ�� ��ȭ�Ѵ�. */

/* ���������� ���̸�(TRIANGLELIST) �����Ͷ���� �����Ѵ�. */

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

//� ��ũ��Ŀ�� �׸���. 
//
//� ���̴��� �׸���. 
//
//� �н��� �׸���.(1)
//
//���۸� �������Ѵ�. 
//
//�н��� ������. 
// 
//���̴��׸��⸦ ������. 


technique Default_Technique
{	
	/* ��� + �׸��� + ���̶���Ʈ + ������Ʈ */
	/* �� ��ü�� �׸����۾��� ������. */
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	/* ���ĺ��� + �ְ�.  */
	/*pass 
	{

	}*/
}