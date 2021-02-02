matrix			g_matWorld;		// ��� ���̺�
matrix			g_matView;
float4x4		g_matProj;

float g_detail;

texture			g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;

	AddressU = Wrap;
	AddressV = Wrap;
};

texture			g_NormalTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;

minfilter = linear;
magfilter = linear;
AddressU = wrap;
AddressV = wrap;
};

struct	VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct	VS_OUT
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	float3 N : TEXCOORD2;
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
};

// ���ؽ� ���̴�

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	float3 worldNormal = mul(In.vNormal, (float3x3)g_matWorld);
	Out.N = normalize(worldNormal);

	float3 Tangent = cross(float3(0.f, 1.f, 0.f), (float3)(In.vNormal));
	float3 worldTangent = mul(Tangent, (float3x3)g_matWorld);
	Out.T = normalize(worldTangent);

	float3 binormal = cross((float3)(In.vNormal), Tangent);
	float3 worldBinormal = mul(binormal, (float3x3)g_matWorld);
	Out.B = normalize(worldBinormal);

	return Out;
}


struct	PS_IN
{
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	float3 N : TEXCOORD2;
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
};

struct	PS_OUT
{
	vector		vColor : COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth : COLOR2;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV * g_detail);	// 2���� �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� ������ �Լ�, ��ȯ Ÿ���� vector Ÿ��

	Out.vColor = vColor;
	Out.vColor.a = 1.f;

	float4 texNormal = tex2D(NormalSampler, In.vTexUV);
	texNormal = (texNormal * 2.0f) - 1.0f;
	float3 normal = (texNormal.x * In.T) + (texNormal.y * In.B) + (texNormal.z * In.N);

	Out.vNormal = vector(normal.xyz * 0.5f + 0.5f, 1.f);

	// vectorŸ���� ��� ���� ��, Z���� W���� ������� ���� ����
	// vDepth��� �ڷ����� ��� �������� �����͸� �����ϴ� ����ü �뵵�� ��

	// x���� z�����Ⱑ ���� z���� ����(In.vProjPos.w �� �佺���̽� ������ z��)
	// y���� �佺���̽� ������ z���� �ؽ��� uv ������ ��ȯ���� ������ ����

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out;
}

technique Default_Device
{
	pass
	{
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};