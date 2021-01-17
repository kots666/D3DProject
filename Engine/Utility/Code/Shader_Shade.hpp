texture		g_NormalTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

vector		g_vLightDir;	// 월드 스페이스 공간에 있는 방향
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
	// 노말 값이 텍스쳐 uv(0 ~ 1)값인 상태

	// 월드 스페이스 공간의 정규화가 이뤄진 노말 벡터로 변경
	// 텍스쳐uv -> 월드 스페이스 값으로 ((0 -> -1) , (1 -> 1))
	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	// 램버트의 확산 조명공식을 위한 빛의 세기를 구함
	Out.vShade = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	return Out;
}

technique Default_Device
{
	// 기능의 캡슐화
	pass Direction
	{
		zwriteenable = false;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};