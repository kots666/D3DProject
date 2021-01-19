matrix			g_matWorld;		// 상수 테이블
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

// 버텍스 쉐이더

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
	vector		vNormal : COLOR1; // 노말이라는 렌더타겟(텍스쳐)에 출력하기 위한 색상
	vector		vDepth : COLOR2;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 얻어오는 함수, 반환 타입이 vector 타입
	
	// (-1 ~ 1)값은 월드 상태의 법선 벡터를 정규화하였기 때문에 xyz값이 나올 수 있는 범위에 해당
	// (0 ~ 1) 텍스쳐 uv좌표로 변환
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

// 버텍스 쉐이더

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
	float3 worldTangent = mul(Tangent, (float3x3)g_matWorld);
	Out.T = normalize(worldTangent);

	float3 binormal = cross((float3)(In.vNormal), Tangent);
	float3 worldBinormal = mul(binormal, (float3x3)g_matWorld);
	Out.B = normalize(worldBinormal);

	return Out;
}


struct	PS_NORMAL_IN
{
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	float3		N : TEXCOORD2;
	float3		T : TEXCOORD3;
	float3		B : TEXCOORD4;
};

struct	PS_NORMAL_OUT
{
	vector		vColor : COLOR0;
	vector		vNormal : COLOR1; // 노말이라는 렌더타겟(텍스쳐)에 출력하기 위한 색상
	vector		vDepth : COLOR2;
};

PS_NORMAL_OUT PS_NORMALMAIN(PS_NORMAL_IN In)
{
	PS_NORMAL_OUT Out = (PS_NORMAL_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 얻어오는 함수, 반환 타입이 vector 타입

												// (-1 ~ 1)값은 월드 상태의 법선 벡터를 정규화하였기 때문에 xyz값이 나올 수 있는 범위에 해당
												// (0 ~ 1) 텍스쳐 uv좌표로 변환
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	float4 texNormal = tex2D(NormalSampler, In.vTexUV);
	texNormal = (texNormal * 2.0f) - 1.0f;
	float3 normal = (texNormal.x * In.T) + (texNormal.y * In.B) + (texNormal.z * In.N);

	Out.vNormal = vector(normal.xyz * 0.5f + 0.5f, 1.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);
	//Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.z / In.vProjPos.w, In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f);

	return Out;
}

PS_NORMAL_OUT PS_NORMALMAIN2(PS_NORMAL_IN In)
{
	PS_NORMAL_OUT Out2 = (PS_NORMAL_OUT)0;

	Out2.vColor = tex2D(BaseSampler, In.vTexUV);

	float4 texNormal = tex2D(NormalSampler, In.vTexUV);
	
	float3 unpackedNormal;
	unpackedNormal.xy = texNormal.wy * 2 - 1;
	unpackedNormal.z = sqrt(1 - saturate(dot(unpackedNormal.xy, unpackedNormal.xy)));

	float3 normal = (unpackedNormal.x * In.T) + (unpackedNormal.y * In.B) + (unpackedNormal.z * In.N);

	Out2.vNormal = vector(normal.xyz * 0.5f + 0.5f, 1.f);

	Out2.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f);

	return Out2;
}

technique Default_Device
{
	// 기능의 캡슐화
	pass
	{
		alphatestenable = true;
		alpharef = 0;
		alphafunc = greaterequal;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass
	{
		vertexshader = compile vs_3_0 VS_NORMALMAIN();
		pixelshader = compile ps_3_0 PS_NORMALMAIN();
	}

	pass
	{
		vertexshader = compile vs_3_0 VS_NORMALMAIN();
		pixelshader = compile ps_3_0 PS_NORMALMAIN2();
	}

	pass
	{
		alphatestenable = true;
		alpharef = 10;
		alphafunc = greater;

		vertexshader = compile vs_3_0 VS_NORMALMAIN();
		pixelshader = compile ps_3_0 PS_NORMALMAIN2();
	}
};