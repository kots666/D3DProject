texture		g_NormalTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

vector		g_vLightDir;	// ���� �����̽� ������ �ִ� ����
vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse = (vector)1.f;
vector		g_vMtrlAmbient = (vector)1.f;

texture		g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vShade : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vNormal = tex2D(NormalSampler, In.vTexUV);
	// �븻 ���� �ؽ��� uv(0 ~ 1)���� ����

	// ���� �����̽� ������ ����ȭ�� �̷��� �븻 ���ͷ� ����
	// �ؽ���uv -> ���� �����̽� ������ ((0 -> -1) , (1 -> 1))
	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	// ����Ʈ�� Ȯ�� ��������� ���� ���� ���⸦ ����
	Out.vShade = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	return Out;
}

technique Default_Device
{
	// ����� ĸ��ȭ
	pass Direction
	{
		zwriteenable = false;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};