matrix		g_matWorld;
matrix		g_matView;
matrix		g_matProj;

texture		g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;

};

float g_percent;

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
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	float sum = Out.vColor.r + Out.vColor.g + Out.vColor.b;

	if (1.f > sum)
	{
		Out.vColor.a = 0.f;
	}

	//Out.vColor.r = 1.f;
	//Out.vColor.g = 0.f;
	//Out.vColor.b = 0.f;

	return Out;
}

technique Default_Device
{
	pass
	{
		cullmode = none;
		//zenable = true;
		//zwriteenable = false;
		
		alphatestenable = true;
		alpharef = 10;
		alphafunc = greater;

		//alphablendenable = true;
		//srcblend = one;
		//destblend = one;
		//srcblendalpha = one;
		//destblendalpha = one;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};