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
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_MAIN2(PS_IN In2)
{
	PS_OUT		Out2 = (PS_OUT)0;

	Out2.vColor = vector(0.f, 0.f, 0.f, 0.f);

	if (In2.vTexUV.x > g_percent)
		return Out2;

	Out2.vColor = tex2D(BaseSampler, In2.vTexUV);

	return Out2;
}

PS_OUT PS_MAIN3(PS_IN In3)
{
	PS_OUT		Out3 = (PS_OUT)0;

	Out3.vColor = tex2D(BaseSampler, In3.vTexUV);

	Out3.vColor.a *= g_percent;

	return Out3;
}

technique Default_Device
{
	pass
	{
		zenable = false;
		zwriteenable = true;
		
		alphatestenable = true;
		alpharef = 150;
		alphafunc = greater;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass
	{
		zenable = false;
		zwriteenable = true;

		alphatestenable = true;
		alpharef = 150;
		alphafunc = greater;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN2();
	}

	pass
	{
		alphatestenable = true;
		alpharef = 100;
		alphafunc = greater;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN2();
	}

	pass
	{
		zenable = false;
		zwriteenable = true;

		alphatestenable = true;
		alpharef = 100;
		alphafunc = greater;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN3();
	}

	pass
	{
		zenable = false;
		zwriteenable = true;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};