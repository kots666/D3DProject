#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertex
	{
		_vec3			pos;

	}VTX;

	const _ulong FVF_VTX = D3DFVF_XYZ;

	typedef	struct tagVertexColor
	{
		_vec3			pos;
		_ulong			color;

	}VTXCOL;

	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef	struct tagVertexTexture
	{
		_vec3			pos;
		_vec3			normal;
		_vec2		 	texUV;

	}VTXTEX;
	
	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexTrail
	{
		_vec3 pos;
		_vec2 texUV;
	}VTXTRAIL;

	const _ulong FVF_TRAIL = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVertexScreen
	{
		_vec4 pos;
		_vec2 texUV;
	}VTXSCREEN;

	const _ulong FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef	struct tagVertexCubeTexture
	{
		_vec3			pos;
		_vec3		 	texUV;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	
	typedef	struct tagIndex16
	{
		_ushort	_0, _1, _2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong	_0, _1, _2;

	}INDEX32;
		
	typedef struct D3DXFRAME_EX : public D3DXFRAME
	{
		_matrix combinedTransformationMatrix;
	}D3DXFRAME_EX;


	typedef struct D3DXMESHCONTAINER_EX : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9* texture;

		LPD3DXMESH originMesh; // x������ �ε����� �� ������ ���¸� ������ �ִ� �� ��ü(�Һ�)

		_ulong numBones; // ���� ������ �����ϴ� ����


		_matrix* frameOffsetMatrix; // �ִϸ��̼��� ������ ������ �� ����
		_matrix** frameCombinedMatrix; // frame ���� ������ �ִ� CombinedTransformationMatrix�� �ּҰ����� �����ϱ� ���� ������
		_matrix* renderingMatrix;	//pFrameOffsetMatrix * (*ppFrameCombinedMatrix) ��������� ���������� �������� �ϱ� ���� ���
	}D3DXMESHCONTAINER_EX;
}

#endif
