matrix			g_matWorld;		// ��� ���̺�
matrix			g_matView;
float4x4		g_matProj;

texture			g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
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

	return Out;
}


struct	PS_IN
{
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
};

struct	PS_OUT
{
	vector		vColor : COLOR0;
	vector		vNormal : COLOR1; // �븻�̶�� ����Ÿ��(�ؽ���)�� ����ϱ� ���� ����
	vector		vDepth : COLOR2;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� ������ �Լ�, ��ȯ Ÿ���� vector Ÿ��
	
	// (-1 ~ 1)���� ���� ������ ���� ���͸� ����ȭ�Ͽ��� ������ xyz���� ���� �� �ִ� ������ �ش�
	// (0 ~ 1) �ؽ��� uv��ǥ�� ��ȯ
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);
	//Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.z / In.vProjPos.w, In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f);


	return Out;
}

struct	VS_NORMAL_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct	VS_NORMAL_OUT
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	float3		N : TEXCOORD2;
	float3		T : TEXCOORD3;
	float3		B : TEXCOORD4;
};

// ���ؽ� ���̴�

VS_NORMAL_OUT VS_NORMALMAIN(VS_NORMAL_IN In)
{
	VS_NORMAL_OUT Out = (VS_NORMAL_OUT)0;

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

	return Out;
}


struct	PS_NORMAL_IN
{
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
};

struct	PS_NORMAL_OUT
{
	vector		vColor : COLOR0;
	vector		vNormal : COLOR1; // �븻�̶�� ����Ÿ��(�ؽ���)�� ����ϱ� ���� ����
	vector		vDepth : COLOR2;
};

PS_NORMAL_OUT PS_NORMALMAIN(PS_NORMAL_IN In)
{
	PS_NORMAL_OUT Out = (PS_NORMAL_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� ������ �Լ�, ��ȯ Ÿ���� vector Ÿ��

												// (-1 ~ 1)���� ���� ������ ���� ���͸� ����ȭ�Ͽ��� ������ xyz���� ���� �� �ִ� ������ �ش�
												// (0 ~ 1) �ؽ��� uv��ǥ�� ��ȯ
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);
	//Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.z / In.vProjPos.w, In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f);


	return Out;
}


technique Default_Device
{
	// ����� ĸ��ȭ
	pass
	{
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass
	{
		vertexshader = compile vs_3_0 VS_NORMALMAIN();
		pixelshader = compile ps_3_0 PS_NORMALMAIN();
	}
};