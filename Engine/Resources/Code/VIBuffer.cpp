#include "VIBuffer.h"

USING(Engine)

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 device) :
	CResources(device), m_VB(nullptr), m_IB(nullptr),
	m_vtxCnt(0), m_triCnt(0), m_vtxSize(0), m_FVF(0),
	m_idxSize(0), m_format(D3DFMT_INDEX16)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs) :
	CResources(rhs), m_VB(rhs.m_VB), m_IB(rhs.m_IB),
	m_vtxCnt(rhs.m_vtxCnt), m_triCnt(rhs.m_triCnt),
	m_vtxSize(rhs.m_vtxSize), m_FVF(rhs.m_FVF),
	m_idxSize(rhs.m_idxSize), m_format(rhs.m_format)
{
	SafeAddRef(m_VB);
	SafeAddRef(m_IB);
}

CVIBuffer::~CVIBuffer()
{
}

HRESULT CVIBuffer::Ready()
{
	if (FAILED(m_device->CreateVertexBuffer(
		m_vtxSize * m_vtxCnt,
		0, // 정적버퍼로 할당하겠다는 옵션
		m_FVF,
		D3DPOOL_MANAGED,
		&m_VB,
		NULL
	)))
	{
		MSG_BOX("Fail to create vertex buffer");
		return E_FAIL;
	}

	if (FAILED(m_device->CreateIndexBuffer(
		m_idxSize * m_triCnt,
		0,
		m_format,
		D3DPOOL_MANAGED,
		&m_IB,
		NULL
	)))
	{
		MSG_BOX("Fail to create index buffer");
		return E_FAIL;
	}

	return S_OK;
}

void CVIBuffer::Render()
{
	// 그래픽 디바이스 장치에게 현재 내가 그리려는 버퍼를 링크시켜주는 함수
	m_device->SetStreamSource(0, m_VB, 0, m_vtxSize);
	// 1인자 : 몇 번 슬롯에 보관할 것인가
	// 2인자 : 어떤 것을 넘겨줄 것인가
	// 3인자 : 어디서부터 그릴 것인가
	// 4인자 : 어떤 단위로 표현할 것인가
	m_device->SetIndices(m_IB);
	m_device->SetFVF(m_FVF);
	m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vtxCnt, 0, m_triCnt);
}

void CVIBuffer::Free()
{
	SafeRelease(m_VB);
	SafeRelease(m_IB);

	CResources::Free();
}

