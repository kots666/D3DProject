matrix		g_matWorld;
matrix		g_matView;
matrix		g_matProj;

float g_Offset;

texture		g_NoiseTexture;

sampler NoiseSampler = sampler_state
{
	texture = g_NoiseTexture;

	minfilter = linear;
	magfilter = linear;

};

texture		g_MaskTexture;

sampler MaskSampler = sampler_state
{
	texture = g_MaskTexture;

	minfilter = linear;
	magfilter = linear;
};

struct	VS_IN
{
	vector		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
};

struct	VS_OUT
{
	vector		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
};

VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float3 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector vColor : COLOR0;
};

PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float3 texUV = In.vTexUV;
	texUV.x = (texUV.x * 0.1f) + g_Offset;

	vector noiseColor = tex2D(NoiseSampler, texUV);
	vector maskColor = tex2D(MaskSampler, In.vTexUV);

	Out.vColor = noiseColor * maskColor;

	return Out;
}

technique Default_Device
{
	pass
	{
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

};