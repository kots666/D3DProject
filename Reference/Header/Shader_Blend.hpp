texture			g_AlbedoTexture;

sampler AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;

	minfilter = linear;
	magfilter = linear;
};

texture g_DistortionTexture;

sampler DistortSampler = sampler_state
{
	texture = g_DistortionTexture;

	minfilter = linear;
	magfilter = linear;
};

texture			g_ShadeTexture;

sampler ShadeSampler = sampler_state
{
	texture = g_ShadeTexture;

	minfilter = linear;
	magfilter = linear;
};

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDistort = tex2D(DistortSampler, In.vTexUV);
	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV + (vDistort.r * 0.1f));
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);

	Out.vColor = vAlbedo * vShade;

	return Out;
}

technique Default_Device
{
	pass Blend
	{
		zwriteenable = false;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};