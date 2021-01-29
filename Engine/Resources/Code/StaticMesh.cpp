#include "StaticMesh.h"
#include "Texture.h"

USING(Engine)

CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 device) :
	CMesh(device),
	m_normalTex(nullptr)
{
}

CStaticMesh::CStaticMesh(const CStaticMesh& rhs) :
	CMesh(rhs), m_adjacency(rhs.m_adjacency),
	m_subset(rhs.m_subset), m_mesh(rhs.m_mesh),
	m_originMesh(rhs.m_originMesh), m_mtrl(rhs.m_mtrl),
	m_subsetCnt(rhs.m_subsetCnt), m_numVtx(rhs.m_numVtx),
	m_vtxPos(rhs.m_vtxPos), m_stride(rhs.m_stride),
	m_normalTex(rhs.m_normalTex)
{
	m_textures = new LPDIRECT3DTEXTURE9[rhs.m_subsetCnt];

	for (_ulong i = 0; i < rhs.m_subsetCnt; ++i)
	{
		m_textures[i] = rhs.m_textures[i];
		SafeAddRef(m_textures[i]);
	}

	SafeAddRef(m_originMesh);
	SafeAddRef(m_mesh);
	SafeAddRef(m_adjacency);
	SafeAddRef(m_subset);
}

CStaticMesh::~CStaticMesh()
{
}

HRESULT CStaticMesh::Ready(const _tchar* filePath, const _tchar* fileName)
{
	m_normalTex = nullptr;

	_tchar fullPath[MAX_PATH] = L"";

	lstrcpy(fullPath, filePath);
	lstrcat(fullPath, fileName);

	// X파일 메쉬 로드
	if (FAILED(D3DXLoadMeshFromX(fullPath, D3DXMESH_MANAGED, m_device,
		&m_adjacency, &m_subset, NULL, &m_subsetCnt, &m_originMesh)))
		return E_FAIL;

	_ulong FVF = m_originMesh->GetFVF();

	// 노말 값이 없는 경우
	if (!(FVF & D3DFVF_NORMAL))
	{
		// mesh에 originMesh FVF + normal로 복사.
		m_originMesh->CloneMeshFVF(m_originMesh->GetOptions(), FVF | D3DFVF_NORMAL, m_device, &m_mesh);
		D3DXComputeNormals(m_mesh, (_ulong*)m_adjacency->GetBufferPointer());
	}
	else
	{
		m_originMesh->CloneMeshFVF(m_originMesh->GetOptions(), FVF | D3DFVF_NORMAL, m_device, &m_mesh);
	}

	void* vertices = nullptr;

	// 메쉬가 지닌 정점의 개수를 반환
	m_numVtx = m_mesh->GetNumVertices();
	m_vtxPos = new _vec3[m_numVtx];

	m_mesh->LockVertexBuffer(0, &vertices);

	// 정점 정보 중 position의 위치 찾기
	D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_mesh->GetDeclaration(decl);

	_ubyte byOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffset = (_ubyte)decl[i].Offset;
			break;
		}
	}
	// FVF 정보를 토대로 정점의 크기를 반환하는 함수
	m_stride = D3DXGetFVFVertexSize(FVF);

	for (_ulong i = 0; i < m_numVtx; ++i)
	{
		m_vtxPos[i] = *((_vec3*)(((_ubyte*)vertices) + (i * m_stride + byOffset)));
	}

	m_mesh->UnlockVertexBuffer();

	// 메쉬가 지닌 재질 정보 중 첫 번째 주소를 반환하여 저장
	m_mtrl = (D3DXMATERIAL*)m_subset->GetBufferPointer();

	// 메쉬에서 얻어온 서브셋의 갯수만큼 텍스쳐를 동적할당.
	m_textures = new LPDIRECT3DTEXTURE9[m_subsetCnt];

	for (_ulong i = 0; i < m_subsetCnt; ++i)
	{
		_tchar texFileName[256] = L"";

		// fullPath에 filePath를 넣어둔다.
		lstrcpy(fullPath, filePath);

		// fileName 문자열에 mtrl에 아스키코드로 저장된 텍스쳐파일명을 저장한다.
		MultiByteToWideChar(CP_ACP, 
			0,
			m_mtrl[i].pTextureFilename,
			strlen(m_mtrl[i].pTextureFilename), 
			texFileName,
			256);

		// fullPath 뒤에 얻어온 텍스쳐파일명을 붙여준다.
		lstrcat(fullPath, texFileName);

		// 텍스쳐 파일을 로드한다.
		if (FAILED(D3DXCreateTextureFromFile(m_device, fullPath, &m_textures[i])))
			return E_FAIL;
	}

	return S_OK;
}

void CStaticMesh::Render()
{
	for (_ulong i = 0; i < m_subsetCnt; ++i)
	{
		m_device->SetTexture(0, m_textures[i]);
		m_mesh->DrawSubset(i);
	}
}

void CStaticMesh::Render(LPD3DXEFFECT & effect)
{
	_int normalTexSize = 0;

	if (nullptr != m_normalTex)
		normalTexSize = m_normalTex->GetSize();

	for (_ulong i = 0; i < m_subsetCnt; ++i)
	{
		effect->SetTexture("g_BaseTexture", m_textures[i]);

		if (i < normalTexSize)
			m_normalTex->SetTexture(effect, "g_NormalTexture", i);

		effect->CommitChanges();

		if (nullptr == m_normalTex)
			effect->BeginPass(0);
		else
		{
			if (m_normalTex->IsBlue(i))
				effect->BeginPass(1);
			else
				effect->BeginPass(4);
		}

		m_mesh->DrawSubset(i);

		effect->EndPass();
	}
}


CStaticMesh* CStaticMesh::Create(LPDIRECT3DDEVICE9 device, const _tchar* filePath, const _tchar* fileName)
{
	CStaticMesh* instance = new CStaticMesh(device);

	if (FAILED(instance->Ready(filePath, fileName)))
		SafeRelease(instance);

	return instance;
}

CComponent* CStaticMesh::Clone()
{
	return new CStaticMesh(*this);
}

void CStaticMesh::Free()
{
	for (_ulong i = 0; i < m_subsetCnt; ++i)
		SafeRelease(m_textures[i]);

	SafeDeleteArray(m_textures);

	if (!m_isClone) SafeDeleteArray(m_vtxPos);

	SafeRelease(m_subset);
	SafeRelease(m_adjacency);
	SafeRelease(m_mesh);
	SafeRelease(m_originMesh);

	CMesh::Free();
}
